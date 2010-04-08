#include "dmzJsExtV8Object.h"
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsModuleV8.h>
#include <dmzObjectConsts.h>
#include <dmzObjectModule.h>
#include <dmzObjectModuleSelect.h>
#include <dmzObjectAttributeMasks.h>
#include <dmzRuntimeData.h>
#include <dmzRuntimeObjectType.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesUUID.h>

#include <qdb.h>
static dmz::qdb out;

inline dmz::Handle
dmz::JsExtV8Object::_to_attr (JsExtV8Object *self, V8Value value) {

   return self ? self->_to_handle (value) : 0;
}


inline dmz::Handle
dmz::JsExtV8Object::_to_object (JsExtV8Object *self, V8Value value) {

   return self ?  self->_to_object_handle (value) : 0;
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
         else { attr = self->_defaultAttr; }
      }
      else { attr = self->_defaultAttr; }
   }

   return objMod && types && obj && attr;
}


dmz::V8Function
dmz::JsExtV8Object::_register_observer_static (
      const v8::Arguments &Args,
      const Mask &AttrMask) {

   v8::HandleScope scope;
   V8Function result;

   JsExtV8Object *self = to_self (Args);
   if (self) { result = self->_register_observer (Args, AttrMask); }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_remove_attribute (
      const v8::Arguments &Args,
      const Mask &AttrMask) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0);
   Handle attr (0);

   if (_get_params (Args, objMod, types, obj, attr)) {

      result = v8::Boolean::New (objMod->remove_attribute (obj, attr, AttrMask));
   }

   return scope.Close (result);
}


