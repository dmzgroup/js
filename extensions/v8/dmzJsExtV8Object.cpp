#include "dmzJsExtV8Object.h"
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsExtV8Object::JsExtV8Object (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      TimeSlice (Info),
      ObjectObserverUtil (Info, local),
      _log (Info) {

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

   }
   else if (Mode == PluginDiscoverRemove) {

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


void
dmz::JsExtV8Object::_init (Config &local) {

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
