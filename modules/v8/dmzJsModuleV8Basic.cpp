#include <dmzJsExtV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include "dmzJsModuleV8Basic.h"
#include <dmzJsKernelEmbed.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilStrings.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeConfigToStringContainer.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

#include <string.h>
#include <stdio.h>

#include <qdb.h>
static dmz::qdb out;

namespace {

static const char LocalInstanceHeader[] = "(function (self, require) { ";
static const size_t LocalInstanceHeaderLength = strlen (LocalInstanceHeader);
static const char LocalRequireHeader[] = "(function (exports, require) { ";
static const size_t LocalRequireHeaderLength = strlen (LocalRequireHeader);
static const char LocalFooter[] = "\n});";
static const size_t LocalFooterLength = strlen (LocalFooter);

void
local_instance_struct_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      dmz::JsModuleV8Basic::InstanceStructBase *ptr =
         (dmz::JsModuleV8Basic::InstanceStructBase *)param;

      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}


v8::Handle<v8::Value>
local_print (const v8::Arguments &Args) {

   v8::HandleScope scope;

   dmz::StreamLog *out = (dmz::StreamLog *)v8::External::Unwrap (Args.Data ());

   if (out) {

      const int Length = Args.Length ();

      for (int ix = 0; ix < Length; ix++) {

         if (ix > 0) { *out << " "; }

         *out << dmz::v8_to_string (Args[ix]);
      }

      *out << dmz::endl;
   }

   return scope.Close (v8::Undefined());
}


v8::Handle<v8::Value>
local_require (const v8::Arguments &Args) {

   v8::HandleScope scope;

   dmz::JsModuleV8Basic *module =
      (dmz::JsModuleV8Basic *)v8::External::Unwrap (Args.Data ());

   v8::Handle<v8::Value> result;

   if ((Args.Length () == 1) && module) {

      result = module->require (dmz::v8_to_string (Args[0]));
   }

   if (result.IsEmpty ()) {

      dmz::String msg ("Require unable to resolve: '");
      msg << dmz::v8_to_string (Args[0]) << "'.";

      return v8::ThrowException (
         v8::Exception::Error (v8::String::New (msg.get_buffer())));
   }
   else { return scope.Close (result); }
}


v8::Handle<v8::Value>
global_setter (
      v8::Local<v8::String> property,
      v8::Local<v8::Value> value,
      const v8::AccessorInfo &info) {

   v8::HandleScope scope;

   dmz::String err ("Global variables forbidden. Unable to initialize: ");
   err << dmz::v8_to_string (property);

   return v8::ThrowException (v8::Exception::Error (v8::String::New (err.get_buffer ())));
}

};


dmz::JsModuleV8Basic::JsModuleV8Basic (
      const PluginInfo &Info,
      Config &local,
      Config &global) :
      Plugin (Info),
      TimeSlice (Info),
      JsModuleV8 (Info),
      _log (Info),
      _out ("", LogLevelOut, Info.get_context ()),
      _rc (Info),
      _defs (Info, &_log),
      _shutdown (False),
      _reset (True),
      _runtime (0) {

   _init (local, global);
}


dmz::JsModuleV8Basic::~JsModuleV8Basic () {

   HashTableStringIterator it;
   InstanceStruct *is (0);

   while (_instanceTable.get_next (it, is)) { _defs.release_unique_name (is->Name); }

   _extTable.clear ();
   _instanceTable.empty ();
   _scriptTable.empty ();

   _globalTemplate.Dispose (); _globalTemplate.Clear ();
   _instanceTemplate.Dispose (); _instanceTemplate.Clear ();
   _requireFuncTemplate.Dispose (); _requireFuncTemplate.Clear ();
   _requireFunc.Dispose (); _requireFunc.Clear ();

   _context.Dispose (); _context.Clear ();

   while (!v8::V8::IdleNotification ()) {;}
   v8::V8::Dispose ();
}


// Plugin Interface
void
dmz::JsModuleV8Basic::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

      update_time_slice (0.0);
   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

      _shutdown = True;
      _release_instances ();
      _shutdown_ext ();
   }
}


