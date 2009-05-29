#ifndef DMZ_JS_MODULE_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_V8_BASIC_DOT_H

#include <dmzJsModuleV8.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeResources.h>
#include <dmzSystemFile.h>

namespace dmz {

   class JsModuleV8Basic :
         public Plugin,
         public JsModuleV8 {

      public:
         JsModuleV8Basic (const PluginInfo &Info, Config &local);
         ~JsModuleV8Basic ();

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
         StreamLog _out;
         Resources _rc;

         PathContainer _localPaths;

         v8::Persistent<v8::Context> _context;

      private:
         JsModuleV8Basic ();
         JsModuleV8Basic (const JsModuleV8Basic &);
         JsModuleV8Basic &operator= (const JsModuleV8Basic &);

   };
};

#endif // DMZ_JS_MODULE_V8_BASIC_DOT_H
