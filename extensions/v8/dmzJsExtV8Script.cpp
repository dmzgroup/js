#include "dmzJsExtV8Script.h"
#include <dmzRuntimeConfig.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzSystemFile.h>

dmz::JsExtV8Script::JsExtV8Script (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      JsObserver (Info),
      _log (Info),
      _js (0),
      _core (0) {

   _init (local);
}


dmz::JsExtV8Script::~JsExtV8Script () {

}


// Plugin Interface
void
dmz::JsExtV8Script::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

   }
}


void
dmz::JsExtV8Script::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_js) { _js = JsModule::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_js && (_js == JsModule::cast (PluginPtr))) { _js = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Script::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Script::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Script::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/components/script",
            _scriptApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _scriptStr = V8StringPersist::New (v8::String::NewSymbol ("script"));
      _instanceStr = V8StringPersist::New (v8::String::NewSymbol ("instance"));
      _errorStr = V8StringPersist::New (v8::String::NewSymbol ("error"));
      _stackStr = V8StringPersist::New (v8::String::NewSymbol ("stack"));
   }
   else if (State == JsExtV8::Stop) {

      if (_js) {

         HandleContainerIterator it;
         Handle script (0);

         while (_scripts.get_next (it, script)) { _js->destroy_script (script); }
      }
   }
   else if (State == JsExtV8::Shutdown) {

      _lastError.clear ();

      _scriptStr.Dispose (); _scriptStr.Clear ();
      _instanceStr.Dispose (); _instanceStr.Clear ();
      _errorStr.Dispose (); _errorStr.Clear ();
      _stackStr.Dispose (); _stackStr.Clear ();

      _scriptApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Script::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

}


// JsObserver Interface
void
dmz::JsExtV8Script::update_js_script (
      const JsObserverModeEnum Mode,
      const Handle Module,
      const Handle Script) {

}


void
dmz::JsExtV8Script::update_js_instance (
      const JsObserverModeEnum Mode,
      const Handle Module,
      const Handle Instance) {

}


void
dmz::JsExtV8Script::update_js_error (
      const Handle Module,
      const Handle Script,
      const Handle Instance,
      const String &ErrorMessage,
      const String &StackTrace) {

   if (_js && (_js->get_js_module_handle () == Module)) {

      _lastError.script = Script;
      _lastError.instance = Instance;
      _lastError.error = ErrorMessage;
      _lastError.stack = StackTrace;
   }
}