void
dmz::JsModuleV8Basic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      JsExtV8 *ext = JsExtV8::cast (PluginPtr);

      if (ext && _extTable.store (ext->get_js_ext_v8_handle (), ext)) {

         ext->update_js_module_v8 (JsExtV8::Store, *this);

         if (!_context.IsEmpty ()) {

            ext->update_js_context_v8 (_context);
            ext->update_js_ext_v8_state (JsExtV8::Register);
            ext->update_js_ext_v8_state (JsExtV8::Init);
            reset_v8 ();
         }
      }

      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      JsExtV8 *ext = JsExtV8::cast (PluginPtr);

      if (ext && _extTable.remove (ext->get_js_ext_v8_handle ())) {

         if (!_shutdown) {

            ext->update_js_ext_v8_state (JsExtV8::Shutdown);
         }

         ext->update_js_module_v8 (JsExtV8::Remove, *this);

         if (!_shutdown) { reset_v8 (); }
      }

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
   }
}


#ifdef DMZ_JS_V8_HEAP_DEBUG
namespace { static int last = 0; }
#endif

// TimeSlice Interface
void
dmz::JsModuleV8Basic::update_time_slice (const Float64 DeltaTime) {

   v8::V8::IdleNotification ();

#ifdef DMZ_JS_V8_HEAP_DEBUG
   v8::HeapStatistics hs;
   v8::V8::GetHeapStatistics (&hs);
   const int Size = hs.total_heap_size ();

   if (Size != last) {

      out << Size << " " << Size - last << endl;
      last = Size;
   }
#endif

   if (_reset) {

      _release_instances ();
      _shutdown_ext ();
      _init_context ();
      v8::Context::Scope cscope (_context);
      _init_ext ();
      _load_scripts ();
      _reset = False;
   }
}


// JsModuleV8 Interface
void
dmz::JsModuleV8Basic::reset_v8 () { _reset = True; }


v8::Handle<v8::Context>
dmz::JsModuleV8Basic::get_v8_context () { return _context; }


dmz::Boolean
dmz::JsModuleV8Basic::register_interface (            
      const String &Name,
      v8::Persistent<v8::Object> object) {

   Boolean result (False);

   v8::HandleScope scope;
   v8::Persistent<v8::Object> *ptr = new v8::Persistent<v8::Object>;
   *ptr = object;

   if (_requireTable.store (Name, ptr)) { result = True; }
   else {

      delete ptr; ptr = 0;
      _log.error << "Failed to register interface: " << Name
         << ". Name not unique?" << endl;
   }

   return result;
}


