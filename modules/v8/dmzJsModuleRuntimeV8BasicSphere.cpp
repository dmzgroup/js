#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>

using namespace dmz;

namespace {

void
local_sphere_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      Sphere *ptr = (Sphere *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};


V8Value
dmz::JsModuleRuntimeV8Basic::_sphere_origin (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Sphere *sph = self->_to_sphere_ptr (Args.This ());

      if (sph) {

         if (Args.Length ()) {

            if (self->_types->is_v8_vector (Args[0])) {

               sph->set_origin (self->_types->to_dmz_vector (Args[0]));
            }
            else if (Args[0]->IsArray ()) {

               V8Array array = v8_to_array (Args[0]);
               if (array->Length () >= 3) {

                  Float32 x = v8_to_number (array->Get (v8::Number::New (0)));
                  Float32 y = v8_to_number (array->Get (v8::Number::New (1)));
                  Float32 z = v8_to_number (array->Get (v8::Number::New (2)));
                  Vector v (x, y, z);
                  sph->set_origin (v);
               }
            }
         }
         result = self->_types->to_v8_vector (sph->get_origin ());
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_sphere_radius (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Sphere *sph = self->_to_sphere_ptr (Args.This ());

      if (sph) {

         if (Args.Length ()) {

            sph->set_radius (v8_to_number (Args[0]));
         }

         result = v8::Number::New (sph->get_radius ());
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_sphere_contains_point (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Boolean::New (false);

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Sphere *sph = self->_to_sphere_ptr (Args.This ());

      if (sph) {

         if (Args.Length ()) {

            if (self->_types->is_v8_vector (Args[0])) {

               if (sph->contains_point (self->_types->to_dmz_vector (Args[0]))) {

                  result = v8::Boolean::New (true);
               }
            }
            else if (Args[0]->IsArray ()) {

               V8Array array = v8_to_array (Args[0]);
               if (array->Length () >= 3) {

                  Float32 x = v8_to_number (array->Get (v8::Number::New (0)));
                  Float32 y = v8_to_number (array->Get (v8::Number::New (1)));
                  Float32 z = v8_to_number (array->Get (v8::Number::New (2)));
                  Vector v (x, y, z);
                  if (sph->contains_point (v)) { result = v8::Boolean::New (true); }
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsModuleRuntimeV8Basic::_init_sphere () {

   v8::HandleScope scope;

   _sphereTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _sphereTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _sphereTemplate->PrototypeTemplate ();

   proto->Set ("origin", v8::FunctionTemplate::New (_sphere_origin, _self));
   proto->Set ("radius", v8::FunctionTemplate::New (_sphere_radius, _self));
   proto->Set (
      "containsPoint",
      v8::FunctionTemplate::New (_sphere_contains_point, _self));

   _sphereApi.add_function ("create", _sphere_create, _self);

}


V8Value
dmz::JsModuleRuntimeV8Basic::_sphere_create (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Vector v;
      Float32 r (-1);
      Sphere *sph (0);

      if (Args.Length () > 1) {

         if (self->_types->is_v8_vector (Args[0])) {

            v = self->_types->to_dmz_vector (Args[0]);
         }
         else if (Args[0]->IsArray ()) {

            V8Array array = v8_to_array (Args[0]);
            if (array->Length () >= 3) {

               Float32 x = v8_to_number (array->Get (v8::Number::New (0)));
               Float32 y = v8_to_number (array->Get (v8::Number::New (1)));
               Float32 z = v8_to_number (array->Get (v8::Number::New (2)));
               v = *(new  Vector (x, y, z));
            }
         }

         r = v8_to_number (Args[1], -1);

      }

      if ((r > 0) && (!v.is_zero ())) { sph = new Sphere (v, r); }
      else { sph = new Sphere (); }

      result = self->create_v8_sphere (sph);
   }
   return scope.Close (result);
}

v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_sphere (const Sphere *Value) {

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (_sphereFunc.IsEmpty () == false) {

      result = _sphereFunc->NewInstance ();

      if (!result.IsEmpty ()) {

         Sphere *ptr (0);
         if (Value) { ptr = new Sphere (*Value); }
         else { ptr = new Sphere (); }
         //if (ptr) { ptr->set_runtime_context (get_plugin_runtime_context ()); }

         result->SetInternalField (0, v8::External::Wrap ((void *)ptr));

         v8::Persistent<v8::Object> persist = v8::Persistent<v8::Object>::New (result);
         persist.MakeWeak ((void *)ptr, local_sphere_delete);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Sphere *
dmz::JsModuleRuntimeV8Basic::to_dmz_sphere (v8::Handle<v8::Value> value) {

   Sphere *result (0);
   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   if (!value.IsEmpty ()) { result = _to_sphere_ptr (value); }

   return result;
}


dmz::Sphere *
dmz::JsModuleRuntimeV8Basic::_to_sphere_ptr (V8Value value) {

   v8::HandleScope scope;

   Sphere *result (0);

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_sphereTemplate->HasInstance (obj)) {

         result = (Sphere *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}
