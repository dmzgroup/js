#ifndef DMZ_JS_EXT_V8_TEST_DOT_H
#define DMZ_JS_EXT_V8_TEST_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeExit.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class JsExtV8Test :
         public Plugin,
         public JsExtV8 {

      public:
         static JsExtV8Test *to_self (const v8::Arguments &Args);

         JsExtV8Test (const PluginInfo &Info, Config &local);
         ~JsExtV8Test ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsExtV8
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

      protected:
         // JsExtV8Test Interface
         static V8Value _start_test (const v8::Arguments &Args);
         static V8Value _validate_test (const v8::Arguments &Args);
         static V8Value _stop_test (const v8::Arguments &Args);

         void _init (Config &local);

         Log _testLog;
         Exit _exit;
         Boolean _failed;

         JsModuleV8 *_core;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;
         V8InterfaceHelper _testApi;

      private:
         JsExtV8Test ();
         JsExtV8Test (const JsExtV8Test &);
         JsExtV8Test &operator= (const JsExtV8Test &);

   };
};


inline dmz::JsExtV8Test *
dmz::JsExtV8Test::to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Test *)v8::External::Unwrap (Args.Data ());
}


#endif // DMZ_JS_EXT_V8_TEST_DOT_H
