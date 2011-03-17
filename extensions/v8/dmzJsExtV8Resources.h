#ifndef DMZ_JS_EXT_V8_RESOURCES_DOT_H
#define DMZ_JS_EXT_V8_RESOURCES_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class JsModuleRuntimeV8;

   class JsExtV8Resources :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8Resources (const PluginInfo &Info, Config &local);
         ~JsExtV8Resources ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

      protected:
         // JsExtV8Resources Interface
         static JsExtV8Resources *_to_self (const v8::Arguments &Args);
         static V8Value _rc_find_file (const v8::Arguments &Args);
         static V8Value _rc_store_config (const v8::Arguments &Args);
         static V8Value _rc_lookup_config (const v8::Arguments &Args);
         static V8Value _rc_remove_config (const v8::Arguments &Args);
         void _init (Config &local);

         Log _log;
         JsModuleV8 *_core;
         JsModuleRuntimeV8 *_runtime;
         V8InterfaceHelper _resourcesApi;
         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

      private:
         JsExtV8Resources ();
         JsExtV8Resources (const JsExtV8Resources &);
         JsExtV8Resources &operator= (const JsExtV8Resources &);
   };
};


inline dmz::JsExtV8Resources *
dmz::JsExtV8Resources::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Resources *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_RESOURCES_DOT_H
