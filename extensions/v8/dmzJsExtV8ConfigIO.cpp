#include "dmzJsExtV8ConfigIO.h"
#include <dmzFoundationConfigFileIO.h>
#include <dmzFoundationConsts.h>
#include <dmzJsModuleV8.h>
#include <dmzJsModuleRuntimeV8.h>
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

};

dmz::JsExtV8ConfigIO::JsExtV8ConfigIO (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _runtime (0) {

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
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
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
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

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

      const int Length = Args.Length ();

      String file = Length > 1 ? v8_to_string (Args[0]) : String (""); 
      StringContainer list;
      Log *log = self->_runtime->to_dmz_log (Args[2]); 

      V8Value value = Length > 1 ?  Args[1] : Args[0];

      if (value.IsEmpty () == false) {

         if (value->IsArray ()) {

            V8Array array = v8_to_array (value);

            if (array.IsEmpty () == false) {

               const int Size = array->Length ();

               for (int ix = 0; ix < Size; ix++) {

                  list.add (v8_to_string (array->Get (v8::Integer::New (ix))));
               }
            }
         }
         else if (value->IsString ()) { list.add (v8_to_string (value)); }
      }

      UInt32 type = FileTypeAutoDetect;

      Config data ("global");

      if (read_config_files (file, list, data, type, log)) {

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

   if (self && self->_runtime) {

      Config data;
      self->_runtime->to_dmz_config (Args[0], data);
      const String File = v8_to_string (Args[1]); 
      const String Name = v8_to_string (Args[2]); 
      V8Array opts = v8_to_array (Args[3]); 
      Log *log = self->_runtime->to_dmz_log (Args[4]);

      UInt32 type = FileTypeAutoDetect;
      UInt32 mode = 0;

      if (opts.IsEmpty () == false) {

         Boolean prettyPrint = True;

         const int Length = opts->Length ();

         for (int ix = 0; ix < Length; ix++) {

            V8Value value = opts->Get (v8::Integer::New (ix));

            if ((value.IsEmpty () == false) && value->IsString ()) {

               const String Str = v8_to_string (value);

               if (Str == LocalAuto) { type = FileTypeAutoDetect; }
               else if (Str == LocalXML) { type = FileTypeXML; }
               else if (Str == LocalJSON) { type = FileTypeJSON; }
               else if (Str == LocalRaw) { prettyPrint = False; }
               else if (Str == LocalStrip) { mode |= ConfigStripGlobal; }
            }
         }

         if (prettyPrint) { mode |= ConfigPrettyPrint; }
      }
      else { mode = ConfigPrettyPrint; }

      if (write_config_file (
            (Name ? File : String ("")),
            (Name ? Name : File),
            data,
            mode,
            type,
            log)) { result = v8::Boolean::New (true); }
   }

   return scope.Close (result);
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
