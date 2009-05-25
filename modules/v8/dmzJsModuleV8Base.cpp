#include "dmzJsModuleV8Base.h"
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

namespace {

static const char LocalDMZName[] = "DMZ";

};

dmz::JsModuleV8Base::JsModuleV8Base (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsModuleV8 (Info),
      _log (Info) {

   _init (local);
}


dmz::JsModuleV8Base::~JsModuleV8Base () {

   _context.Dispose ();
}


// Plugin Interface
void
dmz::JsModuleV8Base::update_plugin_state (
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
dmz::JsModuleV8Base::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


void
dmz::JsModuleV8Base::_init (Config &local) {

   _context = v8::Context::New ();

   v8::Context::Scope scope (_context);

   v8::Handle<v8::Object> global = _context->Global ();

   if (!global.IsEmpty ()) {

      global->Set (v8::String::New (LocalDMZName), v8::Object::New ());
   }
   else { _log.error << "No global object." << endl; }
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleV8Base (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleV8Base (Info, local);
}

};
