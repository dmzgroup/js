#ifndef DMZ_JS_EXT_V8_FILE_DOT_H
#define DMZ_JS_EXT_V8_FILE_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class JsExtV8File :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8File (const PluginInfo &Info, Config &local);
         ~JsExtV8File ();

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
         // JsExtV8File Interface
         static JsExtV8File *_to_self (const v8::Arguments &Args);
         static V8Value _file_split (const v8::Arguments &Args);
         static V8Value _file_valid (const v8::Arguments &Args);
         static V8Value _file_read (const v8::Arguments &Args);
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;

         V8InterfaceHelper _fileApi;

         v8::Handle<v8::Context> _v8Context;

         V8ValuePersist _self;

      private:
         JsExtV8File ();
         JsExtV8File (const JsExtV8File &);
         JsExtV8File &operator= (const JsExtV8File &);

   };
};


inline dmz::JsExtV8File *
dmz::JsExtV8File::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8File *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_FILE_DOT_H