v8::Handle<v8::Object>
dmz::JsModuleV8Basic::require (const String &Value) {

   v8::HandleScope scope;

   v8::Handle<v8::Object> result;
   String scriptPath;

   V8Object *ptr = _requireTable.lookup (Value);

   if (!ptr) {

      find_file (_localPaths, Value + ".js", scriptPath);
      if (scriptPath) { ptr = _requireTable.lookup (scriptPath); }
      else {

         find_file (_localPaths, Value, scriptPath);

         if (scriptPath) {

            _log.warn << "Require parameter: " << Value
               << " should not contain the .js suffix" << endl;
            ptr = _requireTable.lookup (scriptPath);
         }
      }
   }

   if (ptr) { result = *ptr; }
   else {

      v8::String::ExternalAsciiStringResource *sptr (0);

      if (scriptPath) {
      
         sptr = new V8FileString (scriptPath, LocalRequireHeader, LocalFooter);
      }
      else if (Value.contains_sub ("dmz/")) {

      }

      if (sptr) {

         _log.info << "Loaded required script: " << scriptPath << endl;
         v8::TryCatch tc;

         v8::Handle<v8::Script> script = v8::Script::Compile (
            v8::String::NewExternal (sptr),
            v8::String::New (scriptPath.get_buffer ()));

         if (tc.HasCaught ()) { handle_v8_exception (tc); }
         else {

            V8Value value = script->Run ();

            if (tc.HasCaught ()) { handle_v8_exception (tc); }
            else {

               V8Function func = v8_to_function (value);

               if (func.IsEmpty ()) {
                  // Error! no function returned.
                  _log.error << "No function returned from: " << scriptPath << endl;
               }
               else {

                  v8::Persistent<v8::Object> *ptr = new v8::Persistent<v8::Object>;
                  *ptr = v8::Persistent<v8::Object>::New (v8::Object::New ());

                  if (_requireTable.store (scriptPath, ptr)) {

                     result = *ptr;
                     v8::Handle<v8::Value> argv[] = { result, _requireFunc };
                     v8::Handle<v8::Value> value = func->Call (result, 2, argv);

                     if (tc.HasCaught ()) { handle_v8_exception (tc); }
                  }
                  else {

                     _log.error << "Failed to store require: " << scriptPath << endl;
                     ptr->Dispose (); ptr->Clear ();
                     delete ptr; ptr = 0;
                  }
               }
            }
         }
      }
      else {

         _log.error << "Failed to load require script: " << Value << endl;
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsModuleV8Basic::get_require_list (StringContainer &list) {

   list.clear ();

   HashTableStringIterator it;
   V8ObjectPersist *ptr (0);

   while (_requireTable.get_next (it, ptr)) { list.append (it.get_hash_key ()); }
}


void
dmz::JsModuleV8Basic::handle_v8_exception (v8::TryCatch &tc) {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   const String ExStr = v8_to_string (tc.Exception ());

   v8::Handle<v8::Message> message = tc.Message ();

   if (message.IsEmpty()) {

      _log.error << "Unhandled exception: " << ExStr << endl;
   }
   else {

      const String FileName = v8_to_string (message->GetScriptResourceName ());
      const Int32 Line = message->GetLineNumber ();
      _log.error << FileName << ":" << Line << ": " << ExStr << endl;

         // Print line of source code.
         _log.error << v8_to_string (message->GetSourceLine ()) << endl;

         // Print wavy underline (GetUnderline is deprecated).
         int start = message->GetStartColumn ();
         int end = message->GetEndColumn ();

         String space;
         space.repeat ("-", start - 1);
         String line;
         line.repeat ("^", end - start);

         _log.error << " " << space << line << endl;

         _log.error << "Stack trace: " << endl << v8_to_string (tc.StackTrace ()) << endl;
   }
}


dmz::String
dmz::JsModuleV8Basic::get_instance_name (V8Value value) {

   v8::HandleScope scope;

   String result;

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      InstanceStructBase *is (0);
      V8Value wrap =  obj->GetHiddenValue (_instanceAttrName);

      if (wrap.IsEmpty () == false) {

         is = (InstanceStructBase *)v8::External::Unwrap (wrap);
      }

      if (is) { result = is->Name; }
   }

   return result;
}


dmz::Handle
dmz::JsModuleV8Basic::get_instance_handle (V8Value value) {

   v8::HandleScope scope;

   Handle result (0);

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      InstanceStructBase *is (0);
      V8Value wrap =  obj->GetHiddenValue (_instanceAttrName);

      if (wrap.IsEmpty () == false) {

         is = (InstanceStructBase *)v8::External::Unwrap (wrap);
      }

      if (is) { result = is->Object; }
   }

   return result;
}


dmz::Boolean
dmz::JsModuleV8Basic::set_external_instance_handle_and_name (
      const Handle TheHandle,
      const String &TheName,
      v8::Handle<v8::Value> value) {

   dmz::Boolean result (False);

   v8::HandleScope scope;

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      InstanceStructBase *is (0);
      V8Value wrap =  obj->GetHiddenValue (_instanceAttrName);

      if (wrap.IsEmpty () == false) {

         is = (InstanceStructBase *)v8::External::Unwrap (wrap);
      }

      if (!is) {

         is = new InstanceStructBase (TheName, TheHandle);

         if (is) {

            obj->SetHiddenValue (_instanceAttrName, v8::External::Wrap ((void *)is));
            V8ObjectPersist dtor = V8ObjectPersist::New (obj);
            dtor.MakeWeak ((void *)is, local_instance_struct_delete); 
         }
      }
   }


   return result;
}


void
dmz::JsModuleV8Basic::_empty_require () {

   v8::HandleScope scope;

   HashTableStringIterator it;
   v8::Persistent<v8::Object> *ptr (0);

   while (_requireTable.get_next (it, ptr)) { ptr->Dispose (); ptr->Clear (); }

   _requireTable.empty ();
}


void
dmz::JsModuleV8Basic::_release_instances () {

   if (_context.IsEmpty () == false) {

      v8::HandleScope scope;

      HashTableStringIterator it;
      InstanceStruct *is (0);

      while (_instanceTable.get_next (it, is)) {

         if (is->self.IsEmpty () == False) {

            V8Function func = v8_to_function (is->self->Get (_shutdownFuncName));

            if (func.IsEmpty () == false) {

               v8::TryCatch tc;
               v8::Handle<v8::Value> argv[] = { is->self };
               func->Call (is->self, 1, argv);
               if (tc.HasCaught ()) { handle_v8_exception (tc); }
            }
         }
      }

      it.reset ();
      is = 0;

      while (_instanceTable.get_next (it, is)) {

         if (is->self.IsEmpty () == False) { is->self.Dispose (); is->self.Clear (); }
      }
   }
}


