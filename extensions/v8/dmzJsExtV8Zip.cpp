#include <dmzFoundationConsts.h>
#include <dmzFoundationConfigFileIO.h>
#include <dmzFoundationReaderWriterFile.h>
#include <dmzFoundationReaderWriterZip.h>
#include <dmzFoundationStreamZip.h>
#include <dmzFoundationXMLUtil.h>
#include "dmzJsExtV8Zip.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilStrings.h>
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
      _dataStr = V8StringPersist::New (v8::String::NewSymbol ("data"));
      _configStr = V8StringPersist::New (v8::String::NewSymbol ("config"));
   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _nameStr.Dispose (); _nameStr.Clear ();
      _fileStr.Dispose (); _fileStr.Clear ();
      _dataStr.Dispose (); _dataStr.Clear ();
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

// JsExtV8Zip Interface
dmz::V8Value
dmz::JsExtV8Zip::_zip_manifest (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Zip *self = _to_self (Args);

   if (self && self->_runtime) {

      Config manifest ("manifest");

      const int Length = Args.Length ();

      for (int ix = 0; ix < Length; ix++) {

         Config config ("config");
         config.store_attribute ("file", v8_to_string (Args[ix]));
         manifest.add_config (config);
      }

      result = self->_runtime->create_v8_config (&manifest);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Zip::_zip_read (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Zip *self = _to_self (Args);

   if (self && (Args.Length () >= 2) && Args[0]->IsString ()) {

      const String FileName = v8_to_string (Args[0]);
      const String Name = v8_to_string (Args[1]);

      ReaderZip in;

      if (in.open_zip_file (FileName)) {

         if (in.open_file (Name)) {

            const Int32 Size = (Int32)in.get_file_size ();

            if (Size > 0) {

               char *buffer = new char[Size];

               if (buffer && (in.read_file (buffer, Size) == Size)) {

                  V8EmbeddedBuffer *estr = new V8EmbeddedBuffer (buffer, Size);

                  if (estr) { result = v8::String::NewExternal (estr); }
                  else { delete []buffer; buffer = 0; }
               }
               else if (buffer) { delete []buffer; buffer = 0; }
            }
            else {

               result = v8::String::New ("");

               self->_log.warn << "File: " << Name << " in archive: "
                  << FileName << " is empty." << endl;
            }
         }
         else {

            self->_log.error << "Failed to open file: " << Name << " in archive: "
               << FileName << endl;
         }
      }
      else { self->_log.error << "Failed to open archive: " << FileName << endl; }
   }

   return scope.Close (result);
}


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

               ListStruct *ls = self->_create_list_struct (
                  list->Get (v8::Integer::New (ix)));

               if (ls) {

                  if (current) { current->next = ls; current = ls; }
                  else { current = files = ls; }
               }
               else { error = True; }
            }
         }
         else {

            files = self->_create_list_struct (value);

            if (!files) { error = True; }
         }

         StreamZip stream;

         if (!error && files && stream.open_zip_file (OutFileName)) {

            current = files;

            while (current) {

               if (stream.open_file (current->Name)) {

                  if (current->config) {

                     format_config_to_xml (
                        current->config,
                        stream,
                        ConfigPrettyPrint,
                        &(self->_log));
                  }
                  else if (v8_is_valid (current->data)) {

                     String tmp;
                     const char *Buffer (0);
                     int size (0);

                     if (current->data->IsExternalAscii ()) {

                        v8::String::ExternalAsciiStringResource *rc =
                           current->data->GetExternalAsciiStringResource ();

                        if (rc) {

                           Buffer = rc->data ();
                           size = rc->length ();
                        }
                     }
                     else {

                        tmp = v8_to_string (current->data);

                        Buffer = tmp.get_buffer ();
                        size = tmp.get_length ();
                     }

                     if (Buffer && size) {

                        if (stream.write_file (Buffer, size) == False) {

                           error = True;
                        }
                     }
                  }
                  else if (current->File) {

                     ReaderFile in;

                     if (in.open_file (current->File)) {

                        const Int32 Size = (Int32)in.get_file_size ();

                        if (Size > 0) {

                           char *buffer = new char[Size];

                           if (buffer) {

                              if (in.read_file (buffer, Size) == Size) {

                                 if (stream.write_file (buffer, Size) == False) {

                                    error = True;
                                 }
                              }

                              delete []buffer;
                           }
                        }
                     }
                  }

                  stream.close_file ();
               }

               current = current->next;
            }

            if (!error) { result = v8::Boolean::New (true); }
         }

         delete_list (files);
      }
   }

   return scope.Close (result);
}


dmz::JsExtV8Zip::ListStruct *
dmz::JsExtV8Zip::_create_list_struct (V8Value value) {

   v8::HandleScope scope;

   ListStruct *result (0);

   String name;
   String file;
   V8String dataStr;
   Config config;

   if (v8_is_valid (value)) {

      if (value->IsString ()) {

         file = v8_to_string (value);

         String path, root, ext;
         split_path_file_ext (name, path, root, ext);

         name = root + ext;
      }
      else if (value->IsObject ()) {

         V8Object obj = v8_to_object (value);

         if (v8_is_valid (obj)) {

            name = v8_to_string (obj->Get (_nameStr));
            file = v8_to_string (obj->Get (_fileStr));

            if (!file) {

               String path, root, ext;
               split_path_file_ext (name, path, root, ext);

               file = name;
               name = root + ext;
            }

            V8Value dataValue = obj->Get (_dataStr);

            if (v8_is_valid (dataValue) && dataValue->IsString ()) {

               dataStr = dataValue->ToString ();

               file.flush ();
            }
            else if (_runtime) {

               if (_runtime->to_dmz_config (obj->Get (_configStr), config)) {

                  file.flush ();
               }
            }
         }
      }

      if (name) {

         if ((file && is_valid_path (file) && (!is_directory (file))) || config) {

            result = new ListStruct (name, file);

            if (result) {

               if (v8_is_valid (dataStr)) {

                  result->data = V8StringPersist::New (dataStr);
               }
               else { result->config = config; }
            }
         }
      }
   }
 
   return result;
}


void
dmz::JsExtV8Zip::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _zipApi.add_constant ("ManifestFileName", ArchiveManifestFileName);
   _zipApi.add_function ("manifest", _zip_manifest, _self);
   _zipApi.add_function ("read", _zip_read, _self);
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