dmz::V8Function
dmz::JsExtV8Object::_register_observer (
      const v8::Arguments &Args,
      const Mask &AttrMask) {

   v8::HandleScope scope;
   V8Function result;

   const int Length = Args.Length ();

   V8Object src = v8_to_object (Args[0]);
   Handle attr (_defaultAttr);

   if (Length > 2) {

      V8Value arg = Args[1];
      if (arg->IsNumber ()) { attr = arg->Uint32Value (); }
      else if (arg->IsString ()) {

         attr = _defs.create_named_handle (*(v8::String::AsciiValue (arg)));
      }
   }

   V8Function func = v8_to_function (Args[Length > 2 ? 2 : 1]);
   
   if (_core && attr && (src.IsEmpty () == false) && (func.IsEmpty () == false)) {

      Boolean doDump (False);

      const Handle ObsHandle = _core->get_instance_handle (src);

      ObsStruct *os = _obsTable.lookup (ObsHandle);

      if (!os) {

         os = new ObsStruct;
         if (os && !_obsTable.store (ObsHandle, os)) { delete os; os = 0; }
      }

      if (os) {

         CallbackStruct *cbs (0);

         if (AttrMask & ObjectCreateMask) {

            doDump = _createTable.table.get_count () != 0;
            cbs = new CallbackStruct (ObsHandle, &_createTable);
            if (cbs && !cbs->table) { delete cbs; cbs = 0; doDump = False; }
         }
         else if (AttrMask & ObjectDestroyMask) {

            doDump = _destroyTable.table.get_count () != 0;
            cbs = new CallbackStruct (ObsHandle, &_destroyTable);
            if (cbs && !cbs->table) { delete cbs; cbs = 0; doDump = False; }
         }
         else if (AttrMask & ObjectLocalityMask) {

            doDump = _localityTable.table.get_count () != 0;
            cbs = new CallbackStruct (ObsHandle, &_localityTable);
            if (cbs && !cbs->table) { delete cbs; cbs = 0; doDump = False; }
         }
         else if (AttrMask & ObjectUUIDMask) {

            doDump = _uuidTable.table.get_count () != 0;
            cbs = new CallbackStruct (ObsHandle, &_uuidTable);
            if (cbs && !cbs->table) { delete cbs; cbs = 0; doDump = False; }
         }
         else if (AttrMask & ObjectRemoveAttributeMask) {

         }
         else if (AttrMask & ObjectLinkMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _linkTable, doDump);
         }
         else if (AttrMask & ObjectUnlinkMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _unlinkTable, doDump);
         }
         else if (AttrMask & ObjectLinkAttributeMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _linkAttrTable, doDump);
         }
         else if (AttrMask & ObjectCounterMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _counterTable, doDump);
         }
         else if (AttrMask & ObjectMinCounterMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _minCounterTable, doDump);
         }
         else if (AttrMask & ObjectMaxCounterMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _maxCounterTable, doDump);
         }
         else if (AttrMask & ObjectAltTypeMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _altTypeTable, doDump);
         }
         else if (AttrMask & ObjectStateMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _stateTable, doDump);
         }
         else if (AttrMask & ObjectFlagMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _flagTable, doDump);
            if (attr == _hilAttr) { doDump = True; }
         }
         else if (AttrMask & ObjectTimeStampMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _timeStampTable, doDump);
         }
         else if (AttrMask & ObjectPositionMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _positionTable, doDump);
         }
         else if (AttrMask & ObjectOrientationMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _orientationTable, doDump);
         }
         else if (AttrMask & ObjectVelocityMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _velocityTable, doDump);
         }
         else if (AttrMask & ObjectAccelerationMask) {

            cbs = _create_callback (
               ObsHandle,
               attr,
               AttrMask,
               _accelerationTable,
               doDump);
         }
         else if (AttrMask & ObjectScaleMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _scaleTable, doDump);
         }
         else if (AttrMask & ObjectVectorMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _vectorTable, doDump);
         }
         else if (AttrMask & ObjectScalarMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _scalarTable, doDump);
         }
         else if (AttrMask & ObjectTextMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _textTable, doDump);
         }
         else if (AttrMask & ObjectDataMask) {

            cbs = _create_callback (ObsHandle, attr, AttrMask, _dataTable, doDump);
         }

         if (cbs) {

            cbs->self.Dispose (); cbs->self.Clear ();
            cbs->self = V8ObjectPersist::New (src);
            cbs->func.Dispose (); cbs->func.Clear ();
            cbs->func = V8FunctionPersist::New (func);
            result = func;

            cbs->next = os->list; os->list = cbs;

            if (doDump) {

               ObjectModule *objMod = get_object_module ();

               if (objMod) {

                  _newCallback = cbs;
                  objMod->dump_attributes (attr, AttrMask, *this);
                  _newCallback = 0;
               }
            }
            else { activate_object_attribute (attr, AttrMask); }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_release (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);

   if (self && Args[0]->IsObject ()) {

      V8Object src = V8Object::Cast (Args[0]);

      const Handle Obs = self->_core ? self->_core->get_instance_handle (src) : 0;

      ObsStruct *os = self->_obsTable.lookup (Obs);

      if (os) {

         if (Args[1]->IsFunction ()) {

            V8Function func = V8Function::Cast (Args[1]);

            self->_remove_callback (*os, func);
            result = func;
         }
         else if (Args.Length () == 1) {

            while (os->list) {

               self->_remove_callback (*os, os->list->func);
            }

            result = v8::True ();
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_hil (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Object *self = to_self (Args);

   if (self) { result = v8::Integer::New (self->_hil); }
   else { result = v8::Integer::New (0); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_is_object (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_handle (Args[0]);

   if (self && objMod && Object) {

      result = v8::Boolean::New (objMod->is_object (Object));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_is_activated (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_handle (Args[0]);

   if (self && objMod && Object) {

      result = v8::Boolean::New (objMod->is_activated (Object));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_is_link (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Link = v8_to_handle (Args[0]);

   if (self && objMod && Link) {

      result = v8::Boolean::New (objMod->is_link (Link));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_objects (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);

   if (objMod) {

      HandleContainer objects;

      if (objMod->get_object_handles (objects)) {

         result = v8_to_array (objects);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
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

         result = v8::Integer::New (object);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_create_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectCreateMask);

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
dmz::JsExtV8Object::_object_destroy_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectDestroyMask);

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

      result = v8::Integer::New (objMod->clone_object (Object, linkRetention));
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
dmz::JsExtV8Object::_object_type (const v8::Arguments &Args) {

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
dmz::JsExtV8Object::_object_locality (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Object = v8_to_uint32 (Args[0]);

   if (objMod && Object) {

      if (Args.Length () > 1) {

         const ObjectLocalityEnum Locality = (ObjectLocalityEnum) v8_to_uint32 (Args[1]);

         if (objMod->store_locality (Object, Locality)) {

            result = v8::Integer::New (Locality);
         }
      }
      else {

         result = v8::Integer::New (objMod->lookup_locality (Object));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_locality_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectLocalityMask);

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
dmz::JsExtV8Object::_object_uuid_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectUUIDMask);

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

      if (link) { result = v8::Integer::New (link); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_link_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectLinkMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_link_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   ObjectModule *objMod = (self ? self->get_object_module () : 0);
   const Handle Attr = _to_attr (self, Args[0]);
   const Handle Super = _to_object (self, Args[1]);
   const Handle Sub = _to_object (self, Args[2]);

   if (objMod && Attr && Super && Sub) {

      Handle link = objMod->lookup_link_handle (Attr, Super, Sub);

      if (link) { result = v8::Integer::New (link); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_linked_objects (const v8::Arguments &Args) {

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
            v8::Integer::New (attr));

         result->Set (v8::String::NewSymbol ("super"), v8::Integer::New (super));
         result->Set (v8::String::NewSymbol ("sub"), v8::Integer::New (sub));
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
dmz::JsExtV8Object::_object_unlink_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectUnlinkMask);

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
dmz::JsExtV8Object::_object_super_links (const v8::Arguments &Args) {

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
dmz::JsExtV8Object::_object_sub_links (const v8::Arguments &Args) {

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

      if (attrObj) { result = v8::Integer::New (attrObj); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_link_attribute_object_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectLinkAttributeMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_attribute_object_links (const v8::Arguments &Args) {

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
dmz::JsExtV8Object::_object_counter (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Int64 Value = v8_to_int64 (Args[2]);

         if (objMod->store_counter (obj, attr, Value)) {

            result = v8::Integer::New ((Int32)Value);
         }
      }
      else {

         Int64 value (0);

         if (objMod->lookup_counter (obj, attr, value)) {

            result = v8::Integer::New ((Int32)value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectCounterMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectCounterMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_min (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Int64 Value = v8_to_int64 (Args[2]);

         if (objMod->store_counter_minimum (obj, attr, Value)) {

            result = v8::Integer::New ((Int32)Value);
         }
      }
      else {

         Int64 value (0);

         if (objMod->lookup_counter_minimum (obj, attr, value)) {

            result = v8::Integer::New ((Int32)value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_min_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectMinCounterMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_min_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectMinCounterMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_max (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Int64 Value = v8_to_int64 (Args[2]);

         if (objMod->store_counter_maximum (obj, attr, Value)) {

            result = v8::Integer::New ((Int32)Value);
         }
      }
      else {

         Int64 value (0);

         if (objMod->lookup_counter_maximum (obj, attr, value)) {

            result = v8::Integer::New ((Int32)value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_max_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectMaxCounterMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_max_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectMaxCounterMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_counter_rollover (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Boolean Value = v8_to_boolean (Args[2]);

         if (objMod->store_counter_rollover (obj, attr, Value)) {

            result = v8::Boolean::New (Value);
         }
      }
      else {

         Boolean value (False);

         if (objMod->lookup_counter_rollover (obj, attr, value)) {

            result = v8::Boolean::New (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_add_to_counter (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Int64 Value = v8_to_int64 (Args[2]);

         result = v8::Integer::New ((Int32)objMod->add_to_counter (obj, attr, Value));
      }
      else { result = v8::Integer::New ((Int32)objMod->add_to_counter (obj, attr)); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_alt_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   JsModuleRuntimeV8 *runtime = self ? self->_runtime : 0;
   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (runtime && _get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         ObjectType value;

         if (runtime->to_dmz_object_type (Args[2], value) &&
               objMod->store_alternate_object_type (obj, attr, value)) {

            result = runtime->create_v8_object_type (&value);
         }
      }
      else {

         ObjectType value;

         if (objMod->lookup_alternate_object_type (obj, attr, value)) {

            result = runtime->create_v8_object_type (&value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_alt_type_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectAltTypeMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_alt_type_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectAltTypeMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_state (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Mask value;

         if (Args[2]->IsString ()) {

            JsExtV8Object *self = to_self (Args);

            if (self) { self->_defs.lookup_state (v8_to_string (Args[2]), value); }
         }
         else { value  = types->to_dmz_mask (Args[2]); }

         if (objMod->store_state (obj, attr, value)) {

            result = types->to_v8_mask (value);
         }
      }
      else {

         Mask value;

         if (objMod->lookup_state (obj, attr, value)) {

            result = types->to_v8_mask (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_state_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectStateMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_state_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectStateMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_flag (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Boolean Value = v8_to_boolean (Args[2]);

         if (objMod->store_flag (obj, attr, Value)) {

            result = v8::Number::New (Value);
         }
      }
      else {

         result = v8::Boolean::New (objMod->lookup_flag (obj, attr));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_flag_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectFlagMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_flag_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectFlagMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_time_stamp (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Float64 Value = v8_to_number (Args[2]);

         if (objMod->store_time_stamp (obj, attr, Value)) {

            result = v8::Number::New (Value);
         }
      }
      else {

         Float64 value (0.0);

         if (objMod->lookup_time_stamp (obj, attr, value)) {

            result = v8::Number::New (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_time_stamp_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectTimeStampMask);

   return result.IsEmpty () ? result : scope.Close (result);
}

dmz::V8Value
dmz::JsExtV8Object::_object_time_stamp_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectTimeStampMask);

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

         Vector value = types->to_dmz_vector (Args[2]);

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
dmz::JsExtV8Object::_object_position_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectPositionMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_position_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectPositionMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_orientation (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Matrix value = types->to_dmz_matrix (Args[2]);

         if (objMod->store_orientation (obj, attr, value)) {

            result = types->to_v8_matrix (value);
         }
      }
      else {

         Matrix value;

         if (objMod->lookup_orientation (obj, attr, value)) {

            result = types->to_v8_matrix (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_orientation_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectOrientationMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_orientation_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectOrientationMask);

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

         Vector value = types->to_dmz_vector (Args[2]);

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
dmz::JsExtV8Object::_object_velocity_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectVelocityMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_velocity_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectVelocityMask);

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

         Vector value = types->to_dmz_vector (Args[2]);

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
dmz::JsExtV8Object::_object_acceleration_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectAccelerationMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_acceleration_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectAccelerationMask);

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

         Vector value = types->to_dmz_vector (Args[2]);

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
dmz::JsExtV8Object::_object_scale_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectScaleMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_scale_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectScaleMask);

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

         Vector value = types->to_dmz_vector (Args[2]);

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


dmz::V8Value
dmz::JsExtV8Object::_object_vector_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectVectorMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_vector_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectVectorMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_scalar (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const Float64 Value = v8_to_number (Args[2]);

         if (objMod->store_scalar (obj, attr, Value)) {

            result = v8::Number::New (Value);
         }
      }
      else {

         Float64 value (0.0);

         if (objMod->lookup_scalar (obj, attr, value)) {

            result = v8::Number::New (value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_scalar_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectScalarMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_scalar_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectScalarMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (_get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         const String Value = v8_to_string (Args[2]);

         if (objMod->store_text (obj, attr, Value)) {

            result = Args[2];
         }
      }
      else {

         String value;

         if (objMod->lookup_text (obj, attr, value) && value) {

            result = v8::String::New (value.get_buffer ());
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_text_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectTextMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_text_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectTextMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsExtV8Object *self = to_self (Args);
   JsModuleRuntimeV8 *runtime = self ? self->_runtime : 0;
   ObjectModule *objMod (0);
   JsModuleTypesV8 *types (0);
   Handle obj (0), attr (0);
   
   if (runtime && _get_params (Args, objMod, types, obj, attr)) {

      if (Args.Length () > 2) {

         Data value;

         if (runtime->to_dmz_data (Args[2], value) &&
               objMod->store_data (obj, attr, value)) {

            result = Args[2];
         }
      }
      else {

         Data value;

         if (objMod->lookup_data (obj, attr, value)) {

            result = runtime->create_v8_data (&value);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_data_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, ObjectDataMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_data_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _remove_attribute (Args, ObjectDataMask);

   return result.IsEmpty () ? result : scope.Close (result);
}


// Selection Bindings
dmz::V8Value
dmz::JsExtV8Object::_object_selected (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();
   JsExtV8Object *self = to_self (Args);

   if (self && self->_select) {

      HandleContainer list;
      self->_select->get_selected_objects (list);
      result = v8_to_array (list);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_is_selected (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();
   JsExtV8Object *self = to_self (Args);

   if (self && self->_select && (Args.Length () == 1)) {

      const Handle Object = v8_to_handle (Args[0]);
      result = v8::Boolean::New (self->_select->is_selected (Object));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_select (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();
   JsExtV8Object *self = to_self (Args);

   if (self && self->_select) {

      const Handle Object = v8_to_handle (Args[0]);
      ObjectSelectModeEnum mode = ObjectSelectNew;

      if (Args.Length () > 1) {

         const UInt32 Value = v8_to_uint32 (Args[1]);

         if (Value == ObjectSelectAdd) { mode = ObjectSelectAdd; }
         else if (Value == ObjectSelectNew) { mode = ObjectSelectNew; }
         else { self->_log.error << "Unknown selection mode: " << Value << endl; }
      }

      result = v8::Boolean::New (self->_select->select_object (Object, mode));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_unselect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();
   JsExtV8Object *self = to_self (Args);

   if (self && self->_select) {

      const Handle Object = v8_to_handle (Args[0]);
      result = v8::Boolean::New (self->_select->unselect_object (Object));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Object::_object_unselect_all (const v8::Arguments &Args) {

   v8::HandleScope scope;
   JsExtV8Object *self = to_self (Args);

   if (self && self->_select) { self->_select->unselect_all_objects (); }

   return v8::Undefined ();
}


dmz::JsExtV8Object::JsExtV8Object (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      TimeSlice (Info),
      ObjectObserverUtil (Info, local),
      _log (Info),
      _defs (Info),
      _defaultAttr (0),
      _hilAttr (0),
      _hil (0),
      _runtime (0),
      _types (0),
      _core (0),
      _select (0),
      _newCallback (0),
      _createTable (0, ObjectCreateMask),
      _destroyTable (0, ObjectDestroyMask),
      _uuidTable (0, ObjectUUIDMask),
      _localityTable (0, ObjectLocalityMask) {


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
      if (!_select) { _select = ObjectModuleSelect::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
      if (_select && (_select == ObjectModuleSelect::cast (PluginPtr))) { _select = 0; }
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
            "dmz/components/object",
            _objectApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      _objectApi.clear ();

      ObjectModule *objMod = get_object_module ();

      if (objMod) {

         HandleContainerIterator it;
         Handle object (0);

         while (_localObjects.get_next (it, object)) {

            objMod->destroy_object (object);
         }

         _localObjects.clear ();

         _obsTable.empty ();

         deactivate_all_object_attributes ();
         activate_object_attribute (_hilAttr, ObjectFlagMask);
      }

      _v8Context.Clear ();
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

   v8::HandleScope scope;
   const int Argc (4);
   V8Value argv[Argc];
   argv[0] = v8::Integer::New (ObjectHandle);
   if (_runtime) { argv[1] = _runtime->create_v8_object_type (&Type); }
   else { argv[1] = v8::Undefined (); }
   argv[2] = v8::Integer::New ((Int32)Locality);
   _process_callback (&_createTable, Argc, argv);
}


void
dmz::JsExtV8Object::destroy_object (
      const UUID &Identity,
      const Handle ObjectHandle) {

   v8::HandleScope scope;
   const int Argc (2);
   V8Value argv[Argc];
   argv[0] = v8::Integer::New (ObjectHandle);
   _process_callback (&_destroyTable, Argc, argv);
}


void
dmz::JsExtV8Object::update_object_uuid (
      const Handle ObjectHandle,
      const UUID &Identity,
      const UUID &PrevIdentity) {

   v8::HandleScope scope;
   const int Argc (4);
   V8Value argv[Argc];
   argv[0] = v8::Integer::New (ObjectHandle);
   argv[1] = v8::String::New (Identity.to_string (UUID::NotFormatted).get_buffer ());
   argv[2] = v8::String::New (PrevIdentity.to_string (UUID::NotFormatted).get_buffer ());
   _process_callback (&_uuidTable, Argc, argv);
}


void
dmz::JsExtV8Object::remove_object_attribute (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Mask &AttrMask) {

   v8::HandleScope scope;
}


void
dmz::JsExtV8Object::update_object_locality (
      const UUID &Identity,
      const Handle ObjectHandle,
      const ObjectLocalityEnum Locality,
      const ObjectLocalityEnum PrevLocality) {

   v8::HandleScope scope;
   const int Argc (4);
   V8Value argv[Argc];
   argv[0] = v8::Integer::New (ObjectHandle);
   argv[1] = v8::Integer::New ((UInt32)Locality);
   argv[2] = v8::Integer::New ((UInt32)PrevLocality);

   _process_callback (&_localityTable, Argc, argv);
}


void
dmz::JsExtV8Object::link_objects (
      const Handle LinkHandle,
      const Handle AttributeHandle,
      const UUID &SuperIdentity,
      const Handle SuperHandle,
      const UUID &SubIdentity,
      const Handle SubHandle) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::New (LinkHandle);
   argv[1] = v8::Integer::New (AttributeHandle);
   argv[2] = v8::Integer::New (SuperHandle);
   argv[3] = v8::Integer::New (SubHandle);

   _process_callback (_linkTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::unlink_objects (
      const Handle LinkHandle,
      const Handle AttributeHandle,
      const UUID &SuperIdentity,
      const Handle SuperHandle,
      const UUID &SubIdentity,
      const Handle SubHandle) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::New (LinkHandle);
   argv[1] = v8::Integer::New (AttributeHandle);
   argv[2] = v8::Integer::New (SuperHandle);
   argv[3] = v8::Integer::New (SubHandle);

   _process_callback (_unlinkTable.lookup (AttributeHandle), Argc, argv);
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

   v8::HandleScope scope;
   const int Argc (7);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (LinkHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::Integer::NewFromUnsigned (SuperHandle);
   argv[3] = v8::Integer::NewFromUnsigned (SubHandle);
   argv[4] = v8::Integer::NewFromUnsigned (AttributeObjectHandle);
   argv[5] = v8::Integer::NewFromUnsigned (PrevAttributeObjectHandle);

   _process_callback (_linkAttrTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_counter (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::Integer::New ((Int32)Value);
   if (PreviousValue) { argv[3] = v8::Integer::New ((Int32)*PreviousValue); }
   else { argv[3] = v8::Undefined (); }

   _process_callback (_counterTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_counter_minimum (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::Integer::New ((Int32)Value);
   if (PreviousValue) { argv[3] = v8::Integer::New ((Int32)*PreviousValue); }
   else { argv[3] = v8::Undefined (); }

   _process_callback (_minCounterTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_counter_maximum (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::Integer::New ((Int32)Value);
   if (PreviousValue) { argv[3] = v8::Integer::New ((Int32)*PreviousValue); }
   else { argv[3] = v8::Undefined (); }

   _process_callback (_maxCounterTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_alternate_type (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const ObjectType &Value,
      const ObjectType *PreviousValue) {

   v8::HandleScope scope;
}


void
dmz::JsExtV8Object::update_object_state (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Mask &Value,
      const Mask *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_types) {

      argv[2] = _types->to_v8_mask (Value);
      if (PreviousValue) { argv[3] = _types->to_v8_mask (*PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_stateTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_flag (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Boolean Value,
      const Boolean *PreviousValue) {

   if (AttributeHandle == _hilAttr) {

      if (Value) { _hil = ObjectHandle; }
      else if (ObjectHandle == _hil) { _hil = 0; }
   }

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::Boolean::New (Value);
   if (PreviousValue) { argv[3] = v8::Boolean::New (*PreviousValue); }
   else { argv[3] = v8::Undefined (); }

   _process_callback (_flagTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_time_stamp (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Float64 Value,
      const Float64 *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::Number::New (Value);
   if (PreviousValue) { argv[3] = v8::Number::New (*PreviousValue); }
   else { argv[3] = v8::Undefined (); }

   _process_callback (_timeStampTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_position (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_types) {

      argv[2] = _types->to_v8_vector (Value);
      if (PreviousValue) { argv[3] = _types->to_v8_vector (*PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_positionTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_orientation (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Matrix &Value,
      const Matrix *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_types) {

      argv[2] = _types->to_v8_matrix (Value);
      if (PreviousValue) { argv[3] = _types->to_v8_matrix (*PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_orientationTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_velocity (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_types) {

      argv[2] = _types->to_v8_vector (Value);
      if (PreviousValue) { argv[3] = _types->to_v8_vector (*PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_velocityTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_acceleration (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_types) {

      argv[2] = _types->to_v8_vector (Value);
      if (PreviousValue) { argv[3] = _types->to_v8_vector (*PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_accelerationTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_scale (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_types) {

      argv[2] = _types->to_v8_vector (Value);
      if (PreviousValue) { argv[3] = _types->to_v8_vector (*PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_scaleTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_vector (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_types) {

      argv[2] = _types->to_v8_vector (Value);
      if (PreviousValue) { argv[3] = _types->to_v8_vector (*PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_vectorTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_scalar (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Float64 Value,
      const Float64 *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::Number::New (Value);
   if (PreviousValue) { argv[3] = v8::Number::New (*PreviousValue); }
   else { argv[3] = v8::Undefined (); }

   _process_callback (_scalarTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_text (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const String &Value,
      const String *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);
   argv[2] = v8::String::New (Value ? Value.get_buffer () : "");

   if (PreviousValue) {

      argv[3] = v8::String::New (*PreviousValue ? PreviousValue->get_buffer () : "");
   }
   else { argv[3] = v8::Undefined (); }

   _process_callback (_textTable.lookup (AttributeHandle), Argc, argv);
}


void
dmz::JsExtV8Object::update_object_data (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Data &Value,
      const Data *PreviousValue) {

   v8::HandleScope scope;
   const int Argc (5);
   V8Value argv[Argc];
   argv[0] = v8::Integer::NewFromUnsigned (ObjectHandle);
   argv[1] = v8::Integer::NewFromUnsigned (AttributeHandle);

   if (_runtime) {

      argv[2] = _runtime->create_v8_data (&Value);
      if (PreviousValue) { argv[3] = _runtime->create_v8_data (PreviousValue); }
      else { argv[3] = v8::Undefined (); }
   }
   else {

      argv[2] = v8::Undefined ();
      argv[3] = v8::Undefined ();
   }

   _process_callback (_dataTable.lookup (AttributeHandle), Argc, argv);
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


dmz::JsExtV8Object::CallbackStruct *
dmz::JsExtV8Object::_create_callback (
      const Handle ObsHandle,
      const Handle Attr,
      const Mask &AttrMask,
      HashTableHandleTemplate<CallbackTable> &table, 
      Boolean &doDump) {

   CallbackStruct *result (0);

   CallbackTable *attrTable = table.lookup (Attr);

   if (!attrTable) {

      attrTable = new CallbackTable (Attr, AttrMask);

      if (attrTable && !table.store (Attr, attrTable)) {

         delete attrTable; attrTable = 0;
      }
   }

   if (attrTable && !attrTable->table.lookup (ObsHandle)) {

      doDump = attrTable->table.get_count () != 0;
      result = new CallbackStruct (ObsHandle, attrTable);
      if (result && !result->table) { delete result; result = 0; }
   }

   return result;
}


void
dmz::JsExtV8Object::_process_callback (
      CallbackTable *table,
      int argc,
      v8::Handle<v8::Value> argv[]) {

   v8::HandleScope scope;

   if (table && (_v8Context.IsEmpty () == false)) {

      v8::Context::Scope cscope (_v8Context);

      if (_newCallback) {

         if (table->table.lookup (_newCallback->ObsHandle)) {

            _do_callback (_newCallback, argc, argv);
         }
      }
      else {

         HashTableHandleIterator it;
         CallbackStruct *cb (0);

         while (table->table.get_next (it, cb)) { _do_callback (cb, argc, argv); }
      }

      if (table->table.get_count () == 0) {

         if ((table->Attr != _hilAttr) || (!table->AttrMask.contains (ObjectFlagMask))) {

            deactivate_object_attribute (table->Attr, table->AttrMask);
         }
      }
   }
}


void
dmz::JsExtV8Object::_do_callback (
      CallbackStruct *cb,
      int argc,
      v8::Handle<v8::Value> argv[]) {

   if ((cb->func.IsEmpty () == false) && (cb->self.IsEmpty () == false)) {

      v8::TryCatch tc;

      argv[argc - 1] = cb->self;

      cb->func->Call (cb->self, argc, argv);

      if (tc.HasCaught ()) {

         if (_core) { _core->handle_v8_exception (tc); }

         ObsStruct *os = _obsTable.lookup (cb->ObsHandle);

         if (os) { _remove_callback (*os, cb->func); }
      }
   }
}


void
dmz::JsExtV8Object::_remove_callback (ObsStruct &os, V8Function func) {

   CallbackStruct *prev (0);
   CallbackStruct *current (os.list);

   while (current) {

      if (current->func == func) {

         CallbackStruct *tmp (current);
         current = current->next;

         if (prev) { prev->next = tmp->next; }
         else { os.list = tmp->next; }

         CallbackTable *table = tmp->table;
         delete tmp; tmp = 0;

         if (table && (table->table.get_count () == 0)) {

            if ((table->Attr != _hilAttr) ||
                  (!table->AttrMask.contains (ObjectFlagMask))) {

               deactivate_object_attribute (table->Attr, table->AttrMask);
            }
         }
      }
      else { prev = current; current = current->next; }
   }
}


void
dmz::JsExtV8Object::_init (Config &local) {

   v8::HandleScope scope;

   _defaultAttr = _defs.create_named_handle (ObjectAttributeDefaultName);
   _hilAttr = _defs.create_named_handle (ObjectAttributeHumanInTheLoopName);

   activate_object_attribute (_hilAttr, ObjectFlagMask);

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Constants
   _objectApi.add_constant ("LocalityUnknown", (UInt32)ObjectLocalityUnknown);
   _objectApi.add_constant ("Local", (UInt32)ObjectLocal);
   _objectApi.add_constant ("Remote", (UInt32)ObjectRemote);
   _objectApi.add_constant ("RetainLinks", (UInt32)ObjectRetainLinks);
   _objectApi.add_constant ("IgnoreLinks", (UInt32)ObjectIgnoreLinks);
   _objectApi.add_constant ("SelectNew", (UInt32)ObjectSelectNew);
   _objectApi.add_constant ("SelectAdd", (UInt32)ObjectSelectAdd);
   _objectApi.add_constant ("DefaultAttribute", _defaultAttr);
   _objectApi.add_constant ("HILAttribute", _hilAttr);
   _objectApi.add_constant (
      "LNVAttribute",
      _defs.create_named_handle (ObjectAttributeLastNetworkValueName));
   _objectApi.add_constant (
      "SelectAttribute",
      _defs.create_named_handle (ObjectAttributeSelectName));
   _objectApi.add_constant (
      "HideAttribute",
      _defs.create_named_handle (ObjectAttributeHideName));
   // Functions
   _objectApi.add_function ("hil", _object_hil, _self);
   _objectApi.add_function ("release", _object_release, _self);
   _objectApi.add_function ("isObject", _object_is_object, _self);
   _objectApi.add_function ("isActivated", _object_is_activated, _self);
   _objectApi.add_function ("isLink", _object_is_link, _self);
   _objectApi.add_function ("getObjects", _object_objects, _self);
   _objectApi.add_function ("create", _object_create, _self);
   _objectApi.add_function ("create.observe", _object_create_observe, _self);
   _objectApi.add_function ("activate", _object_activate, _self);
   _objectApi.add_function ("destroy", _object_destroy, _self);
   _objectApi.add_function ("destroy.observe", _object_destroy_observe, _self);
   _objectApi.add_function ("clone", _object_clone, _self);
   _objectApi.add_function ("makePersistent", _object_make_persistent, _self);
   _objectApi.add_function ("type", _object_type, _self);
   _objectApi.add_function ("locality", _object_locality, _self);
   _objectApi.add_function ("locality.observe", _object_locality_observe, _self);
   _objectApi.add_function ("uuid", _object_uuid, _self);
   _objectApi.add_function ("uuid.observe", _object_uuid_observe, _self);
   _objectApi.add_function ("link", _object_link, _self);
   _objectApi.add_function ("link.observe", _object_link_observe, _self);
   _objectApi.add_function ("linkHandle", _object_link_handle, _self);
   _objectApi.add_function ("linkedObjects", _object_linked_objects, _self);
   _objectApi.add_function ("unlink", _object_unlink, _self);
   _objectApi.add_function ("unlink.observe", _object_unlink_observe, _self);
   _objectApi.add_function ("unlinkSuperObjects", _object_unlink_super_objects, _self);
   _objectApi.add_function ("unlinkSubObjects", _object_unlink_sub_objects, _self);
   _objectApi.add_function ("superLinks", _object_super_links, _self);
   _objectApi.add_function ("subLinks", _object_sub_links, _self);
   _objectApi.add_function ("linkAttributeObject", _object_link_attribute_object, _self);
   _objectApi.add_function (
      "linkAttributeObject.observe",
      _object_link_attribute_object_observe,
      _self);
   _objectApi.add_function (
      "attributeObjectLinks",
      _object_attribute_object_links,
      _self);
   _objectApi.add_function ("addToCounter", _object_add_to_counter, _self);
   _objectApi.add_function ("counter", _object_counter, _self);
   _objectApi.add_function ("counter.observe", _object_counter_observe, _self);
   _objectApi.add_function ("counter.remove", _object_counter_remove, _self);
   _objectApi.add_function ("counter.min", _object_counter_min, _self);
   _objectApi.add_function ("counter.min.observe", _object_counter_min_observe, _self);
   _objectApi.add_function ("counter.min.remove", _object_counter_min_remove, _self);
   _objectApi.add_function ("counter.max", _object_counter_max, _self);
   _objectApi.add_function ("counter.max.observe", _object_counter_max_observe, _self);
   _objectApi.add_function ("counter.max.remove", _object_counter_max_remove, _self);
   _objectApi.add_function ("counter.rollover", _object_counter_rollover, _self);
   _objectApi.add_function ("altType", _object_alt_type, _self);
   _objectApi.add_function ("altType.observe", _object_alt_type_observe, _self);
   _objectApi.add_function ("altType.remove", _object_alt_type_remove, _self);
   _objectApi.add_function ("state", _object_state, _self);
   _objectApi.add_function ("state.observe", _object_state_observe, _self);
   _objectApi.add_function ("state.remove", _object_state_remove, _self);
   _objectApi.add_function ("flag", _object_flag, _self);
   _objectApi.add_function ("flag.observe", _object_flag_observe, _self);
   _objectApi.add_function ("flag.remove", _object_flag_remove, _self);
   _objectApi.add_function ("timeStamp", _object_time_stamp, _self);
   _objectApi.add_function ("timeStamp.observe", _object_time_stamp_observe, _self);
   _objectApi.add_function ("timeStamp.remove", _object_time_stamp_remove, _self);
   _objectApi.add_function ("position", _object_position, _self);
   _objectApi.add_function ("position.observe", _object_position_observe, _self);
   _objectApi.add_function ("position.remove", _object_position_remove, _self);
   _objectApi.add_function ("orientation", _object_orientation, _self);
   _objectApi.add_function ("orientation.observe", _object_orientation_observe, _self);
   _objectApi.add_function ("orientation.remove", _object_orientation_remove, _self);
   _objectApi.add_function ("velocity", _object_velocity, _self);
   _objectApi.add_function ("velocity.observe", _object_velocity_observe, _self);
   _objectApi.add_function ("velocity.remove", _object_velocity_remove, _self);
   _objectApi.add_function ("acceleration", _object_acceleration, _self);
   _objectApi.add_function ("acceleration.observe", _object_acceleration_observe, _self);
   _objectApi.add_function ("acceleration.remove", _object_acceleration_remove, _self);
   _objectApi.add_function ("scale", _object_scale, _self);
   _objectApi.add_function ("scale.observe", _object_scale_observe, _self);
   _objectApi.add_function ("scale.remove", _object_scale_remove, _self);
   _objectApi.add_function ("vector", _object_vector, _self);
   _objectApi.add_function ("vector.observe", _object_vector_observe, _self);
   _objectApi.add_function ("vector.remove", _object_vector_remove, _self);
   _objectApi.add_function ("scalar", _object_scalar, _self);
   _objectApi.add_function ("scalar.observe", _object_scalar_observe, _self);
   _objectApi.add_function ("scalar.remove", _object_scalar_remove, _self);
   _objectApi.add_function ("text", _object_text, _self);
	_objectApi.add_function ("text.observe", _object_text_observe, _self);
	_objectApi.add_function ("text.remove", _object_text_remove, _self);
   _objectApi.add_function ("data", _object_data, _self);
   _objectApi.add_function ("data.observe", _object_data_observe, _self);
   _objectApi.add_function ("data.remove", _object_data_remove, _self);
   _objectApi.add_function ("getSelected", _object_selected, _self);
   _objectApi.add_function ("isSelected", _object_is_selected, _self);
   _objectApi.add_function ("select", _object_select, _self);
   _objectApi.add_function ("unselect", _object_unselect, _self);
   _objectApi.add_function ("unselectAll", _object_unselect_all, _self);
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
