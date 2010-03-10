#include "dmzJsExtV8Object.h"
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsModuleV8.h>
#include <dmzObjectConsts.h>
#include <dmzObjectModule.h>
#include <dmzObjectAttributeMasks.h>
#include <dmzRuntimeObjectType.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesUUID.h>

dmz::Handle
dmz::JsExtV8Object::_to_attr (JsExtV8Object *self, V8Value value) {

   Handle result (0);

   if (self) { result = self->_to_handle (value); }

   return result;
}


dmz::Handle
dmz::JsExtV8Object::_to_object (JsExtV8Object *self, V8Value value) {

   Handle result (0);

   if (self) { result = self->_to_object_handle (value); }

   return result;
}


dmz:: Boolean
dmz::JsExtV8Object::_get_params (
      const v8::Arguments &Args,
      ObjectModule *&objMod,
      JsModuleTypesV8 *&types,
      Handle &obj,
      Handle &attr) {

   objMod = 0;
   types = 0;
   obj = 0;
   attr = 0;
   JsExtV8Object *self = to_self (Args);

   if (self) {

      objMod = self->get_object_module ();
      types = self->_types;

      V8Value objVal = Args[0];

      if (objVal->IsNumber ()) { obj = objVal->Uint32Value (); }
      else if (objVal->IsString ()) {

         const String IdStr = v8_to_string (objVal);
         const UUID Id (IdStr);

         if (Id && objMod) { obj = objMod->lookup_handle_from_uuid (Id); }
      }

      if (Args.Length () > 1) {

         V8Value arg = Args[1];

         if (arg->IsString ()) {

            const String Name = *(v8::String::AsciiValue (arg));
            attr = self->_defs.create_named_handle (Name);
         }
         else if (arg->IsNumber ()) {

            attr = arg->Uint32Value ();
         }
      }
      else { attr = self->_defaultAttr; }
   }

   return objMod && types && obj && attr;
}


