#include "dmzJsExtV8ConfigIO.h"
#include <dmzFoundationConfigFileIO.h>
#include <dmzFoundationConsts.h>
#include <dmzJsModuleV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

namespace {

const char LocalAuto[] = "Auto";
const char LocalXML[] = "XML";
const char LocalJSON[] = "JSON";
const char LocalRaw[] = "NoFormatting";
const char LocalStrip[] = "StripGlobal";
const char LocalFile[] = "file";
const char LocalFiles[] = "files";
const char LocalArchive[] = "archive";
const char LocalData[] = "data";
const char LocalType[] = "type";
const char LocalMode[] = "mode";
const char LocalLog[] = "log";

static bool
local_is_valid (dmz::V8Value &value) {

   return ((value.IsEmpty () == false) && (!value->IsUndefined ()));
}

};

dmz::JsExtV8ConfigIO::JsExtV8ConfigIO (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _runtime (0),
      _types (0) {

   _init (local);
}


dmz::JsExtV8ConfigIO::~JsExtV8ConfigIO () {

}


// Plugin Interface
void
dmz::JsExtV8ConfigIO::update_plugin_state (
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
dmz::JsExtV8ConfigIO::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
      if (!_types) { _types = JsModuleTypesV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8ConfigIO::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8ConfigIO::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8ConfigIO::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/runtime/configIO",
            _ioApi.get_new_instance ());
      }

      _fileStr = V8StringPersist::New (v8::String::NewSymbol (LocalFile));
      _fileListStr = V8StringPersist::New (v8::String::NewSymbol (LocalFiles));
      _archiveStr = V8StringPersist::New (v8::String::NewSymbol (LocalArchive));
      _dataStr = V8StringPersist::New (v8::String::NewSymbol (LocalData));
      _typeStr = V8StringPersist::New (v8::String::NewSymbol (LocalType));
      _modeStr = V8StringPersist::New (v8::String::NewSymbol (LocalMode));
      _logStr = V8StringPersist::New (v8::String::NewSymbol (LocalLog));
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _fileStr.Dispose (); _fileStr.Clear ();
      _fileListStr.Dispose (); _fileListStr.Clear ();
      _archiveStr.Dispose (); _archiveStr.Clear ();
      _dataStr.Dispose (); _dataStr.Clear ();
      _typeStr.Dispose (); _typeStr.Clear ();
      _modeStr.Dispose (); _modeStr.Clear ();
      _logStr.Dispose (); _logStr.Clear ();

      _ioApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8ConfigIO::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {;}


// JsExtV8ConfigIO Interface
dmz::V8Value
dmz::JsExtV8ConfigIO::_io_read (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8ConfigIO *self = _to_self (Args);

   if (self && self->_runtime) {

      ArgStruct args = self->_to_args (Args[0]);

      Config data ("global");

      if (args.data) { data = args.data; }

      if (read_config_files (args.archive, args.files, data, args.type, args.log)) {

         result = self->_runtime->create_v8_config (&data);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8ConfigIO::_io_write (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8ConfigIO *self = _to_self (Args);

   if (self) {

      ArgStruct args = self->_to_args (Args[0]);

      String file;
      args.files.get_first (file);

      if (write_config_file (
               args.archive,
               file,
               args.data,
               args.mode,
               args.type,
               args.log)) { result = v8::Boolean::New (true); }
   }

   return scope.Close (result);
}


dmz::JsExtV8ConfigIO::ArgStruct
dmz::JsExtV8ConfigIO::_to_args (V8Value value) {

   v8::HandleScope scope;

   ArgStruct result;

   if ((value.IsEmpty () == false) && value->IsObject ()) {

      V8Object obj = value->ToObject ();

      if (obj.IsEmpty () == false) {

         Boolean prettyPrint = True;

         V8Value file = obj->Get (_fileStr);

         if (local_is_valid (file) && _types) {

            _types->to_dmz_string_container (file, result.files);
         }

         V8Value fileList = obj->Get (_fileListStr);

         if (local_is_valid (fileList) && _types) {

            _types->to_dmz_string_container (fileList, result.files);
         }

         V8Value archive = obj->Get (_archiveStr);

         if (local_is_valid (archive)) {

            result.archive = v8_to_string (archive);
         }

         V8Value data = obj->Get (_dataStr);

         if (local_is_valid (data) && _runtime) {

            _runtime->to_dmz_config (data, result.data);
         }

         V8Value type = obj->Get (_typeStr);

         if (local_is_valid (type)) {

            const String TypeStr = v8_to_string (type);

            if (TypeStr == LocalAuto) { result.type = FileTypeAutoDetect; }
            else if (TypeStr == LocalXML) { result.type = FileTypeXML; }
            else if (TypeStr == LocalJSON) { result.type = FileTypeJSON; }
            else {

               _log.error << "Unknown file type: " << TypeStr
                  << " defaulting to auto-detect." << endl;

               result.type = FileTypeAutoDetect;
            }
         }
         else { result.type = FileTypeAutoDetect; }

         V8Value mode = obj->Get (_modeStr);

         if (local_is_valid (mode) && _types) {

            StringContainer modeList;

            _types->to_dmz_string_container (mode, modeList);

            StringContainerIterator it;
            String value;

            while (modeList.get_next (it, value)) {

               if (value == LocalRaw) { prettyPrint = False; }
               else if (value == LocalStrip) { result.mode |= ConfigStripGlobal; }
               else { _log.warn << "Unknown mode flag: " << value << endl; }
            }
         }

         if (prettyPrint) { result.mode |= ConfigPrettyPrint; }

         V8Value log = obj->Get (_logStr);

         if (local_is_valid (log) && _runtime) {

            result.log = _runtime->to_dmz_log (log);
         }
      }
   }

   return result;
}


void
dmz::JsExtV8ConfigIO::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _ioApi.add_constant ("Auto", LocalAuto);
   _ioApi.add_constant ("XML", LocalXML);
   _ioApi.add_constant ("JSON", LocalJSON);
   _ioApi.add_constant ("NoFormatting", LocalRaw);
   _ioApi.add_constant ("StripGlobal", LocalStrip);
   _ioApi.add_function ("read", _io_read, _self);
   _ioApi.add_function ("write", _io_write, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8ConfigIO (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8ConfigIO (Info, local);
}

};
