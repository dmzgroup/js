#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

#include <qdb.h>
static dmz::qdb out;

dmz::JsModuleRuntimeV8Basic::JsModuleRuntimeV8Basic (
      const PluginInfo &Info,
      Config &local) :
      Plugin (Info),
      JsModuleRuntimeV8 (Info),
      JsExtV8 (Info),
      _log (Info),
      _defs (Info, &_log),
      _core (0),
      _types (0) {

   _init (local);
}


dmz::JsModuleRuntimeV8Basic::~JsModuleRuntimeV8Basic () {

}


// Plugin Interface
void
dmz::JsModuleRuntimeV8Basic::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

      _data.Dispose (); _data.Clear ();
   }
}


void
dmz::JsModuleRuntimeV8Basic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_types) {

         _types = JsModuleTypesV8::cast (PluginPtr);
      }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) {

         _types = 0;
      }
   }
}


// JsExtV8 Interface
void
dmz::JsModuleRuntimeV8Basic::update_js_module_v8 (
      const ModeEnum Mode,
      JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { 

      if (!_core) { _core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {

      if (_core == &module) { _core = 0; }
   }
}


void
dmz::JsModuleRuntimeV8Basic::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsModuleRuntimeV8Basic::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      _logFunc.Dispose ();

      if (_logFuncTemplate.IsEmpty () == false) {

         _logFunc = V8FunctionPersist::New (_logFuncTemplate->GetFunction ());
      }

      if (_core) {

         _core->register_interface (
            "dmz/runtime/definitions",
            _defsApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      _v8Context.Clear ();
   }
}


// JsModuleRuntimeV8Basic Interface
void
dmz::JsModuleRuntimeV8Basic::_clear () {

}


void
dmz::JsModuleRuntimeV8Basic::_reset () {

}


void
dmz::JsModuleRuntimeV8Basic::_init (Config &local) {

   v8::HandleScope scope;

   _data = V8ValuePersist::New (v8::External::Wrap (this));

   _init_log ();
   _init_definitions ();
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleRuntimeV8Basic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleRuntimeV8Basic (Info, local);
}

};
