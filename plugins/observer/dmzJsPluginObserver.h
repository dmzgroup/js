#ifndef DMZ_JS_PLUGIN_OBSERVER_DOT_H
#define DMZ_JS_PLUGIN_OBSERVER_DOT_H

#include <dmzJsModule.h>
#include <dmzJsObserver.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class JsPluginObserver :
         public Plugin,
         public JsObserver {

      public:
         JsPluginObserver (const PluginInfo &Info, Config &local);
         ~JsPluginObserver ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

      // JsObserver Interface
      virtual void update_js_script (
         const JsObserverModeEnum Mode,
         const Handle Module,
         const Handle Script);

      virtual void update_js_instance (
         const JsObserverModeEnum Mode,
         const Handle Module,
         const Handle Instance);

      virtual void update_js_error (
         const Handle Module,
         const Handle Script,
         const Handle Instance,
         const String &ErrorMessage,
         const String &StackTrace);

      protected:
         // JsPluginObserver Interface
         String _script_name (const Handle Script);
         String _instance_name (const Handle Instance);
         String _mode (const JsObserverModeEnum Mode);
         void _init (Config &local);

         Log _log;

         JsModule *_core;

      private:
         JsPluginObserver ();
         JsPluginObserver (const JsPluginObserver &);
         JsPluginObserver &operator= (const JsPluginObserver &);

   };
};

#endif // DMZ_JS_PLUGIN_OBSERVER_DOT_H
