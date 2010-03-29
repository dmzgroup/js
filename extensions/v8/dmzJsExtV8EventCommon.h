#ifndef DMZ_JS_EXT_V8_EVENT_COMMON_DOT_H
#define DMZ_JS_EXT_V8_EVENT_COMMON_DOT_H

#include <dmzEventObserverUtil.h>
#include <dmzJsExtV8.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeEventType.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHandleContainer.h>
#include <dmzTypesHashTableHandleTemplate.h>

#include <v8.h>

namespace dmz {

   class EventModule;
   class EventModuleCommon;
   class JsModuleV8;
   class JsModuleRuntimeV8;
   class JsModuleTypesV8;

   class JsExtV8EventCommon :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8EventCommon (const PluginInfo &Info, Config &local);
         ~JsExtV8EventCommon ();

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

      protected:
         static JsExtV8EventCommon *_to_self (const v8::Arguments &Args);

         // Event API Bindings
         static V8Value _event_close (const v8::Arguments &Args);
         static V8Value _event_create_launch (const v8::Arguments &Args);
         static V8Value _event_create_open_launch (const v8::Arguments &Args);
         static V8Value _event_create_detonation (const v8::Arguments &Args);
         static V8Value _event_create_open_detonation (const v8::Arguments &Args);
         static V8Value _event_create_collision (const v8::Arguments &Args);
         static V8Value _event_create_open_collision (const v8::Arguments &Args);

         void _init (Config &local);

         Log _log;
         Definitions _defs;

         V8InterfaceHelper _eventApi;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         EventModule *_event;
         EventModuleCommon *_common;
         JsModuleV8 *_core;
         JsModuleRuntimeV8 *_runtime;
         JsModuleTypesV8 *_types;

      private:
         JsExtV8EventCommon ();
         JsExtV8EventCommon (const JsExtV8EventCommon &);
         JsExtV8EventCommon &operator= (const JsExtV8EventCommon &);
   };
};


inline dmz::JsExtV8EventCommon *
dmz::JsExtV8EventCommon::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8EventCommon *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_EVENT_COMMON_DOT_H
