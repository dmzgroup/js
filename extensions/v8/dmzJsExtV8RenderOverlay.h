#ifndef DMZ_JS_EXT_V8_RENDER_OVERLAY_DOT_H
#define DMZ_JS_EXT_V8_RENDER_OVERLAY_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableUInt32Template.h>

#include <v8.h>

namespace dmz {

   class OverlayParameters;
   class OverlayTestContainer;
   class JsModuleTypesV8;
   class JsModuleV8;
   class RenderModuleOverlay;

   class JsExtV8RenderOverlay :
         public Plugin,
         public JsExtV8 {

      public:
         static JsExtV8RenderOverlay *to_self (const v8::Arguments &Args);
         static JsExtV8RenderOverlay *to_node (const v8::Arguments &Args, Handle &node);

         JsExtV8RenderOverlay (const PluginInfo &Info, Config &local);
         ~JsExtV8RenderOverlay ();

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
         static V8Value _overlay_lookup (const v8::Arguments &Args);
         static V8Value _overlay_instance (const v8::Arguments &Args);
         static V8Value _overlay_destroy (const v8::Arguments &Args);
         static V8Value _overlay_is_node (const v8::Arguments &Args);
         static V8Value _overlay_is_text (const v8::Arguments &Args);
         static V8Value _overlay_is_group (const v8::Arguments &Args);
         static V8Value _overlay_is_switch (const v8::Arguments &Args);
         static V8Value _overlay_is_transform (const v8::Arguments &Args);
         // Node API
         static V8Value _overlay_get_name (const v8::Arguments &Args);
         static V8Value _overlay_lookup_sub_node (const v8::Arguments &Args);
         // Text Node API
         static V8Value _overlay_text (const v8::Arguments &Args);
         // Group Node API
         static V8Value _overlay_add (const v8::Arguments &Args);
         static V8Value _overlay_remove (const v8::Arguments &Args);
         static V8Value _overlay_get_child_count (const v8::Arguments &Args);
         // Switch Node API
         static V8Value _overlay_switch_state (const v8::Arguments &Args);
         static V8Value _overlay_set_switch_state_all (const v8::Arguments &Args);
         static V8Value _overlay_enable_single_switch_state (const v8::Arguments &Args);
         // Transform Node API
         static V8Value _overlay_position (const v8::Arguments &Args);
         static V8Value _overlay_rotation (const v8::Arguments &Args);
         static V8Value _overlay_scale (const v8::Arguments &Args);
        

         V8Value _to_v8_node (const Handle Node);
         void _get_x_and_y (const v8::Arguments &Args, Float64 &theX, Float64 &theY);
         void _init (Config &local);

         Log _log;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         JsModuleV8 *_core;
         RenderModuleOverlay *_overlay;

         V8InterfaceHelper _overlayApi;

         V8FunctionTemplatePersist _nodeTemp;
         V8FunctionPersist _nodeCtor;
         V8FunctionTemplatePersist _textNodeTemp;
         V8FunctionPersist _textNodeCtor;
         V8FunctionTemplatePersist _groupNodeTemp;
         V8FunctionPersist _groupNodeCtor;
         V8FunctionTemplatePersist _switchNodeTemp;
         V8FunctionPersist _switchNodeCtor;
         V8FunctionTemplatePersist _xformNodeTemp;
         V8FunctionPersist _xformNodeCtor;

      private:
         JsExtV8RenderOverlay ();
         JsExtV8RenderOverlay (const JsExtV8RenderOverlay &);
         JsExtV8RenderOverlay &operator= (const JsExtV8RenderOverlay &);

   };
};


inline dmz::JsExtV8RenderOverlay *
dmz::JsExtV8RenderOverlay::to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8RenderOverlay *)v8::External::Unwrap (Args.Data ());
}


inline dmz::JsExtV8RenderOverlay *
dmz::JsExtV8RenderOverlay::to_node (const v8::Arguments &Args, Handle &node) {

   node = v8_to_handle (Args.This ()->GetInternalField (1));
   return (dmz::JsExtV8RenderOverlay *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_RENDER_OVERLAY_DOT_H
