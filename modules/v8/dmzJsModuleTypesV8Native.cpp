#include "dmzJsModuleTypesV8Native.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

#include <qdb.h>
static dmz::qdb out;

namespace {

inline static v8::Persistent<v8::Function>
local_to_v8_function (v8::Handle<v8::Object> obj, v8::Handle<v8::String> name) {

   dmz::V8FunctionPersist result;

   if (obj.IsEmpty () == false) {

      dmz::V8Value func = obj->Get (name);

      if ((func.IsEmpty () == false) && func->IsFunction ()) {

         result = dmz::V8FunctionPersist::New (dmz::v8_to_function (func));
      }
   }

   return result;
}

void
local_delete_vector (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      dmz::Vector *ptr = (dmz::Vector *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};


dmz::JsModuleTypesV8Native::JsModuleTypesV8Native (
      const PluginInfo &Info,
      Config &local) :
      Plugin (Info),
      JsModuleTypesV8 (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0) {

   _init (local);
}


dmz::JsModuleTypesV8Native::~JsModuleTypesV8Native () {

}


// Plugin Interface
void
dmz::JsModuleTypesV8Native::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

      _clear ();
   }
}


void
dmz::JsModuleTypesV8Native::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// JsModuleTypesV8 Interface
v8::Handle<v8::Object>
dmz::JsModuleTypesV8Native::to_v8_vector (const Vector &Value) {

   v8::HandleScope scope;

   V8Object result = _vectorCtor->NewInstance ();

   result->SetPointerInInternalField (0, new Vector (Value));

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Vector
dmz::JsModuleTypesV8Native::to_dmz_vector (const v8::Handle<v8::Value> Value) {

   return _to_vector (Value);
}


v8::Handle<v8::Object>
dmz::JsModuleTypesV8Native::to_v8_matrix (const Matrix &Value) {

   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (_matrixCtor.IsEmpty () == false) {

      Float64 array[9];

      Value.to_array (array);

      V8Value argv[] = {
         v8::Number::New (array[0]),
         v8::Number::New (array[1]),
         v8::Number::New (array[2]),
         v8::Number::New (array[3]),
         v8::Number::New (array[4]),
         v8::Number::New (array[5]),
         v8::Number::New (array[6]),
         v8::Number::New (array[7]),
         v8::Number::New (array[8]),
      };

      v8::TryCatch tc;

      result = v8_to_object (_matrixCtor->Call (_matrix, 9, argv));

      if (tc.HasCaught () && _core) { _core->handle_v8_exception (tc); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Matrix
dmz::JsModuleTypesV8Native::to_dmz_matrix (const v8::Handle<v8::Value> Value) {

   v8::HandleScope scope;

   Matrix result;

   V8Object obj = v8_to_object (Value);

   if (!obj.IsEmpty ()) {

      V8Array array = v8_to_array (Value->IsArray () ? Value : obj->Get (_vStr));

      if (!array.IsEmpty ()) {

         const uint32_t Length = array->Length ();

         Float64 mat[9];

         for (Int32 ix = 0; ix < 9; ix++) {

            mat[ix] = v8_to_number (array->Get (v8::Integer::New (ix)));
         }

         result.from_array (mat);
      }
   }

   return result;
}


v8::Handle<v8::Object>
dmz::JsModuleTypesV8Native::to_v8_mask (const Mask &Value) {

   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (_maskCtor.IsEmpty () == false) {

      const Int32 Length = Value.get_size ();

      V8Array array = v8::Array::New (Length);

      for (Int32 ix = 0; ix < Length; ix++) {

         array->Set (
            v8::Integer::New (ix),
            v8::Integer::NewFromUnsigned (Value.get_sub_mask (ix)));
      }

      V8Value argv[] = { array };

      v8::TryCatch tc;

      result = v8_to_object (_maskCtor->Call (_mask, 1, argv));

      if (tc.HasCaught () && _core) { _core->handle_v8_exception (tc); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Mask
dmz::JsModuleTypesV8Native::to_dmz_mask (const v8::Handle<v8::Value> Value) {

   v8::HandleScope scope;

   Mask result;

   V8Object obj = v8_to_object (Value);

   if (!obj.IsEmpty ()) {

      V8Array bits = v8_to_array (obj->Get (_bitsStr));

      if (!bits.IsEmpty ()) {

         const Int32 Length = (Int32)bits->Length ();

         result.grow (Length);

         for (Int32 ix = 0; ix < Length; ix++) {

            V8Value element = bits->Get (v8::Integer::New (ix));

            if (!element.IsEmpty ()) {

               result.set_sub_mask (ix, element->Uint32Value ());
            }
         }
      }
   }

   return result;
}


// JsExtV8 Interface
void
dmz::JsModuleTypesV8Native::update_js_module_v8 (
      const ModeEnum Mode,
      JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) {

      if (!_core) { _core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {

      if (_core && (_core == &module)) { _core = 0; _clear (); } 
   }
}


void
dmz::JsModuleTypesV8Native::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/types/vector",
            _vectorApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _create_symbols ();

      if (_core) {

         _mask = v8::Persistent<v8::Object>::New (_core->require ("dmz/types/mask"));
         _matrix = v8::Persistent<v8::Object>::New (_core->require ("dmz/types/matrix"));

         v8::Handle<v8::String> create = v8::String::NewSymbol ("create");
         _maskCtor = local_to_v8_function (_mask, create);
         _matrixCtor = local_to_v8_function (_matrix, create);

         _vectorCtor = V8FunctionPersist::New (_vectorTemplate->GetFunction ());
      }
   }
   else if (State == JsExtV8::Shutdown) { _clear (); }
}


void
dmz::JsModuleTypesV8Native::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

}


// JsModuleTypesV8Native Interface


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_create (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsModuleTypesV8Native *self = _to_self (Args);

   if (self) {

      V8Object vec = self->_vectorCtor->NewInstance ();
      Vector *ptr = new Vector;
      vec->SetPointerInInternalField (0, (void *)ptr);

      V8ObjectPersist weak = V8ObjectPersist::New (vec);
      weak.MakeWeak ((void *)ptr, local_delete_vector);

      result = vec;

      const int Length = Args.Length ();

      if (Length > 0) {

         if (Length == 1) {

            V8Value arg = Args[0];

            if (self->_vectorTemplate->HasInstance (arg)) {

               Vector *value = _to_vector (arg->ToObject ());

               if (value) { *ptr = * value; }
            }
            else if (arg->IsArray ()) {

               V8Object array = arg->ToObject ();

               ptr->set_xyz (
                  v8_to_number (array->Get (v8::Integer::New (0))),
                  v8_to_number (array->Get (v8::Integer::New (1))),
                  v8_to_number (array->Get (v8::Integer::New (2))));
            }
         }
         else if (Length == 3) {

            ptr->set_xyz (
               v8_to_number (Args[0]),
               v8_to_number (Args[1]),
               v8_to_number (Args[2]));
         }
      }
   }

   return scope.Close (result);
}

dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_get_x (
      v8::Local<v8::String> property,
      const v8::AccessorInfo &Info) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Info.This ());

   if (ptr) { result = v8::Number::New (ptr->get_x ()); }

   return scope.Close (result);
}


void
dmz::JsModuleTypesV8Native::_vector_set_x (
      v8::Local<v8::String> property,
      v8::Local<v8::Value> value,
      const v8::AccessorInfo &Info) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Info.This ());

   if (ptr) { ptr->set_x (v8_to_number (value)); }
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_get_y (
      v8::Local<v8::String> property,
      const v8::AccessorInfo &Info) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Info.This ());

   if (ptr) { result = v8::Number::New (ptr->get_y ()); }

   return scope.Close (result);
}