void
dmz::JsModuleV8Basic::_init_context () {

   v8::HandleScope scope;

   _empty_require ();

   _requireFunc.Dispose (); _requireFunc.Clear ();
   _instanceAttrName.Dispose (); _instanceAttrName.Clear ();
   _shutdownFuncName.Dispose (); _shutdownFuncName.Clear ();

   if (_globalTemplate.IsEmpty ()) {

      _globalTemplate = v8::Persistent<v8::ObjectTemplate>::New (
         v8::ObjectTemplate::New ());

      _globalTemplate->SetNamedPropertyHandler (0, global_setter);
   }

   if (_instanceTemplate.IsEmpty ()) {

      _instanceTemplate = v8::Persistent<v8::ObjectTemplate>::New (
         v8::ObjectTemplate::New ());
   }

   if (_requireFuncTemplate.IsEmpty ()) {

      _requireFuncTemplate = v8::Persistent<v8::FunctionTemplate>::New (
         v8::FunctionTemplate::New (local_require, v8::External::Wrap (this)));
   }

//   char flags[] = "--expose_debug_as V8";
//   v8::V8::SetFlagsFromString (flags, strlen (flags));
   while (!v8::V8::IdleNotification ()) {;}

   if (_context.IsEmpty () == false) {

      _context.Dispose (); _context.Clear ();
      v8::V8::ContextDisposedNotification ();
   }

   _context = v8::Context::New (0, _globalTemplate);

   v8::Context::Scope cscope (_context);

   _instanceAttrName = V8StringPersist::New (v8::String::NewSymbol ("dmz::InstanceInfo"));

   _shutdownFuncName = V8StringPersist::New (v8::String::NewSymbol ("shutdown"));

   _init_builtins ();

   _requireFunc = v8::Persistent<v8::Function>::New (
      _requireFuncTemplate->GetFunction ());
}


void
dmz::JsModuleV8Basic::_init_builtins () {

   v8::HandleScope scope;

   v8::Persistent<v8::Object> *ptr = new v8::Persistent<v8::Object>;

   if (ptr) {

      *ptr= v8::Persistent<v8::Object>::New (v8::Object::New ());

      (*ptr)->Set (
         v8::String::NewSymbol ("puts"),
         v8::FunctionTemplate::New (
            local_print,
            v8::External::Wrap (&_out))->GetFunction ());

      if (!_requireTable.store ("sys", ptr)) {

         ptr->Dispose (); ptr->Clear ();
         delete ptr; ptr = 0;
      }
   }
}


void
dmz::JsModuleV8Basic::_init_ext () {

   v8::HandleScope scope;

   HashTableHandleIterator it;
   JsExtV8 *ext (0);

   while (_extTable.get_next (it, ext)) {

      ext->update_js_context_v8 (_context);
      ext->update_js_ext_v8_state (JsExtV8::Register);
   }

   it.reset ();
   ext = 0;

   while (_extTable.get_next (it, ext)) {

      ext->update_js_ext_v8_state (JsExtV8::Init);
   }
}


void
dmz::JsModuleV8Basic::_shutdown_ext () {

   if (_context.IsEmpty () == false) {

      v8::HandleScope scope;

      HashTableHandleIterator it;
      JsExtV8 *ext (0);

      while (_extTable.get_next (it, ext)) {

         ext->update_js_ext_v8_state (JsExtV8::Shutdown);
      }
   }
}


