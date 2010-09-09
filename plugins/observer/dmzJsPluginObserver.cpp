#include "dmzJsPluginObserver.h"
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsPluginObserver::JsPluginObserver (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsObserver (Info),
      _log (Info),
      _core (0) {

   _init (local);
}


dmz::JsPluginObserver::~JsPluginObserver () {

}


// Plugin Interface
void
dmz::JsPluginObserver::update_plugin_state (
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
dmz::JsPluginObserver::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_core) { _core = JsModule::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_core && (_core == JsModule::cast (PluginPtr))) { _core = 0; }
   }
}


// JsObserver Interface
void
dmz::JsPluginObserver::update_js_script (
         const JsObserverModeEnum Mode,
         const Handle Module,
         const Handle Script) {

   _log.error << _mode (Mode) << ": " << _script_name (Script) << endl;
}

         
void
dmz::JsPluginObserver::update_js_instance (
         const JsObserverModeEnum Mode,
         const Handle Module,
         const Handle Instance) {

   _log.error << _mode (Mode) << ": " << _instance_name (Instance) << endl;
}

         
void
dmz::JsPluginObserver::update_js_error (
         const Handle Module,
         const Handle Script,
         const Handle Instance,
         const String &ErrorMessage,
         const String &StackTrace) {

   _log.error << _script_name (Script) << ": " << _instance_name (Instance) << endl
      << ErrorMessage << endl << StackTrace << endl;
}


// JsPluginObserver Interface
dmz::String
dmz::JsPluginObserver::_script_name (const Handle Script) {

   String result ("<anonymous>");

   if (_core) {

      const String SName = _core->lookup_script_name (Script);

      if (SName) { result = SName; }
   }
   return result;
}


dmz::String
dmz::JsPluginObserver::_instance_name (const Handle Instance) {

   String result ("<anonymous>");

   if (_core) {

      const String IName = _core->lookup_instance_name (Instance);

      const Handle Script = _core->lookup_instance_script (Instance);

      if (IName) { result = IName; }

      result << ":";

      result << _script_name (Script);
   }

   return result;
}


dmz::String
dmz::JsPluginObserver::_mode (const JsObserverModeEnum Mode) {

   String result ("<anonymous>");

   if (Mode == JsObserverCreate) { result = "create"; }
   else if (Mode == JsObserverRelease) { result = "release"; }
   if (Mode == JsObserverDestroy) { result = "destroy"; }

   return result;
}


void
dmz::JsPluginObserver::_init (Config &local) {

}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsPluginObserver (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsPluginObserver (Info, local);
}

};
