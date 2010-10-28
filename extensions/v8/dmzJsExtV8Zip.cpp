#include <dmzFoundationReaderWriterZip.h>
#include "dmzJsExtV8Zip.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzSystemFile.h>
#include <dmzTypesDeleteListTemplate.h>

dmz::JsExtV8Zip::JsExtV8Zip (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _runtime (0) {

   _init (local);
}


dmz::JsExtV8Zip::~JsExtV8Zip () {

}


// Plugin Interface
void
dmz::JsExtV8Zip::update_plugin_state (
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
dmz::JsExtV8Zip::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Zip::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Zip::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Zip::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/system/zip",
            _zipApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _nameStr = V8StringPersist::New (v8::String::NewSymbol ("name"));
      _fileStr = V8StringPersist::New (v8::String::NewSymbol ("file"));
      _configStr = V8StringPersist::New (v8::String::NewSymbol ("config"));
   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _nameStr.Dispose (); _nameStr.Clear ();
      _fileStr.Dispose (); _fileStr.Clear ();
      _configStr.Dispose (); _configStr.Clear ();

      _zipApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Zip::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {;}

namespace {

struct ListStruct {

   const dmz::String Name;
   const dmz::String File;
   dmz::Config data;
   ListStruct *next;

   ListStruct (const dmz::String &TheName, const dmz::String &TheFile) :
         Name (TheName),
         File (TheFile),
         next (0) {;}
};

};

// JsExtV8Zip Interface
dmz::V8Value
dmz::JsExtV8Zip::_zip_write (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Zip *self = _to_self (Args);

   if (self && (Args.Length () >= 2) && Args[0]->IsString ()) {

      const String OutFileName = v8_to_string (Args[0]);
      V8Value value = Args[1];

      ListStruct *files (0);
      ListStruct *current (0);

      if (OutFileName && v8_is_valid (value)) {

         V8Array list = v8_to_array (value);

         Boolean error (False);

         if (v8_is_valid (list)) {

            const int Length = list->Length ();

            for (int ix = 0; ix < Length; ix++) {

               String name;
               String file;
               Config data;
               Boolean valid (False);

               V8Value item = list->Get(v8::Integer::New (ix));

               if (v8_is_valid (item)) {

                  if (item->IsString ()) {

                     name = file = v8_to_string (item);
                  }
                  else if (item->IsObject ()) {

                     V8Object obj = v8_to_object (item);

                     if (v8_is_valid (obj)) {

                        name = v8_to_string (obj->Get (self->_nameStr));
                        file = v8_to_string (obj->Get (self->_fileStr));

                        if (!file) {

                           String path, root, ext;
                           split_path_file_ext (name, path, root, ext);
        
                           file = name;
                           name = root + ext;
                        }

                        if (self->_runtime) {

                           if (self->_runtime->to_dmz_config (
                                 obj->Get (self->_configStr),
                                 data)) { file.flush (); }
                        }
                     }
                  }

                  if (name) {

                     if ((file && is_valid_path (file) && (!is_directory (file))) ||
                           data) {

                        ListStruct *ls = new ListStruct (name, file);

                        if (ls) {

                           valid = True;
                           ls->data = data;
                           if (current) { current->next = ls; current = ls; }
                           else { files = current = ls; }
                        }
                     }
                  }
               }

               if (!valid) { error = True; }
            }
         }
         else if (value->IsString ()) {

            const String Name = v8_to_string (value);

            if (Name && is_valid_path (Name) && !is_directory (Name)) {

               files = new ListStruct (Name, Name);
            }
         }
         else if (value->IsObject ()) {

         }
      }

      delete_list (files);
   }

   return scope.Close (result);
}


void
dmz::JsExtV8Zip::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _zipApi.add_function ("write", _zip_write, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Zip (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Zip (Info, local);
}

};