// JsExtV8Script Interface
// API Bindings
dmz::V8Value
dmz::JsExtV8Script::_script_error (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Script *self = _to_self (Args);

   if (self) {

      V8Object obj = v8::Object::New ();

      obj->Set (self->_scriptStr, v8::Integer::NewFromUnsigned (self->_lastError.script));
      obj->Set (
         self->_instanceStr,
         v8::Integer::NewFromUnsigned (self->_lastError.instance));
      obj->Set (
         self->_errorStr,
         v8::String::New (
            self->_lastError.error ? self->_lastError.error.get_buffer () : "<Unknown>"));
      obj->Set (
         self->_stackStr,
         v8::String::New (
            self->_lastError.stack ? self->_lastError.stack.get_buffer () : "<Unknown>"));

      result = obj;
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Script::_script_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Script *self = _to_self (Args);

   if (self && self->_js) {

      const String Name = v8_to_string (Args[0]);
      String fileName;
      if (Args.Length () > 1) { fileName = v8_to_string (Args[1]); }

      if (fileName) { fileName = self->_js->find_script (fileName); }
      else { fileName = self->_js->find_script (Name); }

      if (fileName) {

         FILE *file = open_file (fileName, "rb");

         if (file) {

            const Int32 Size = get_file_size (fileName);

            if (Size > 0) {

               char *buffer = new char[Size];

               if (buffer) {

                  if (read_file (file, Size, buffer) == Size) {

                     const Handle ScriptHandle =
                        self->_js->compile_script (Name, fileName, buffer, Size);

                     if (ScriptHandle) {

                        result = v8::Integer::NewFromUnsigned (ScriptHandle);

                        self->_scripts.add (ScriptHandle);
                     }
                  }

                  delete []buffer; buffer = 0;
               }
            }
            else {

               self->_lastError.clear ();
               self->_lastError.error << "Script: " << fileName << " is empty.";
            }

            close_file (file); file = 0;
         }
         else {

            self->_lastError.clear ();
            self->_lastError.error << "Unable to open script: " << fileName;
         }
      }
      else {

         self->_lastError.clear ();
         self->_lastError.error << "Unable to find script: " << Name;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Script::_script_reload (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Script *self = _to_self (Args);

   if (self && self->_js) {

      const Handle ScriptHandle = v8_to_handle (Args[0]);

      String fileName = self->_js->lookup_script_file_name (ScriptHandle);

      if (fileName) {

         FILE *file = open_file (fileName, "rb");

         if (file) {

            const Int32 Size = get_file_size (fileName);

            if (Size > 0) {

               char *buffer = new char[Size];

               if (buffer) {

                  if (read_file (file, Size, buffer) == Size) {

                     if (self->_js->recompile_script (ScriptHandle, buffer, Size)) {

                        result = v8::Boolean::New (true);
                     }
                  }

                  delete []buffer; buffer = 0;
               }
            }
            else {

               self->_lastError.clear ();
               self->_lastError.error << "Script: " << fileName << " is empty.";
            }

            close_file (file); file = 0;
         }
         else {

            self->_lastError.clear ();
            self->_lastError.error << "Unable to open script: " << fileName;
         }
      }
      else {

         self->_lastError.clear ();
         self->_lastError.error << "Unable to find script: " << ScriptHandle;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Script::_script_compile (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Script *self = _to_self (Args);

   if (self && self->_js) {

      Handle scriptHandle (0);
      String name, fileName;

      V8Value value = Args[0];

      if (value->IsString ()) { fileName = v8_to_string (value); }
      else {

         scriptHandle = v8_to_handle (Args[0]);

         if (scriptHandle) {

            String fileName = self->_js->lookup_script_file_name (scriptHandle);
         }
      }

      String out;
      const char *Buffer (0);
      Int32 size (0);

      V8String str = Args[1]->ToString ();

      if (v8_is_valid (str)) {

         if (str->IsExternalAscii ()) {

            v8::String::ExternalAsciiStringResource *rc =
               str->GetExternalAsciiStringResource ();

            if (rc) {

               Buffer = rc->data ();
               size = rc->length ();
            }
         }
         else {

            out = v8_to_string (str);

            Buffer = out.get_buffer ();
            size = out.get_length ();
         }
      }

      if (Buffer && size) {

         if (scriptHandle) {

            if (self->_js->recompile_script (scriptHandle, Buffer, size)) {

               result = value;
            }
         }
         else if (fileName) {

            String path, name, ext;

            split_path_file_ext (fileName, path, name, ext);

            if (!name) { name = fileName; }

            const Handle ScriptHandle =
               self->_js->compile_script (name, fileName, Buffer, size);

            if (ScriptHandle) {

               result = v8::Integer::NewFromUnsigned (ScriptHandle);

               self->_scripts.add (ScriptHandle);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Script::_script_instance (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Script *self = _to_self (Args);

   if (self && self->_js) {

      const Handle ScriptHandle = v8_to_handle (Args[0]);
      String name;

      if (Args.Length () > 1) { name = v8_to_string (Args[1]); }
      else { name = self->_js->lookup_script_name (ScriptHandle); }

      if (ScriptHandle && name) {

         Config data;
         const Handle Instance = self->_js->create_instance (name, ScriptHandle, data);

         if (Instance) { result = v8::Integer::NewFromUnsigned (Instance); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Script::_script_destroy (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Script *self = _to_self (Args);

   if (self && self->_js) {

      const Handle TheHandle = v8_to_handle (Args[0]);

      if (TheHandle) {

         if (self->_js->lookup_instance_script (TheHandle)) {

            if (self->_js->destroy_instance (TheHandle)) {

               result = v8::Boolean::New (true);
            }
         }
         else if (self->_js->destroy_script (TheHandle)) {

            result = v8::Boolean::New (true);
            self->_scripts.remove (TheHandle);
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsExtV8Script::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _scriptApi.add_function ("error", _script_error, _self);
   _scriptApi.add_function ("load", _script_load, _self);
   _scriptApi.add_function ("reload", _script_reload, _self);
   _scriptApi.add_function ("compile", _script_compile, _self);
   _scriptApi.add_function ("instance", _script_instance, _self);
   _scriptApi.add_function ("destroy", _script_destroy, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Script (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Script (Info, local);
}

};
