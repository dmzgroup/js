#ifndef DMZ_JS_MODULE_V8_BASE_DOT_H
#define DMZ_JS_MODULE_V8_BASE_DOT_H

#include <dmzJsModuleV8.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class JsModuleV8Base :
         public Plugin,
         public JsModuleV8 {

      public:
         JsModuleV8Base (const PluginInfo &Info, Config &local);
         ~JsModuleV8Base ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsModuleV8 Interface

      protected:
         void _init (Config &local);

         Log _log;

         v8::Persistent<v8::Context> _context;

      private:
         JsModuleV8Base ();
         JsModuleV8Base (const JsModuleV8Base &);
         JsModuleV8Base &operator= (const JsModuleV8Base &);

   };
};

#endif // DMZ_JS_MODULE_V8_BASE_DOT_H