void
dmz::JsModuleTypesV8Native::_vector_set_y (
      v8::Local<v8::String> property,
      v8::Local<v8::Value> value,
      const v8::AccessorInfo &Info) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Info.This ());

   if (ptr) { ptr->set_y (v8_to_number (value)); }
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_get_z (
      v8::Local<v8::String> property,
      const v8::AccessorInfo &Info) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Info.This ());

   if (ptr) { result = v8::Number::New (ptr->get_z ()); }

   return scope.Close (result);
}


void
dmz::JsModuleTypesV8Native::_vector_set_z (
      v8::Local<v8::String> property,
      v8::Local<v8::Value> value,
      const v8::AccessorInfo &Info) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Info.This ());

   if (ptr) { ptr->set_z (v8_to_number (value)); }
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_copy (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) {

      result = self->to_v8_vector (*ptr);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_to_string (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());

   if (ptr) {

      String value ("[");
      value << ptr->get_x () << ", " << ptr->get_y () << ", " << ptr->get_z () << "]";
      result = v8::String::New (value.get_buffer ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_from_array (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());

   if (ptr) {

      if (Args[0]->IsArray ()) {

         V8Object array = Args[0]->ToObject ();

         ptr->set_xyz (
            v8_to_number (array->Get (v8::Integer::New (0))),
            v8_to_number (array->Get (v8::Integer::New (1))),
            v8_to_number (array->Get (v8::Integer::New (2))));

         result = Args.This ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_to_array (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());

   if (ptr) {

      V8Array array = v8::Array::New (3);

      array->Set (v8::Integer::New (0), v8::Number::New (ptr->get_x ()));
      array->Set (v8::Integer::New (1), v8::Number::New (ptr->get_y ()));
      array->Set (v8::Integer::New (2), v8::Number::New (ptr->get_z ()));

      result = array;
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_set (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) {

      const int Length = Args.Length ();

      if (Length == 1) {

         V8Value arg = Args[0];

         if (self->_vectorTemplate->HasInstance (arg)) {

            Vector *value = _to_vector (arg->ToObject ());

            if (value) { *ptr = * value; }
         }
         else if (arg->IsArray ()) {

            V8Object array = arg->ToObject ();

            ptr->set_xyz (
               v8_to_number (array->Get (v8::Integer::New (0))),
               v8_to_number (array->Get (v8::Integer::New (1))),
               v8_to_number (array->Get (v8::Integer::New (2))));
         }
      }
      else if (Length == 3) {

         ptr->set_xyz (
            v8_to_number (Args[0]),
            v8_to_number (Args[1]),
            v8_to_number (Args[2]));
      }

      result = Args.This ();
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_set_xyz (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());

   if (ptr) {

      ptr->set_xyz (
         v8_to_number (Args[0]),
         v8_to_number (Args[1]),
         v8_to_number (Args[2]));

      result = Args.This ();
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_magnitude (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());

   if (ptr) { result = v8::Number::New (ptr->magnitude ()); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_normalize (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) { result = self->to_v8_vector (ptr->normalize ()); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_add (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) { result = self->to_v8_vector (*ptr + self->_to_vector (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_subtract (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) { result = self->to_v8_vector (*ptr - self->_to_vector (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_multiply_const (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) { result = self->to_v8_vector (*ptr * v8_to_number (Args[0])); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_dot (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) { result = v8::Number::New (ptr->dot (self->_to_vector (Args[0]))); }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_cross (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) {

      result = self->to_v8_vector (ptr->cross (self->_to_vector (Args[0])));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleTypesV8Native::_vector_get_angle (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   Vector *ptr = _to_vector (Args.This ());
   JsModuleTypesV8Native *self = _to_self (Args);

   if (ptr && self) {

      result = v8::Number::New (ptr->get_angle (self->_to_vector (Args[0])));
   }

   return scope.Close (result);
}


dmz::Vector
dmz::JsModuleTypesV8Native::_to_vector (const V8Value &Value) {

   v8::HandleScope scope;

   Vector result;

   if (_vectorTemplate->HasInstance (Value)) {

      Vector *ptr = (Vector *)Value->ToObject ()->GetPointerFromInternalField (0);

      if (ptr) { result = *ptr; }
   }
   else if (Value->IsArray ()) {

      V8Object array = Value->ToObject ();

      result.set_xyz (
         v8_to_number (array->Get (v8::Integer::New (0))),
         v8_to_number (array->Get (v8::Integer::New (1))),
         v8_to_number (array->Get (v8::Integer::New (2))));
   }

   return result;
}


void
dmz::JsModuleTypesV8Native::_clear () {

   _vectorApi.clear ();

   _vectorCtor.Dispose (); _vectorCtor.Clear ();

   _mask.Dispose (); _mask.Clear ();
   _maskCtor.Dispose (); _maskCtor.Clear ();
   _matrix.Dispose (); _matrix.Clear ();
   _matrixCtor.Dispose (); _matrixCtor.Clear ();

   _bitsStr.Dispose (); _bitsStr.Clear ();

   _vStr.Dispose (); _vStr.Clear ();

   _xStr.Dispose (); _xStr.Clear ();
   _yStr.Dispose (); _yStr.Clear ();
   _zStr.Dispose (); _zStr.Clear ();
}


void
dmz::JsModuleTypesV8Native::_create_symbols () {

   _bitsStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("bits"));
   _vStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("v"));
   _xStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("x"));
   _yStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("y"));
   _zStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("z"));
}


void
dmz::JsModuleTypesV8Native::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   _vectorApi.add_function ("create", _vector_create, _self);

   _vectorTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   V8ObjectTemplate instance = _vectorTemplate->InstanceTemplate ();
   instance->SetInternalFieldCount (1);
   V8ObjectTemplate proto = _vectorTemplate->PrototypeTemplate ();

   proto->SetAccessor (v8::String::NewSymbol ("x"), _vector_get_x, _vector_set_x);
   proto->SetAccessor (v8::String::NewSymbol ("y"), _vector_get_y, _vector_set_y);
   proto->SetAccessor (v8::String::NewSymbol ("z"), _vector_get_z, _vector_set_z);

   proto->Set ("copy", v8::FunctionTemplate::New (_vector_copy, _self));
   proto->Set ("toString", v8::FunctionTemplate::New (_vector_to_string, _self));
   proto->Set ("fromArray", v8::FunctionTemplate::New (_vector_from_array, _self));
   proto->Set ("toArray", v8::FunctionTemplate::New (_vector_to_array, _self));
   proto->Set ("set", v8::FunctionTemplate::New (_vector_set, _self));
   proto->Set ("setXYZ", v8::FunctionTemplate::New (_vector_set_xyz, _self));
   proto->Set ("magnitude", v8::FunctionTemplate::New (_vector_magnitude, _self));
   proto->Set ("normalize", v8::FunctionTemplate::New (_vector_normalize, _self));
   proto->Set ("add", v8::FunctionTemplate::New (_vector_add, _self));
   proto->Set ("subtract", v8::FunctionTemplate::New (_vector_subtract, _self));
   proto->Set (
      "multiplyConst",
      v8::FunctionTemplate::New (_vector_multiply_const, _self));
   proto->Set ("dot", v8::FunctionTemplate::New (_vector_dot, _self));
   proto->Set ("cross", v8::FunctionTemplate::New (_vector_cross, _self));
   proto->Set ("getAngle", v8::FunctionTemplate::New (_vector_get_angle, _self));
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleTypesV8Native (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleTypesV8Native (Info, local);
}

};
