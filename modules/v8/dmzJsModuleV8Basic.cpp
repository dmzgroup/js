#include <dmzJsExtV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include "dmzJsModuleV8Basic.h"
#include <dmzJsKernelEmbed.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilStrings.h>
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

const char *
to_c_string (const v8::String::Utf8Value& value) {

   return *value ? *value : "<string conversion failed>";
}


v8::Handle<v8::Value>
local_print (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::StreamLog *out = (dmz::StreamLog *)v8::External::Unwrap (args.Data ());

   if (out) {

      const int Length = args.Length ();

      for (int ix = 0; ix < Length; ix++) {

         if (ix > 0) { *out << " "; }

         v8::String::Utf8Value str (args[ix]);
         const char *cstr = to_c_string (str);
         *out << cstr;
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
   v8::String::Utf8Value val (property);
   err << to_c_string (val);

   return v8::ThrowException (v8::Exception::Error (v8::String::New (err.get_buffer ())));
}

};


dmz::JsModuleV8Basic::JsModuleV8Basic (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      TimeSlice (Info, TimeSliceTypeRuntime, TimeSliceModeSingle, 0.0),
      JsModuleV8 (Info),
      _log (Info),
      _out ("", LogLevelOut, Info.get_context ()),
      _rc (Info),
      _runtime (0) {

   _init (local);
}


dmz::JsModuleV8Basic::~JsModuleV8Basic () {

   _extTable.clear ();
   _instanceTable.empty ();
   _scriptTable.empty ();

   _globalTemplate.Dispose ();
   _requireFuncTemplate.Dispose ();
   _requireFunc.Dispose ();

   _context.Dispose ();

   while (!v8::V8::IdleNotification ()) {;}
   v8::V8::Dispose ();
}


// Plugin Interface
void
dmz::JsModuleV8Basic::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

      reset_v8 ();
   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

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
         }
      }

      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      JsExtV8 *ext = JsExtV8::cast (PluginPtr);

      if (ext && _extTable.remove (ext->get_js_ext_v8_handle ())) {

         ext->update_js_module_v8 (JsExtV8::Remove, *this);
      }

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
   }
}