void
dmz::JsModuleV8Basic::_load_scripts () {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   HashTableStringIterator it;
   ScriptStruct *info (0);

   while (_scriptTable.get_next (it, info)) {

      info->clear ();
      
      v8::TryCatch tc;

      V8FileString *sptr =
         new V8FileString (info->FileName, LocalInstanceHeader, LocalFooter);

      v8::Handle<v8::Script> script = v8::Script::Compile (
         v8::String::NewExternal (sptr),
         v8::String::New (info->FileName.get_buffer ()));

      if (tc.HasCaught ()) { handle_v8_exception (tc); }
      else {

         _log.info << "Loaded script: " << info->FileName << endl;

         info->script = v8::Persistent<v8::Script>::New (script);
         v8::Handle<v8::Value> value = info->script->Run ();

         if (tc.HasCaught ()) { handle_v8_exception (tc); }
         else {

            V8Function func = v8_to_function (value);
            if (func.IsEmpty ()) {
               // Error! no function returned.
               _log.error << "No function returned from: " << info->FileName << endl;
            }
            else {

               info->ctor = v8::Persistent<v8::Function>::New (func);
            }
         }
      }
   }

   it.reset ();
   InstanceStruct *instance (0);

   while (_instanceTable.get_next (it, instance)) {

      if (!instance->script.ctor.IsEmpty ()) {

         v8::TryCatch tc;

         instance->self =
            v8::Persistent<v8::Object>::New (_instanceTemplate->NewInstance ());;

         InstanceStructBase *isb = instance;

         instance->self->SetHiddenValue (
            _instanceAttrName,
            v8::External::Wrap ((void *)isb));

         instance->self->Set (
            v8::String::NewSymbol ("name"),
            v8::String::New (instance->Name.get_buffer ()));

         if (_runtime) {

            instance->self->Set (
               v8::String::NewSymbol ("config"),
               _runtime->create_v8_config (&(instance->local)));

            instance->self->Set (
               v8::String::NewSymbol ("log"),
               _runtime->create_v8_log (instance->Name));
         }

         v8::Handle<v8::Value> argv[] = { instance->self, _requireFunc };

         V8Value value = instance->script.ctor->Call (instance->self, 2, argv);

         if (tc.HasCaught ()) { handle_v8_exception (tc); }
         else {

            _log.info << "Created instance: " << instance->Name
               << " from script: " << instance->script.FileName << endl;
         }
      }
   }
}


dmz::JsModuleV8Basic::ScriptStruct *
dmz::JsModuleV8Basic::_find_script (Config &script) {

   ScriptStruct *result (0);

   const String Name = config_to_string ("name", script);

   if (Name) {

      String scriptPath = _rc.find_file (Name);

      if (!scriptPath && (_localPaths.get_count () > 0)) {

         find_file (_localPaths, Name + ".js", scriptPath);
      }

      if (scriptPath) {

         result = _scriptTable.lookup (scriptPath);

         if (!result) {

            result = new ScriptStruct (Name, scriptPath);

            if (!_scriptTable.store (scriptPath, result)) {

               delete result; result = 0;
               _log.error << "Failed to add script: " << scriptPath << endl
                  << "Has script already been specified?" << endl;
            }
         }
      }
      else {

         _log.error << "Failed to find script resource: " << Name << endl;
      }
   }

   return result;
}


void
dmz::JsModuleV8Basic::_init (Config &local, Config &global) {

   _localPaths = config_to_path_string_container (local);

   Config scriptList;

   if (local.lookup_all_config ("script", scriptList)) {

      ConfigIterator it;
      Config script;

      while (scriptList.get_next_config (it, script)) { _find_script (script); }
   }

   Config instanceList;

   if (local.lookup_all_config ("instance", instanceList)) {

      ConfigIterator it;
      Config instance;

      while (instanceList.get_next_config (it, instance)) {

         ScriptStruct *ss = _find_script (instance);

         if (ss) {

            const String UniqueName = config_to_string ("unique", instance, ss->Name);
            const String ScopeName = config_to_string ("scope", instance, UniqueName);

            if (_defs.create_unique_name (UniqueName)) {

               InstanceStruct *ptr = new InstanceStruct (
                  UniqueName,
                  _defs.create_named_handle (UniqueName),
                  *ss);

               if (!_instanceTable.store (UniqueName, ptr)) {

                  delete ptr; ptr = 0;
                  _log.error << "Script instance name: " << UniqueName
                     << " is not unique." << endl;
               }
               else {

                  Config local (ScopeName);
                  global.lookup_all_config_merged (String ("dmz.") + ScopeName, local);
                  ptr->local = local;
               }
            }
            else {

               _log.error << "Unable to register script instance: " << UniqueName
                  << ". Because name is not unique." << endl;
            }
         }
      }
   }

   _log.info << "Using V8 v" << v8::V8::GetVersion () << endl;
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleV8Basic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleV8Basic (Info, local, global);
}

};
