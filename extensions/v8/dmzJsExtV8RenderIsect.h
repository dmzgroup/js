#ifndef DMZ_JS_EXT_V8_RENDER_ISECT_DOT_H
#define DMZ_JS_EXT_V8_RENDER_ISECT_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableUInt32Template.h>

#include <v8.h>

namespace dmz {

   class IsectParameters;
   class IsectTestContainer;
   class JsModuleTypesV8;
   class JsModuleV8;
   class RenderModuleIsect;

   class JsExtV8RenderIsect :
         public Plugin,
         public JsExtV8 {

      public:
         static JsExtV8RenderIsect *to_self (const v8::Arguments &Args);

         JsExtV8RenderIsect (const PluginInfo &Info, Config &local);
         ~JsExtV8RenderIsect ();

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

      protected:
         typedef HashTableUInt32Template<V8FunctionPersist> FuncTable;
         static V8Value _isect_do_isect (const v8::Arguments &Args);
         static V8Value _isect_enable (const v8::Arguments &Args);
         static V8Value _isect_disable (const v8::Arguments &Args);
         Boolean _get_params (V8Object obj, IsectParameters &params);

         Boolean _add_test (
            const UInt32 Id,
            V8Object test,
            IsectTestContainer &list,
            FuncTable &table,
            String &error);

         void _init (Config &local);

         Log _log;
         Definitions _defs;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;
         V8StringPersist _startStr;
         V8StringPersist _endStr;
         V8StringPersist _directionStr;
         V8StringPersist _callbackStr;

         V8StringPersist _idStr;
         V8StringPersist _typeStr;
         V8StringPersist _pointStr;
         V8StringPersist _normalStr;
         V8StringPersist _objectStr;
         V8StringPersist _distanceStr;
         V8StringPersist _cullStr;
         V8StringPersist _attrStr;

         JsModuleTypesV8 *_types;
         JsModuleV8 *_core;
         RenderModuleIsect *_isect;

         V8InterfaceHelper _isectApi;

      private:
         JsExtV8RenderIsect ();
         JsExtV8RenderIsect (const JsExtV8RenderIsect &);
         JsExtV8RenderIsect &operator= (const JsExtV8RenderIsect &);

   };
};


inline dmz::JsExtV8RenderIsect *
dmz::JsExtV8RenderIsect::to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8RenderIsect *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_RENDER_ISECT_DOT_H
