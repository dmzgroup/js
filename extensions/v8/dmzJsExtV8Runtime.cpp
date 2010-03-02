#include "dmzJsExtV8Runtime.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

#include <qdb.h>
static dmz::qdb out;

using namespace dmz;

namespace {

enum RuntimeDataOffsetEnum {
   TypesData = 0,
   DefinitionsData,
   MaxRuntimeData,
};


// Definitions bindings
V8Value
local_create_named_handle (const v8::Arguments &args) {

   v8::HandleScope scope;
   V8Value result;

   Definitions *defs = v8_to_pointer<Definitions> (args.Data (), DefinitionsData);

   if (defs) {

      const String Name = v8_to_string (args[0]);

      if (Name) {
 
         Handle val = defs->create_named_handle (Name);
         result = v8::Number::New ((double)val);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
local_lookup_named_handle (const v8::Arguments &args) {

   v8::HandleScope scope;
   V8Value result;

   Definitions *defs = v8_to_pointer<Definitions> (args.Data (), DefinitionsData);

   if (defs) {

      const String Name = v8_to_string (args[0]);

      if (Name) {
 
         result = v8::Number::New ((double)defs->lookup_named_handle (Name));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
local_lookup_named_handle_name (const v8::Arguments &args) {

   v8::HandleScope scope;
   V8Value result;

   Definitions *defs = v8_to_pointer<Definitions> (args.Data (), DefinitionsData);

   if (defs) {

      const UInt32 Value = v8_to_uint32 (args[0]);

      if (Value) {
 
         const String Name = defs->lookup_named_handle_name (Value);

         if (Name) { result = v8::String::New (Name.get_buffer ()); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
local_lookup_state (const v8::Arguments &args) {

   v8::HandleScope scope;
   V8Value result;

   Definitions *defs = v8_to_pointer<Definitions> (args.Data (), DefinitionsData);
   JsModuleTypesV8 *types = v8_to_pointer<JsModuleTypesV8> (args.Data (), TypesData);

   if (types && defs) {

      const String Name = v8_to_string (args[0]);

      Mask state;

      if (defs->lookup_state (Name, state)) { result = types->to_v8_mask (state); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
local_lookup_state_name (const v8::Arguments &args) {

   v8::HandleScope scope;
   V8Value result;

   Definitions *defs = v8_to_pointer<Definitions> (args.Data (), DefinitionsData);
   JsModuleTypesV8 *types = v8_to_pointer<JsModuleTypesV8> (args.Data (), TypesData);

   if (types && defs) {

      const Mask State = types->to_dmz_mask (args[0]);
      String name;
      defs->lookup_state_name (State, name);
      if (name) { result = v8::String::New (name.get_buffer ()); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}

};

dmz::JsExtV8Runtime::JsExtV8Runtime (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _defs (Info, &_log),
      _core (0),
      _types (0) {

   _init (local);
}


dmz::JsExtV8Runtime::~JsExtV8Runtime () {

}


// Plugin Interface
void
dmz::JsExtV8Runtime::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

      _dataTemplate.Dispose (); _dataTemplate.Clear ();
   }
}


void
dmz::JsExtV8Runtime::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_types) {

         _types = JsModuleTypesV8::cast (PluginPtr);

         if (_types && !_data.IsEmpty ()) {

            v8::HandleScope scope;
            _data->SetInternalField (TypesData, v8::External::New ((void *)_types));
         }
      }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) {

         if (!_data.IsEmpty ()) {

            v8::HandleScope scope;
            _data->SetInternalField (TypesData, v8::External::New (0));
         }

         _types = 0;
      }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Runtime::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { 

      if (!_core) { _core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {

      if (_core == &module) { _core = 0; }
   }
}


void
dmz::JsExtV8Runtime::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Runtime::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      _data = V8ObjectPersist::New (_dataTemplate->NewInstance ());

      if (!_data.IsEmpty ()) {

         _data->SetInternalField (DefinitionsData, v8::External::New ((void *)&_defs));
         _data->SetInternalField (TypesData, v8::External::New ((void *)_types));
      }

      if (_core) {

         _core->register_interface ("dmz/runtime/definitions", _init_definitions ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      _v8Context.Clear ();
   }
}


// JsExtV8Runtime Interface
void
dmz::JsExtV8Runtime::_clear () {

}


void
dmz::JsExtV8Runtime::_reset () {

}


dmz::V8ObjectPersist
dmz::JsExtV8Runtime::_init_definitions () {

   v8::HandleScope scope;

   _definitionsInterface.clear ();

   _definitionsInterface.add_function (
      "createNamedHandle",
      local_create_named_handle,
      _data);

   _definitionsInterface.add_function (
      "lookupNamedHandle",
      local_lookup_named_handle,
      _data);

   _definitionsInterface.add_function (
      "lookupNamedHandleName",
      local_lookup_named_handle_name,
      _data);

   _definitionsInterface.add_function (
      "lookupState",
      local_lookup_state,
      _data);

   _definitionsInterface.add_function (
      "lookupStateName",
      local_lookup_state_name,
      _data);

   return _definitionsInterface.get_new_instance ();
}


void
dmz::JsExtV8Runtime::_init (Config &local) {

   v8::HandleScope scope;

   _dataTemplate = V8ObjectTemplatePersist::New (v8::ObjectTemplate::New ());
   _dataTemplate->SetInternalFieldCount (MaxRuntimeData);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Runtime (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Runtime (Info, local);
}

};