// TimeSlice Interface
void
dmz::JsModuleV8Basic::update_time_slice (const Float64 DeltaTime) {

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

         if (script.IsEmpty ()) { _handle_exception (tc); }
         else {

            v8::Handle<v8::Value> value = script->Run ();

            if (value.IsEmpty ()) { _handle_exception (tc); }
            else {

               v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast (value);

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

                     if (value.IsEmpty ()) { _handle_exception (tc); }
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
dmz::JsModuleV8Basic::_empty_require () {

   v8::HandleScope scope;

   HashTableStringIterator it;
   v8::Persistent<v8::Object> *ptr (0);

   while (_requireTable.get_next (it, ptr)) { ptr->Dispose (); ptr->Clear (); }

   _requireTable.empty ();
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
dmz::JsModuleV8Basic::_handle_exception (v8::TryCatch &tc) {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   v8::String::Utf8Value exception (tc.Exception ());
   const String ExStr = to_c_string (exception);

   v8::Handle<v8::Message> message = tc.Message ();

   if (message.IsEmpty()) {

      _log.error << "Unhandled exception: " << ExStr << endl;
   }
   else {

      v8::String::Utf8Value fptr (message->GetScriptResourceName ());
      const String FileName = to_c_string (fptr);
      const Int32 Line = message->GetLineNumber ();
      _log.error << FileName << ":" << Line << ": " << ExStr << endl;

         // Print line of source code.
         v8::String::Utf8Value sptr (message->GetSourceLine ());
         _log.error << to_c_string (sptr) << endl;

         // Print wavy underline (GetUnderline is deprecated).
         int start = message->GetStartColumn ();
         int end = message->GetEndColumn ();

         String space;
         space.repeat ("-", start - 1);
         String line;
         line.repeat ("^", end - start);

         _log.error << " " << space << line << endl;

         v8::String::Utf8Value stack (tc.StackTrace ());
         _log.error << "Stack trace: " << endl << to_c_string (stack) << endl;
   }
}


/*
void
dmz::JsModuleV8Basic::_load_kernel () {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   if (_kernelList) { delete _kernelList; _kernelList = 0; }

   ScriptStruct *last (0);

   const int Count = get_embedded_js_count ();

   for (int ix = 0; ix < Count; ix++) {

      V8EmbeddedString *buffer = new V8EmbeddedString (
         get_embedded_js_text (ix),
         get_embedded_js_length (ix));

      ScriptStruct *ss = new ScriptStruct (get_embedded_js_file_name (ix));

      if (ss) {

         v8::TryCatch tc;

         v8::Handle<v8::Script> script = v8::Script::Compile (
            v8::String::NewExternal (buffer),
            v8::String::New (get_embedded_js_file_name (ix)));

         if (script.IsEmpty ()) { _handle_exception (tc); }
         else {

            ss->script = v8::Persistent<v8::Script>::New (script);
            if (last) { last->next = ss; last = ss; }
            else { _kernelList = last = ss; }
         }
      }
   }
 
   V8EmbeddedString *buffer = new V8EmbeddedString (
      get_v8_internal_text (0),
      get_v8_internal_length (0));

   ScriptStruct *ss = new ScriptStruct (get_v8_internal_file_name (0));

   if (ss) {

      v8::TryCatch tc;

      v8::Handle<v8::Script> script = v8::Script::Compile (
         v8::String::NewExternal (buffer),
         v8::String::New (get_v8_internal_file_name (0)));

      if (script.IsEmpty ()) { _handle_exception (tc); }
      else {

         ss->script = v8::Persistent<v8::Script>::New (script);
         if (last) { last->next = ss; last = ss; }
         else { _kernelList = last = ss; }
      }
   }  
}
*/


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

      if (script.IsEmpty ()) { _handle_exception (tc); }
      else {

         _log.info << "Loaded script: " << info->FileName << endl;

         info->script = v8::Persistent<v8::Script>::New (script);
         v8::Handle<v8::Value> value = info->script->Run ();

         if (value.IsEmpty ()) { _handle_exception (tc); }
         else {

            v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast (value);
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

         v8::Handle<v8::Object> self = v8::Object::New ();

         self->Set (
            v8::String::NewSymbol ("name"),
            v8::String::New (instance->Name.get_buffer ()));

         if (_runtime) {

            self->Set (
               v8::String::NewSymbol ("log"),
               _runtime->create_v8_log (instance->Name));
         }

         v8::Handle<v8::Value> argv[] = { self, _requireFunc };

         v8::Handle<v8::Value> value = instance->script.ctor->Call (self, 2, argv);

         if (value.IsEmpty ()) { _handle_exception (tc); }
         else {

            _log.info << "Created instance: " << instance->Name
               << " from script: " << instance->script.FileName << endl;
         }
      }
   }
}


/*
void
dmz::JsModuleV8Basic::_run_scripts (ScriptStruct *list) {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   ScriptStruct *current (list);

   while (current) {

      if (!current->script.IsEmpty ()) {

         v8::TryCatch tc;

         v8::Handle<v8::Value> value = current->script->Run ();

         if (value.IsEmpty ()) {

            _handle_exception (tc);
            current->clear ();
         }
      }

      current = current->next;
   }
}
*/


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
dmz::JsModuleV8Basic::_init (Config &local) {

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

            InstanceStruct *ptr = new InstanceStruct (UniqueName, *ss);

            if (!_instanceTable.store (UniqueName, ptr)) {

               delete ptr; ptr = 0;
               _log.error << "Script instance name: " << UniqueName << " is not unique."
                  << endl;
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

   return new dmz::JsModuleV8Basic (Info, local);
}

};
