#include "dmzJsExtV8RenderOverlay.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRenderModuleOverlay.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsExtV8RenderOverlay::JsExtV8RenderOverlay (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _overlay (0) {

   _init (local);
}


dmz::JsExtV8RenderOverlay::~JsExtV8RenderOverlay () {

}


// Plugin Interface
void
dmz::JsExtV8RenderOverlay::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

   }
}


void
dmz::JsExtV8RenderOverlay::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_overlay) { _overlay = RenderModuleOverlay::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_overlay && (_overlay == RenderModuleOverlay::cast (PluginPtr))) {

         _overlay = 0;
      }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8RenderOverlay::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8RenderOverlay::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8RenderOverlay::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/components/overlay",
            _overlayApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _nodeCtor = V8FunctionPersist::New (_nodeTemp->GetFunction ());
      _textNodeCtor = V8FunctionPersist::New (_textNodeTemp->GetFunction ());
      _groupNodeCtor = V8FunctionPersist::New (_groupNodeTemp->GetFunction ());
      _switchNodeCtor = V8FunctionPersist::New (_switchNodeTemp->GetFunction ());
      _xformNodeCtor = V8FunctionPersist::New (_xformNodeTemp->GetFunction ());

      _redStr = V8StringPersist::New (v8::String::NewSymbol ("red"));
      _greenStr = V8StringPersist::New (v8::String::NewSymbol ("green"));
      _blueStr = V8StringPersist::New (v8::String::NewSymbol ("blue"));
      _alphaStr = V8StringPersist::New (v8::String::NewSymbol ("alpha"));
   }
   else if (State == JsExtV8::Shutdown) {

      _redStr.Dispose (); _redStr.Clear ();
      _greenStr.Dispose (); _greenStr.Clear ();
      _blueStr.Dispose (); _blueStr.Clear ();
      _alphaStr.Dispose (); _alphaStr.Clear ();

      _nodeCtor.Dispose (); _nodeCtor.Clear ();
      _textNodeCtor.Dispose (); _textNodeCtor.Clear ();
      _groupNodeCtor.Dispose (); _groupNodeCtor.Clear ();
      _switchNodeCtor.Dispose (); _switchNodeCtor.Clear ();
      _xformNodeCtor.Dispose (); _xformNodeCtor.Clear ();

      _overlayApi.clear ();
      _v8Context.Clear ();
   }
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_color (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);
   const String Name = v8_to_string (Args[0]);

   if (self && self->_overlay && Name) {

      Float64 red (0.0), green (0.0), blue (0.0), alpha (0.0);

      if (self->_overlay->lookup_named_color (Name, red, green, blue, alpha)) {

         V8Object obj = v8::Object::New ();
         obj->Set (self->_redStr, v8::Number::New (red));
         obj->Set (self->_greenStr, v8::Number::New (green));
         obj->Set (self->_blueStr, v8::Number::New (blue));
         obj->Set (self->_alphaStr, v8::Number::New (alpha));
         result = obj;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_lookup (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);
   const String Name = v8_to_string (Args[0]);

   if (self && self->_overlay && Name) {

      const Handle Node = self->_overlay->lookup_node_handle (Name);
      result = self->_to_v8_node (Node);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_instance (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);
   const String Name = v8_to_string (Args[0]);

   if (self && self->_overlay) {

      const Handle Node = self->_overlay->clone_template (Name);
      if (Node) { result = self->_to_v8_node (Node); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_destroy (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);
   Handle node (0);

   if (Args[0]->IsObject ()) {

      V8Object obj = V8Object::Cast (Args[0]);

      node = v8_to_handle (obj->GetInternalField (0));
   }

   if (self && self->_overlay && node) {

      result = v8::Boolean::New (self->_overlay->destroy_node (node));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_is_node (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);

   if (self) { result = v8::Boolean::New (self->_nodeTemp->HasInstance (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_is_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);

   if (self) { result = v8::Boolean::New (self->_textNodeTemp->HasInstance (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_is_group (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);

   if (self) { result = v8::Boolean::New (self->_groupNodeTemp->HasInstance (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_is_switch (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);

   if (self) { result = v8::Boolean::New (self->_switchNodeTemp->HasInstance (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_is_transform (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderOverlay *self = to_self (Args);

   if (self) { result = v8::Boolean::New (self->_xformNodeTemp->HasInstance (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_get_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      const String Name = self->_overlay->lookup_node_name (node);

      if (Name) { result = v8::String::New (Name.get_buffer ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_lookup_sub_node (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);
   const String Name = v8_to_string (Args[0]);

   if (self && self->_overlay && node && Name) {

      const Handle Sub = self->_overlay->lookup_node_clone_sub_handle (node, Name);

      if (Sub) { result = self->_to_v8_node (Sub); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_node_color (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      Float64 red (0.0), green (0.0), blue (0.0), alpha (1.0);

      const int Length = Args.Length ();

      if (Length == 0) {

      }
      else if (Length == 1) {

         V8Object color = v8_to_object (Args[0]);

         if (color.IsEmpty () == false) {

            red = v8_to_number (color->Get (self->_redStr), red);
            green = v8_to_number (color->Get (self->_greenStr), green);
            blue = v8_to_number (color->Get (self->_blueStr), blue);
            alpha = v8_to_number (color->Get (self->_alphaStr), alpha);
         }
      }
      else {

         red = v8_to_number (Args[0], red);
         green = v8_to_number (Args[1], green);
         blue = v8_to_number (Args[2], blue);
         alpha = v8_to_number (Args[3], alpha);
      }

      if (self->_overlay->store_color (node, red, green, blue, alpha)) {

         V8Object obj = v8::Object::New ();
         obj->Set (self->_redStr, v8::Number::New (red));
         obj->Set (self->_greenStr, v8::Number::New (green));
         obj->Set (self->_blueStr, v8::Number::New (blue));
         obj->Set (self->_alphaStr, v8::Number::New (alpha));
         result = obj;
      }
   }

   return scope.Close (result);

}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      if (Args.Length () > 0) {

         const String Value = v8_to_string (Args[0]);

         if (self->_overlay->store_text (node, Value)) { result = Args[0]; }
      }
      else {

         String value;

         if (self->_overlay->lookup_text (node, value) && value) {

            result = v8::String::New (value.get_buffer ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_add (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);
   Handle child (0);

   V8Object obj = v8_to_object (Args[0]);
   if (obj.IsEmpty () == false) { child = v8_to_handle (obj->GetInternalField (0)); }

   if (self && self->_overlay && node && child) {

      result = v8::Boolean::New (self->_overlay->add_group_child (node, child));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);
   Handle child (0);

   V8Object obj = v8_to_object (Args[0]);
   if (obj.IsEmpty () == false) { child = v8_to_handle (obj->GetInternalField (0)); }

   if (self && self->_overlay && node && child) {

      result = v8::Boolean::New (self->_overlay->remove_group_child (node, child));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_get_child_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      result = v8::Integer::New (self->_overlay->lookup_group_child_count (node));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_switch_state (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      if (Args.Length () > 0) {

         const Int32 Which = v8_to_int32 (Args[0]);

         if (Args.Length () == 1) {

            result = v8::Boolean::New (self->_overlay->lookup_switch_state (node, Which));
         }
         else {

            const Boolean State = v8_to_boolean (Args[1]);
            result = v8::Boolean::New (
               self->_overlay->store_switch_state (node, Which, State));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_set_switch_state_all (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      const Boolean State = v8_to_boolean (Args[0]);
      result = v8::Boolean::New (self->_overlay->store_switch_state_all (node, State));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_enable_single_switch_state (
      const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      const Int32 Which = v8_to_int32 (Args[0]);

      result = v8::Boolean::New (
         self->_overlay->enable_switch_state_single (node, Which));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_position (
      const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      Float64 theX (0.0), theY (0.0);

      const int Length = Args.Length ();

      if (Length > 0) {

         self->_get_x_and_y (Args, theX, theY);
         self->_overlay->store_transform_position (node, theX, theY);
      }

      if (self->_overlay->lookup_transform_position (node, theX, theY)) {

         V8Array array = v8::Array::New (2);
         array->Set (v8::Integer::New (0), v8::Number::New (theX));
         array->Set (v8::Integer::New (1), v8::Number::New (theY));
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_rotation (
      const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      if (Args.Length () > 0) {

         const Float64 Value = v8_to_number (Args[0]);

         if (self->_overlay->store_transform_rotation (node, Value)) {

            result = v8::Number::New (Value);
         }
      }
      else {

         Float64 value (0.0);

         if (self->_overlay->lookup_transform_rotation (node, value)) {

            result = v8::Number::New (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_overlay_scale (
      const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   Handle node (0);
   JsExtV8RenderOverlay *self = to_node (Args, node);

   if (self && self->_overlay && node) {

      Float64 theX (0.0), theY (0.0);

      const int Length = Args.Length ();

      if (Length > 0) {

         self->_get_x_and_y (Args, theX, theY);
         self->_overlay->store_transform_scale (node, theX, theY);
      }

      if (self->_overlay->lookup_transform_scale (node, theX, theY)) {

         V8Array array = v8::Array::New (2);
         array->Set (v8::Integer::New (0), v8::Number::New (theX));
         array->Set (v8::Integer::New (1), v8::Number::New (theY));
         result = array;
      }

   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8RenderOverlay::_to_v8_node (const Handle Node) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   if (_overlay) {

      const RenderOverlayTypeEnum Type = _overlay->lookup_node_type (Node);

      V8Object obj;

      if (Type == RenderOverlayNode) {

         if (_nodeCtor.IsEmpty () == false) { obj = _nodeCtor->NewInstance (); }
      }
      else if (Type == RenderOverlayText) {

         if (_textNodeCtor.IsEmpty () == false) { obj = _textNodeCtor->NewInstance (); }
      }
      else if (Type == RenderOverlayGroup) {

         if (_groupNodeCtor.IsEmpty () == false) { obj = _groupNodeCtor->NewInstance (); }
      }
      else if (Type == RenderOverlaySwitch) {

         if (_switchNodeCtor.IsEmpty () == false) {

            obj = _switchNodeCtor->NewInstance ();
         }
      }
      else if (Type == RenderOverlayTransform) {

         if (_xformNodeCtor.IsEmpty () == false) { obj = _xformNodeCtor->NewInstance (); }
      }

      if (obj.IsEmpty () == false) {

         obj->SetInternalField (0, v8::Integer::New (Node));
         result = obj;
      }
   }

   return scope.Close (result);
}


void
dmz::JsExtV8RenderOverlay::_get_x_and_y (
      const v8::Arguments &Args,
      Float64 &theX,
      Float64 &theY) {

   v8::HandleScope scope;

   const int Length = Args.Length ();

   if (Length == 1) {

      if (Args[0]->IsArray ()) {

         V8Array array = V8Array::Cast (Args[0]);
         theX = v8_to_number (array->Get (v8::Number::New (0)));
         theY = v8_to_number (array->Get (v8::Number::New (1)));
      }
      else if (Args[0]->IsObject ()) {

         V8Object obj = V8Object::Cast (Args[0]);
         theX = v8_to_number (obj->Get (v8::String::NewSymbol ("x")));
         theY = v8_to_number (obj->Get (v8::String::NewSymbol ("y")));
      }
      else {

         theY = theX = v8_to_number (Args[0]);
      }
   }
   else if (Length > 1) {

      theX = v8_to_number (Args[0]);
      theY = v8_to_number (Args[1]);
   }
}

void
dmz::JsExtV8RenderOverlay::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // API
   _overlayApi.add_function ("color", _overlay_color, _self);
   _overlayApi.add_function ("lookup", _overlay_lookup, _self);
   _overlayApi.add_function ("instance", _overlay_instance, _self);
   _overlayApi.add_function ("destroy", _overlay_destroy, _self);
   _overlayApi.add_function ("isNode", _overlay_is_node, _self);
   _overlayApi.add_function ("isText", _overlay_is_text, _self);
   _overlayApi.add_function ("isGroup", _overlay_is_group, _self);
   _overlayApi.add_function ("isSwitch", _overlay_is_switch, _self);
   _overlayApi.add_function ("isTransform", _overlay_is_transform, _self);

   _nodeTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _nodeTemp->SetClassName (v8::String::NewSymbol ("dmz::OverlayNode"));
   V8ObjectTemplate nodeProto = _nodeTemp->PrototypeTemplate ();
   nodeProto->Set ("name", v8::FunctionTemplate::New (_overlay_get_name, _self));
   nodeProto->Set ("color", v8::FunctionTemplate::New (_overlay_node_color, _self));
   V8ObjectTemplate nodeInstance = _nodeTemp->InstanceTemplate ();
   nodeInstance->SetInternalFieldCount (1);

   _textNodeTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _textNodeTemp->SetClassName (v8::String::NewSymbol ("dmz::OverlayTextNode"));
   _textNodeTemp->Inherit (_nodeTemp);
   V8ObjectTemplate textProto = _textNodeTemp->PrototypeTemplate ();
   textProto->Set ("text", v8::FunctionTemplate::New (_overlay_text, _self));
   V8ObjectTemplate textInstance = _textNodeTemp->InstanceTemplate ();
   textInstance->SetInternalFieldCount (1);

   _groupNodeTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _groupNodeTemp->SetClassName (v8::String::NewSymbol ("dmz::OverlayGroupNode"));
   _groupNodeTemp->Inherit (_nodeTemp);
   V8ObjectTemplate groupProto = _groupNodeTemp->PrototypeTemplate ();
   groupProto->Set (
      "lookup",
      v8::FunctionTemplate::New (_overlay_lookup_sub_node, _self));
   groupProto->Set ("add", v8::FunctionTemplate::New (_overlay_add, _self));
   groupProto->Set ("remove", v8::FunctionTemplate::New (_overlay_remove, _self));
   groupProto->Set (
      "childCount",
      v8::FunctionTemplate::New (_overlay_get_child_count, _self));
   V8ObjectTemplate groupInstance = _groupNodeTemp->InstanceTemplate ();
   groupInstance->SetInternalFieldCount (1);

   _switchNodeTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _switchNodeTemp->SetClassName (v8::String::NewSymbol ("dmz::OverlaySwitchNode"));
   _switchNodeTemp->Inherit (_groupNodeTemp);
   V8ObjectTemplate switchProto = _switchNodeTemp->PrototypeTemplate ();
   switchProto->Set (
      "switchState",
      v8::FunctionTemplate::New (_overlay_switch_state, _self));
   switchProto->Set (
      "setSwitchStateAll",
      v8::FunctionTemplate::New (_overlay_set_switch_state_all, _self));
   switchProto->Set (
      "enableSingleSwitchState",
      v8::FunctionTemplate::New (_overlay_enable_single_switch_state, _self));
   V8ObjectTemplate switchInstance = _switchNodeTemp->InstanceTemplate ();
   switchInstance->SetInternalFieldCount (1);

   _xformNodeTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _xformNodeTemp->SetClassName (v8::String::NewSymbol ("dmz::OverlayTransformNode"));
   _xformNodeTemp->Inherit (_groupNodeTemp);
   V8ObjectTemplate xformProto = _xformNodeTemp->PrototypeTemplate ();
   xformProto->Set ("position", v8::FunctionTemplate::New (_overlay_position, _self));
   xformProto->Set ("rotation", v8::FunctionTemplate::New (_overlay_rotation, _self));
   xformProto->Set ("scale", v8::FunctionTemplate::New (_overlay_scale, _self));
   V8ObjectTemplate xformInstance = _xformNodeTemp->InstanceTemplate ();
   xformInstance->SetInternalFieldCount (1);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8RenderOverlay (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8RenderOverlay (Info, local);
}

};
