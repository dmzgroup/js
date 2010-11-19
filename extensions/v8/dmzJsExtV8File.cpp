#include "dmzJsExtV8File.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilStrings.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzSystemFile.h>

#include <stdio.h>

dmz::JsExtV8File::JsExtV8File (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0) {

   _init (local);
}


dmz::JsExtV8File::~JsExtV8File () {

}


// Plugin Interface
void
dmz::JsExtV8File::update_plugin_state (
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
dmz::JsExtV8File::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// JsExtV8 Interface
void
dmz::JsExtV8File::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8File::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8File::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/system/file",
            _fileApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _fileApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8File::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {;}


// JsExtV8File Interface
dmz::V8Value
dmz::JsExtV8File::_file_split (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (Args.Length () > 0) {

      if (Args[0]->IsString ()) {

         const String FilePath = v8_to_string (Args[0]);

         String path, file, ext;
         split_path_file_ext (FilePath, path, file, ext);

         V8Object obj = v8::Object::New ();

         if (obj.IsEmpty () == false) {

            if (path) {

               obj->Set (
                  v8::String::NewSymbol("path"),
                  v8::String::New (path.get_buffer ()));
            }

            if (file) {

               obj->Set (
                  v8::String::NewSymbol("file"),
                  v8::String::New (file.get_buffer ()));
            }

            if (ext) {

               obj->Set (
                  v8::String::NewSymbol("ext"),
                  v8::String::New (ext.get_buffer ()));
            }

            result = obj;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8File::_file_valid (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (Args.Length () > 0) {

      if (Args[0]->IsString ()) {

         const String FilePath = v8_to_string (Args[0]);

         result = v8::Boolean::New (is_valid_path (v8_to_string (Args[0])));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8File::_file_read (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (Args.Length () > 0) {

      if (Args[0]->IsString ()) {

         const String FilePath = v8_to_string (Args[0]);

         FILE *file = open_file (FilePath, "r");

         if (file) {

            const int Size = get_file_size (FilePath);

            if (Size > 0) {

               char *buffer = new char[Size];

               if (buffer) {

                  if (read_file (file, Size, buffer) == Size) {

                     V8EmbeddedBuffer *estr = new V8EmbeddedBuffer (buffer, Size);

                     if (estr) { result = v8::String::NewExternal (estr); }
                     else { delete []buffer; buffer = 0; }
                  }
                  else { delete []buffer; buffer = 0; }
               }
            }
            else {

               result = v8::String::New ("");
            }

            close_file (file); file = 0;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8File::_file_write (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   const String FilePath = v8_to_string (Args[0]);
   const String Data = v8_to_string (Args[1]);

   if (FilePath) {

      FILE *file = open_file (FilePath, "w");

      if (file) {

         const size_t Size = Data.get_length ();

         if (fwrite (Data.get_buffer (), sizeof (char), Size, file) == Size) {

            result = v8::Integer::New (Size);
         }

         close_file (file); file = 0;
      }
   }

   return scope.Close (result);
}


void
dmz::JsExtV8File::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _fileApi.add_function ("split", _file_split, _self);
   _fileApi.add_function ("valid", _file_valid, _self);
   _fileApi.add_function ("read", _file_read, _self);
   _fileApi.add_function ("write", _file_write, _self);

}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8File (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8File (Info, local);
}

};
