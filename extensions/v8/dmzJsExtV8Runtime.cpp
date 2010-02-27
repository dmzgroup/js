#include "dmzJsExtV8Runtime.h"
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsExtV8Runtime::JsExtV8Runtime (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0) {

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

   }
   else if (Mode == PluginDiscoverRemove) {

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
dmz::JsExtV8Runtime::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

   }
   else if (State == JsExtV8::Init) {

   }
}


// JsExtV8Runtime Interface
void
dmz::JsExtV8Runtime::_clear () {

}


void
dmz::JsExtV8Runtime::_reset () {

}


void
dmz::JsExtV8Runtime::_init (Config &local) {

   v8::HandleScope scope;

   _dataTemplate = V8ObjectTemplatePersist::New (v8::ObjectTemplate::New ());
   _dataTemplate->SetInternalFieldCount (1);
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
