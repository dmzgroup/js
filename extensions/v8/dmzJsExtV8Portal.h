#ifndef DMZ_JS_EXT_V8_PORTAL_DOT_H
#define DMZ_JS_EXT_V8_PORTAL_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableUInt32Template.h>

#include <v8.h>

namespace dmz {

   class AudioModulePortal;
   class JsModuleTypesV8;
   class JsModuleV8;
   class RenderModulePortal;

   class JsExtV8Portal :
         public Plugin,
         public JsExtV8 {

      public:
         static JsExtV8Portal *to_self (const v8::Arguments &Args);

         JsExtV8Portal (const PluginInfo &Info, Config &local);
         ~JsExtV8Portal ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

      protected:
         static V8Value _portal_view (const v8::Arguments &Args);

         void _init (Config &local);

         Log _log;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;
         V8StringPersist _posStr;
         V8StringPersist _oriStr;

         JsModuleTypesV8 *_types;
         JsModuleV8 *_core;

         AudioModulePortal *_audio;
         RenderModulePortal *_render;

         V8InterfaceHelper _portalApi;

      private:
         JsExtV8Portal ();
         JsExtV8Portal (const JsExtV8Portal &);
         JsExtV8Portal &operator= (const JsExtV8Portal &);

   };
};


inline dmz::JsExtV8Portal *
dmz::JsExtV8Portal::to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Portal *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_PORTAL_DOT_H
