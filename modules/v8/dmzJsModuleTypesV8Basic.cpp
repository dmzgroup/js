#include "dmzJsModuleTypesV8Basic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

namespace {

inline static v8::Persistent<v8::Function>
local_to_v8_function (v8::Handle<v8::Object> obj, v8::Handle<v8::String> name) {

   return v8::Persistent<v8::Function>::New (
      v8::Handle<v8::Function>::Cast (obj->Get (name)));
}

};


dmz::JsModuleTypesV8Basic::JsModuleTypesV8Basic (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsModuleTypesV8 (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0) {

   _init (local);
}


dmz::JsModuleTypesV8Basic::~JsModuleTypesV8Basic () {

}


// Plugin Interface
void
dmz::JsModuleTypesV8Basic::update_plugin_state (
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
dmz::JsModuleTypesV8Basic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// JsModuleTypesV8 Interface
v8::Handle<v8::Object>
dmz::JsModuleTypesV8Basic::to_v8_vector (const Vector &Value) {

   v8::HandleScope scope;

   V8Object result;

   if (!_vectorCtor.IsEmpty ()) {

      V8Value argv[] = {
         v8::Number::New (Value.get_x ()),
         v8::Number::New (Value.get_y ()),
         v8::Number::New (Value.get_z ())
      };

      v8::TryCatch tc;

      result = V8Object::Cast (_vectorCtor->Call (_vector, 3, argv));

      if (tc.HasCaught () && _core) { _core->handle_v8_exception (tc); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Vector
dmz::JsModuleTypesV8Basic::to_dmz_vector (const v8::Handle<v8::Value> Value) {

   v8::HandleScope scope;

   Vector result;

   V8Object obj = V8Object::Cast (Value);

   if (!obj.IsEmpty ()) {

      Float64 theX = 0.0, theY = 0.0, theZ = 0.0;

      V8Number xnum = v8::Handle<v8::Number>::Cast (obj->Get (_xStr));
      if (!xnum.IsEmpty ()) { theX = xnum->Value (); }

      V8Number ynum = v8::Handle<v8::Number>::Cast (obj->Get (_yStr));
      if (!ynum.IsEmpty ()) { theY = ynum->Value (); }

      V8Number znum = v8::Handle<v8::Number>::Cast (obj->Get (_zStr));
      if (!znum.IsEmpty ()) { theZ = znum->Value (); }

      result.set_xyz (theX, theY, theZ);
   }

   return result;
}


v8::Handle<v8::Object>
dmz::JsModuleTypesV8Basic::to_v8_matrix (const Matrix &Value) {

   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (!_matrixCtor.IsEmpty ()) {

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

      result = V8Object::Cast (_matrixCtor->Call (_matrix, 9, argv));

      if (tc.HasCaught () && _core) { _core->handle_v8_exception (tc); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Matrix
dmz::JsModuleTypesV8Basic::to_dmz_matrix (const v8::Handle<v8::Value> Value) {

   v8::HandleScope scope;

   Matrix result;

   V8Object obj = V8Object::Cast (Value);

   if (!obj.IsEmpty ()) {

      V8Array array = v8::Handle<v8::Array>::Cast (obj->Get (_vStr));

      if (!array.IsEmpty ()) {

         const uint32_t Length = array->Length ();

         Float64 mat[9];

         for (Int32 ix = 0; ix < 9; ix++) {

            V8Number element = V8Number::Cast (
               array->Get (v8::Integer::New (ix)));

            mat[ix] = element.IsEmpty () ? 0.0 : element->Value ();
         }

         result.from_array (mat);
      }
   }

   return result;
}


v8::Handle<v8::Object>
dmz::JsModuleTypesV8Basic::to_v8_mask (const Mask &Value) {

   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (!_maskCtor.IsEmpty ()) {

      const Int32 Length = Value.get_size ();

      V8Array array = v8::Array::New (Length);

      for (Int32 ix = 0; ix < Length; ix++) {

         array->Set (
            v8::Integer::New (ix),
            v8::Integer::NewFromUnsigned (Value.get_sub_mask (ix)));
      }

      V8Value argv[] = { array };

      v8::TryCatch tc;

      result = V8Object::Cast (_maskCtor->Call (_mask, 1, argv));

      if (tc.HasCaught () && _core) { _core->handle_v8_exception (tc); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Mask
dmz::JsModuleTypesV8Basic::to_dmz_mask (const v8::Handle<v8::Value> Value) {

   v8::HandleScope scope;

   Mask result;

   V8Object obj = V8Object::Cast (Value);

   if (!obj.IsEmpty ()) {

      V8Array bits = V8Array::Cast (obj->Get (_bitsStr));

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
dmz::JsModuleTypesV8Basic::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) {

      if (!_core) { _core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {

      if (_core && (_core == &module)) { _core = 0; _clear (); } 
   }
}


void
dmz::JsModuleTypesV8Basic::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Init) {

      _create_symbols ();

      if (_core) {

         _mask = v8::Persistent<v8::Object>::New (_core->require ("dmz/types/mask"));
         _matrix = v8::Persistent<v8::Object>::New (_core->require ("dmz/types/matrix"));
         _vector = v8::Persistent<v8::Object>::New (_core->require ("dmz/types/vector"));

         v8::Handle<v8::String> create = v8::String::NewSymbol ("create");

         _maskCtor = local_to_v8_function (_mask, create);
         _matrixCtor = local_to_v8_function (_matrix, create);
         _vectorCtor = local_to_v8_function (_vector, create);
      }
   }
}


// JsModuleTypesV8Basic Interface
void
dmz::JsModuleTypesV8Basic::_clear () {

   _mask.Dispose (); _mask.Clear ();
   _maskCtor.Dispose (); _maskCtor.Clear ();
   _matrix.Dispose (); _matrix.Clear ();
   _matrixCtor.Dispose (); _matrixCtor.Clear ();
   _vector.Dispose (); _vector.Clear ();
   _vectorCtor.Dispose (); _vectorCtor.Clear ();

   _bitsStr.Dispose (); _bitsStr.Clear ();

   _vStr.Dispose (); _vStr.Clear ();

   _xStr.Dispose (); _xStr.Clear ();
   _yStr.Dispose (); _yStr.Clear ();
   _zStr.Dispose (); _zStr.Clear ();
}


void
dmz::JsModuleTypesV8Basic::_create_symbols () {

   _clear ();

   _bitsStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("bits"));
   _vStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("v"));
   _xStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("x"));
   _yStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("y"));
   _zStr = v8::Persistent<v8::String>::New (v8::String::NewSymbol ("z"));
}


void
dmz::JsModuleTypesV8Basic::_init (Config &local) {

}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleTypesV8Basic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleTypesV8Basic (Info, local);
}

};
