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

#include <strings.h>
#include <stdio.h>

namespace {

static const char LocalInstanceHeader[] = "(function (self, require) {\n";
static const size_t LocalInstanceHeaderLength = strlen (LocalInstanceHeader);
static const char LocalRequireHeader[] = "(function (exports, require) {\n";
static const size_t LocalRequireHeaderLength = strlen (LocalRequireHeader);
static const char LocalFooter[] = "\n});";
static const size_t LocalFooterLength = strlen (LocalFooter);

v8::Handle<v8::Value>
local_print (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::StreamLog *out = (dmz::StreamLog *)v8::External::Unwrap (args.Data ());

   if (out) {

      const int Length = args.Length ();

      for (int ix = 0; ix < Length; ix++) {

         if (ix > 0) { *out << " "; }

         *out << dmz::v8_to_string (args[ix]);
      }

      *out << dmz::endl;
   }

   return scope.Close (v8::Undefined());
}


v8::Handle<v8::Value>
local_require (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::JsModuleV8Basic *module =
      (dmz::JsModuleV8Basic *)v8::External::Unwrap (args.Data ());

   v8::Handle<v8::Value> result;

   if (module) {

      result = module->require (*(v8::String::Utf8Value (args[0])));
   }

   return result.IsEmpty () ? result : scope.Close (result);
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
      TimeSlice (Info, TimeSliceTypeRuntime, TimeSliceModeSingle, 0.0),
      JsModuleV8 (Info),
      _log (Info),
      _out ("", LogLevelOut, Info.get_context ()),
      _rc (Info),
      _defs (Info, &_log),
      _shutdown (False),
      _runtime (0) {

   _init (local, global);
}


dmz::JsModuleV8Basic::~JsModuleV8Basic () {

   _extTable.clear ();
   _instanceTable.empty ();
   _scriptTable.empty ();

   _globalTemplate.Dispose (); _globalTemplate.Clear ();
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


// TimeSlice Interface
void
dmz::JsModuleV8Basic::update_time_slice (const Float64 DeltaTime) {

   _release_instances ();
   _shutdown_ext ();
   _init_context ();
   v8::Context::Scope cscope (_context);
   _init_ext ();
   _load_scripts ();
}


// JsModuleV8 Interface
void
dmz::JsModuleV8Basic::reset_v8 () { start_time_slice (); }


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

         if (script.IsEmpty ()) { handle_v8_exception (tc); }
         else {

            V8Value value = script->Run ();

            if (value.IsEmpty ()) { handle_v8_exception (tc); }
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

                     if (value.IsEmpty ()) { handle_v8_exception (tc); }
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

      HashTableStringIterator it;
      InstanceStruct *is (0);

      while (_instanceTable.get_next (it, is)) {

         _defs.release_unique_name (is->Name);
      }
   }
}


void
dmz::JsModuleV8Basic::_init_context () {

   v8::HandleScope scope;

   _empty_require ();

   if (!_context.IsEmpty ()) { _context.Dispose (); _context.Clear (); }

   _requireFunc.Dispose (); _requireFunc.Clear ();

   if (_globalTemplate.IsEmpty ()) {

      _globalTemplate = v8::Persistent<v8::ObjectTemplate>::New (
         v8::ObjectTemplate::New ());

      _globalTemplate->SetNamedPropertyHandler (0, global_setter);
   }

   if (_requireFuncTemplate.IsEmpty ()) {

      _requireFuncTemplate = v8::Persistent<v8::FunctionTemplate>::New (
         v8::FunctionTemplate::New (local_require, v8::External::Wrap (this)));
   }

//   char flags[] = "--expose_debug_as V8";
//   v8::V8::SetFlagsFromString (flags, strlen (flags));

   _context = v8::Context::New (0, _globalTemplate);

   v8::Context::Scope cscope (_context);

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

      if (script.IsEmpty ()) { handle_v8_exception (tc); }
      else {

         _log.info << "Loaded script: " << info->FileName << endl;

         info->script = v8::Persistent<v8::Script>::New (script);
         v8::Handle<v8::Value> value = info->script->Run ();

         if (value.IsEmpty ()) { handle_v8_exception (tc); }
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

      if (_defs.create_unique_name (instance->Name)) {

         if (!instance->script.ctor.IsEmpty ()) {

            v8::TryCatch tc;

            v8::Handle<v8::Object> self = v8::Object::New ();

            self->Set (
               v8::String::NewSymbol ("name"),
               v8::String::New (instance->Name.get_buffer ()));

            if (_runtime) {

               self->Set (
                  v8::String::NewSymbol ("config"),
                  _runtime->create_v8_config (&(instance->local)));

               self->Set (
                  v8::String::NewSymbol ("log"),
                  _runtime->create_v8_log (instance->Name));
            }

            v8::Handle<v8::Value> argv[] = { self, _requireFunc };

            v8::Handle<v8::Value> value = instance->script.ctor->Call (self, 2, argv);

            if (value.IsEmpty ()) { handle_v8_exception (tc); }
            else {

               _log.info << "Created instance: " << instance->Name
                  << " from script: " << instance->script.FileName << endl;
            }
         }
      }
      else {

         _log.error << "Unable to create: " << instance->Name << " instance of: "
            << instance->script.FileName << " script because name is  not unique."
            << endl;
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

            InstanceStruct *ptr = new InstanceStruct (UniqueName, *ss);

            if (!_instanceTable.store (UniqueName, ptr)) {

               delete ptr; ptr = 0;
               _log.error << "Script instance name: " << UniqueName << " is not unique."
                  << endl;
            }
            else {

               Config local (ScopeName);
               global.lookup_all_config_merged (String ("dmz.") + ScopeName, local);
               ptr->local = local;
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