dmz::V8Value
dmz::JsExtV8Object::_object_create (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);

   if (self && self->_runtime && self->_types && objMod) {

      ObjectType type;

      if (self->_runtime->to_dmz_object_type (Args[0], type)) {

         ObjectLocalityEnum locality = ObjectLocal;

         if (Args.Length () > 1) {

            locality = (ObjectLocalityEnum)v8_to_uint32 (Args[1]);

            if (locality == ObjectLocalityUnknown) { locality = ObjectLocal; }
         }

         Handle object = objMod->create_object (type, locality);

         if (object && (locality == ObjectLocal)) {

            self->_localObjects.add_handle (object);
         }

         result = v8::Number::New ((double)object);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_activate (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_uint32 (Args[0]);

   if (objMod && Object) {

      result = v8::Boolean::New (objMod->activate_object (Object));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_destroy (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_uint32 (Args[0]);

   if (objMod && Object) {

      result = v8::Boolean::New (objMod->destroy_object (Object));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_clone (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_uint32 (Args[0]);

   if (objMod && Object) {

      ObjectLinkRetentionEnum linkRetention (ObjectIgnoreLinks);

      if (Args.Length () > 1) {

         linkRetention = (ObjectLinkRetentionEnum)Args[1]->Uint32Value ();
      }

      result = v8::Number::New ((double)objMod->clone_object (Object, linkRetention));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_make_persistent (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   const Handle Object = v8_to_uint32 (Args[0]);

   if (self && Object) {

      result = v8::Boolean::New (self->_localObjects.remove_handle (Object));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_lookup_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   JsModuleRuntimeV8 *runtime = self ? self->_runtime : 0;
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_uint32 (Args[0]);

   if (objMod && runtime && Object) {

      ObjectType type = objMod->lookup_object_type (Object);

      if (type) { result = runtime->create_v8_object_type (&type); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_uuid (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_uint32 (Args[0]);

   if (objMod && Object) {

      if (Args.Length () > 1) {

         UUID id;
         const String Value = v8_to_string (Args[2]);

         if (id.from_string (Value) && objMod->store_uuid (Object, id)) {

            result = v8::String::New (id.to_string (UUID::NotFormatted).get_buffer ());
         }
      }
      else {

         UUID id;

         if (objMod->lookup_uuid (Object, id)) {

            result = v8::String::New (id.to_string (UUID::NotFormatted).get_buffer ());
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_link (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Attr = _to_attr (self, Args[0]);
   const Handle Super = _to_object (self, Args[1]);
   const Handle Sub = _to_object (self, Args[2]);

   if (objMod && Attr && Super && Sub) {

      Handle link = objMod->link_objects (Attr, Super, Sub);

      if (link) { result = v8::Number::New ((double)link); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_lookup_link_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Attr = _to_attr (self, Args[0]);
   const Handle Super = _to_object (self, Args[1]);
   const Handle Sub = _to_object (self, Args[2]);

   if (objMod && Attr && Super && Sub) {

      Handle link = objMod->lookup_link_handle (Attr, Super, Sub);

      if (link) { result = v8::Number::New ((double)link); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_lookup_linked_objects (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Object result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Link = v8_to_uint32 (Args[0]);

   if (objMod && Link) {

      Handle attr (0), super (0), sub (0);

      if (objMod->lookup_linked_objects (Link, attr, super, sub)) {

         result = v8::Object::New ();

         result->Set (
            v8::String::NewSymbol ("attribute"),
            v8::Number::New ((double)attr));

         result->Set (v8::String::NewSymbol ("super"), v8::Number::New ((double)super));
         result->Set (v8::String::NewSymbol ("sub"), v8::Number::New ((double)sub));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_unlink (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Link = v8_to_uint32 (Args[0]);

   if (objMod && Link) {

      result = v8::Boolean::New (objMod->unlink_objects (Link));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_unlink_super_objects (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = _to_object (self, Args[0]);
   const Handle Attr = _to_attr (self, Args[1]);

   if (objMod && Object && Attr) {

      result = v8::Boolean::New (objMod->unlink_super_links (Object, Attr));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_unlink_sub_objects (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = _to_object (self, Args[0]);
   const Handle Attr = _to_attr (self, Args[1]);

   if (objMod && Object && Attr) {

      result = v8::Boolean::New (objMod->unlink_sub_links (Object, Attr));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_lookup_super_links (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = _to_object (self, Args[0]);
   const Handle Attr = _to_attr (self, Args[1]);

   if (objMod && Object && Attr) {

      HandleContainer container;

      if (objMod->lookup_super_links (Object, Attr, container)) {

         result = v8_to_array (container);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_lookup_sub_links (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = _to_object (self, Args[0]);
   const Handle Attr = _to_attr (self, Args[1]);

   if (objMod && Object && Attr) {

      HandleContainer container;

      if (objMod->lookup_sub_links (Object, Attr, container)) {

         result = v8_to_array (container);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_link_attribute_object (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Link = v8_to_handle (Args[0]);
   const Handle Object = _to_object (self, Args[1]);

   if (objMod && Link) {

      Handle attrObj (0);

      if (Object) {

         if (objMod->store_link_attribute_object (Link, Object)) { attrObj = Object; }
      }
      else {

         attrObj = objMod->lookup_link_attribute_object (Link);
      }

      if (attrObj) { result = v8::Number::New ((double)attrObj); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_lookup_attribute_object_links (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = _to_object (self, Args[0]);

   if (objMod && Object) {

      HandleContainer container;

      if (objMod->lookup_attribute_object_links (Object, container)) {

         result = v8_to_array (container);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_position (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Vector value = types->to_dmz_vector (Args[3]);

         if (objMod->store_position (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
      else {

         Vector value;

         if (objMod->lookup_position (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_velocity (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Vector value = types->to_dmz_vector (Args[3]);

         if (objMod->store_velocity (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
      else {

         Vector value;

         if (objMod->lookup_velocity (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_acceleration (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Vector value = types->to_dmz_vector (Args[3]);

         if (objMod->store_acceleration (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
      else {

         Vector value;

         if (objMod->lookup_acceleration (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_scale (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Vector value = types->to_dmz_vector (Args[3]);

         if (objMod->store_scale (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
      else {

         Vector value;

         if (objMod->lookup_scale (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_vector (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Vector value = types->to_dmz_vector (Args[3]);

         if (objMod->store_vector (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
      else {

         Vector value;

         if (objMod->lookup_vector (obj, attr, value)) {

            result = types->to_v8_vector (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::JsExtV8Object::JsExtV8Object (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      TimeSlice (Info),
      ObjectObserverUtil (Info, local),
      _log (Info),
      _defs (Info),
      _defaultAttr (0),
      _runtime (0),
      _types (0),
      _core (0) {

   _init (local);
}


dmz::JsExtV8Object::~JsExtV8Object () {

}


// Plugin Interface
void
dmz::JsExtV8Object::update_plugin_state (
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
dmz::JsExtV8Object::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
      if (!_types) { _types = JsModuleTypesV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Object::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Object::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Object::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/framework/object",
            _objectApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      ObjectModule *objMod = get_object_module ();

      if (objMod) {

         HandleContainerIterator it;
         Handle object (0);

         while (_localObjects.get_next (it, object)) {

            objMod->destroy_object (object);
         }

         _localObjects.clear ();
      }
   }
}


// TimeSlice Interface
void
dmz::JsExtV8Object::update_time_slice (const Float64 TimeDelta) {


}


// Object Observer Interface
void
dmz::JsExtV8Object::create_object (
      const UUID &Identity,
      const Handle ObjectHandle,
      const ObjectType &Type,
      const ObjectLocalityEnum Locality) {

}


void
dmz::JsExtV8Object::destroy_object (
      const UUID &Identity,
      const Handle ObjectHandle) {

}


void
dmz::JsExtV8Object::update_object_uuid (
      const Handle ObjectHandle,
      const UUID &Identity,
      const UUID &PrevIdentity) {

}


void
dmz::JsExtV8Object::remove_object_attribute (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Mask &AttrMask) {

}


void
dmz::JsExtV8Object::update_object_locality (
      const UUID &Identity,
      const Handle ObjectHandle,
      const ObjectLocalityEnum Locality,
      const ObjectLocalityEnum PrevLocality) {

}


void
dmz::JsExtV8Object::link_objects (
      const Handle LinkHandle,
      const Handle AttributeHandle,
      const UUID &SuperIdentity,
      const Handle SuperHandle,
      const UUID &SubIdentity,
      const Handle SubHandle) {

}


void
dmz::JsExtV8Object::unlink_objects (
      const Handle LinkHandle,
      const Handle AttributeHandle,
      const UUID &SuperIdentity,
      const Handle SuperHandle,
      const UUID &SubIdentity,
      const Handle SubHandle) {

}


void
dmz::JsExtV8Object::update_link_attribute_object (
      const Handle LinkHandle,
      const Handle AttributeHandle,
      const UUID &SuperIdentity,
      const Handle SuperHandle,
      const UUID &SubIdentity,
      const Handle SubHandle,
      const UUID &AttributeIdentity,
      const Handle AttributeObjectHandle,
      const UUID &PrevAttributeIdentity,
      const Handle PrevAttributeObjectHandle) {


}


void
dmz::JsExtV8Object::update_object_counter (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_counter_minimum (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_counter_maximum (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_alternate_type (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const ObjectType &Value,
      const ObjectType *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_state (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Mask &Value,
      const Mask *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_flag (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Boolean Value,
      const Boolean *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_time_stamp (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Float64 Value,
      const Float64 *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_position (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_orientation (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Matrix &Value,
      const Matrix *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_velocity (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_acceleration (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_scale (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_vector (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_scalar (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Float64 Value,
      const Float64 *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_text (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const String &Value,
      const String *PreviousValue) {

}


void
dmz::JsExtV8Object::update_object_data (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Data &Value,
      const Data *PreviousValue) {

}


// JsExtV8Object Interface
dmz::Handle
dmz::JsExtV8Object::_to_handle (V8Value value) {

   Handle result (0);

   if (value.IsEmpty () == false) {

      if (value->IsNumber ()) { result = value->Uint32Value (); }
      else if (value->IsString ()) {

         result = _defs.create_named_handle (*(v8::String::AsciiValue (value)));
      }
   }

   return result;
}


dmz::Handle
dmz::JsExtV8Object::_to_object_handle (V8Value value) {

   Handle result (0);

   if (value.IsEmpty () == false) {

      if (value->IsNumber ()) { result = value->Uint32Value (); }
      else if (value->IsString ()) {

         ObjectModule *objMod = get_object_module ();

         if (objMod) {

            const String IdStr = *(v8::String::AsciiValue (value));
            const UUID id (IdStr);

            result = objMod->lookup_handle_from_uuid (id);
         }
      }
   }

   return result;
}


void
dmz::JsExtV8Object::_init (Config &local) {

   v8::HandleScope scope;

   _defaultAttr = _defs.create_named_handle (ObjectAttributeDefaultName);

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Constants
   _objectApi.add_constant ("LocalityUnknown", (UInt32)ObjectLocalityUnknown);
   _objectApi.add_constant ("Local", (UInt32)ObjectLocal);
   _objectApi.add_constant ("Remote", (UInt32)ObjectRemote);
   _objectApi.add_constant ("RetainLinks", (UInt32)ObjectRetainLinks);
   _objectApi.add_constant ("IgnoreLinks", (UInt32)ObjectIgnoreLinks);
   _objectApi.add_constant ("SelectNew", (UInt32)ObjectSelectNew);
   _objectApi.add_constant ("SelectAdd", (UInt32)ObjectSelectAdd);
   // Functions
   _objectApi.add_function ("create", _object_create, _self);
   _objectApi.add_function ("activate", _object_activate, _self);
   _objectApi.add_function ("destroy", _object_destroy, _self);
   _objectApi.add_function ("clone", _object_clone, _self);
   _objectApi.add_function ("makePersistent", _object_make_persistent, _self);
   _objectApi.add_function ("lookupType", _object_lookup_type, _self);
   _objectApi.add_function ("uuid", _object_uuid, _self);
   _objectApi.add_function ("link", _object_link, _self);
   _objectApi.add_function ("lookupLinkHandle", _object_lookup_link_handle, _self);
   _objectApi.add_function ("lookupLinkedObjects", _object_lookup_linked_objects, _self);
   _objectApi.add_function ("unlink", _object_unlink, _self);
   _objectApi.add_function ("unlinkSuperObjects", _object_unlink_super_objects, _self);
   _objectApi.add_function ("unlinkSubObjects", _object_unlink_sub_objects, _self);
   _objectApi.add_function ("lookupSuperLinks", _object_lookup_super_links, _self);
   _objectApi.add_function ("lookupSubLinks", _object_lookup_sub_links, _self);
   _objectApi.add_function ("linkAttributeObject", _object_link_attribute_object, _self);
   _objectApi.add_function (
      "lookupAttributeObjectLinks",
      _object_lookup_attribute_object_links,
      _self);
   _objectApi.add_function ("position", _object_position, _self);
   _objectApi.add_function ("velocity", _object_velocity, _self);
   _objectApi.add_function ("acceleration", _object_acceleration, _self);
   _objectApi.add_function ("scale", _object_scale, _self);
   _objectApi.add_function ("vector", _object_vector, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Object (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Object (Info, local);
}

};
