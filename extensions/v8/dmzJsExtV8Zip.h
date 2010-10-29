#ifndef DMZ_JS_EXT_V8_ZIP_DOT_H
#define DMZ_JS_EXT_V8_ZIP_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class JsModuleRuntimeV8;

   class JsExtV8Zip :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8Zip (const PluginInfo &Info, Config &local);
         ~JsExtV8Zip ();

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
         // JsExtV8Zip Interface
         static JsExtV8Zip *_to_self (const v8::Arguments &Args);
         static V8Value _zip_read (const v8::Arguments &Args);
         static V8Value _zip_write (const v8::Arguments &Args);
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;
         JsModuleRuntimeV8 *_runtime;

         V8InterfaceHelper _zipApi;

         v8::Handle<v8::Context> _v8Context;

         V8StringPersist _nameStr;
         V8StringPersist _fileStr;
         V8StringPersist _configStr;

         V8ValuePersist _self;

      private:
         JsExtV8Zip ();
         JsExtV8Zip (const JsExtV8Zip &);
         JsExtV8Zip &operator= (const JsExtV8Zip &);

   };
};


inline dmz::JsExtV8Zip *
dmz::JsExtV8Zip::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Zip *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_ZIP_DOT_H
