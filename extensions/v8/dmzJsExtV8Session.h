#ifndef DMZ_JS_EXT_V8_SESSION_DOT_H
#define DMZ_JS_EXT_V8_SESSION_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class JsModuleRuntimeV8;

   class JsExtV8Session :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8Session (const PluginInfo &Info, Config &local);
         ~JsExtV8Session ();

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
         // JsExtV8Session Interface
         static JsExtV8Session *_to_self (const v8::Arguments &Args);
         static V8Value _session_get (const v8::Arguments &Args);
         static V8Value _session_set (const v8::Arguments &Args);
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;
         JsModuleRuntimeV8 *_runtime;

         V8InterfaceHelper _sessionApi;

         v8::Handle<v8::Context> _v8Context;

         V8ValuePersist _self;

      private:
         JsExtV8Session ();
         JsExtV8Session (const JsExtV8Session &);
         JsExtV8Session &operator= (const JsExtV8Session &);

   };
};


inline dmz::JsExtV8Session *
dmz::JsExtV8Session::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Session *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_SESSION_DOT_H
