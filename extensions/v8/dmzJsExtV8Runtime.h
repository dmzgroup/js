#ifndef DMZ_JS_EXT_V8_RUNTIME_DOT_H
#define DMZ_JS_EXT_V8_RUNTIME_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

#include <v8.h>

namespace dmz {

   class JsExtV8Runtime :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8Runtime (const PluginInfo &Info, Config &local);
         ~JsExtV8Runtime ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_ext_v8_state (const StateEnum State);

      protected:
         void _clear ();
         void _reset ();
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;

         V8ObjectTemplatePersist _dataTemplate;
         V8ObjectPersist _data;

      private:
         JsExtV8Runtime ();
         JsExtV8Runtime (const JsExtV8Runtime &);
         JsExtV8Runtime &operator= (const JsExtV8Runtime &);

   };
};

#endif // DMZ_JS_EXT_V8_RUNTIME_DOT_H
