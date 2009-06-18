#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilTypes.h>

#include <qdb.h>

static dmz::qdb out;

namespace {

static bool
validate_root (dmz::V8Object &obj) {

   bool result = false;

   if (obj.IsEmpty ()) {

      v8::Handle<v8::Context> context = v8::Context::GetCalling ();

      if (context.IsEmpty () == false) {

         dmz::V8Object global = context->Global ();

         if (global.IsEmpty () == false) {

            dmz::V8Value value = global->Get (v8::String::New ("DMZ"));

            if (dmz::is_object (value)) {

               obj = dmz::V8Object::Cast (value);
               result = true;
            }
         }
      }
   }
   else { result = true; }

   return result;
}

};


dmz::V8Value
dmz::to_v8_uuid (const UUID &Value) {

   v8::HandleScope scope;

   return scope.Close (v8::String::New (Value.to_string ().get_buffer ()));
}


dmz::UUID
dmz::to_dmz_uuid (V8Value value) {

   v8::HandleScope scope;

   return UUID (to_string (value));
}


dmz::V8Value
dmz::to_v8_vector (const Vector &Value, V8Object root) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (validate_root (root)) {

      V8Value vecValue = root->Get (v8::String::New ("Vector"));

      if (is_function (vecValue)) {

         V8Function function = V8Function::Cast (vecValue);

         if (function.IsEmpty () == false) {

            result = function->NewInstance ();
         }

         if (is_object (result)) {

            V8Object vec = result->ToObject ();

            if (vec.IsEmpty () == false) {

               vec->Set (v8::String::New ("x"), v8::Number::New (Value.get_x ()));
               vec->Set (v8::String::New ("y"), v8::Number::New (Value.get_y ()));
               vec->Set (v8::String::New ("z"), v8::Number::New (Value.get_z ()));
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::Vector
dmz::to_dmz_vector (V8Value value) {

   v8::HandleScope scope;

   Vector result;

   Float64 xx = 0.0, yy = 0.0, zz = 0.0;

   if (value.IsEmpty () == false) {

      V8Object vec = value->ToObject ();

      if (value->IsArray ()) {

          xx = vec->Get (v8::Integer::New (0))->NumberValue ();
          yy = vec->Get (v8::Integer::New (1))->NumberValue ();
          zz = vec->Get (v8::Integer::New (2))->NumberValue ();
      }
      else {

          xx = vec->Get (v8::String::New ("x"))->NumberValue ();
          yy = vec->Get (v8::String::New ("y"))->NumberValue ();
          zz = vec->Get (v8::String::New ("z"))->NumberValue ();
      }
   }

   result.set_xyz (xx, yy, zz);

   return result;
}


dmz::V8Value
dmz::to_v8_matrix (const Matrix &Value, V8Object root) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (validate_root (root)) {

      V8Value matVal = root->Get (v8::String::New ("Matrix"));

      if (is_function (matVal)) {

         V8Function function = V8Function::Cast (matVal);

         if (function.IsEmpty () == false) { result = function->NewInstance (); }
      }

      if (is_object (result)) {

         V8Object mat = result->ToObject ();

         if (mat.IsEmpty () == false) {

            V8Value arrayValue = mat->Get (v8::String::New ("v"));

            if (is_array (arrayValue)) {

               V8Array array = V8Array::Cast (arrayValue);

               if (array.IsEmpty () == false) {

                  Float64 v[] = {
                     1.0, 0.0, 0.0,
                     0.0, 1.0, 0.0,
                     0.0, 0.0, 1.0 };

                  Value.to_array (v);

                  array->Set (v8::Integer::New (0), v8::Number::New (v[0]));
                  array->Set (v8::Integer::New (1), v8::Number::New (v[1]));
                  array->Set (v8::Integer::New (2), v8::Number::New (v[2]));
                  array->Set (v8::Integer::New (3), v8::Number::New (v[3]));
                  array->Set (v8::Integer::New (4), v8::Number::New (v[4]));
                  array->Set (v8::Integer::New (5), v8::Number::New (v[5]));
                  array->Set (v8::Integer::New (6), v8::Number::New (v[6]));
                  array->Set (v8::Integer::New (7), v8::Number::New (v[7]));
                  array->Set (v8::Integer::New (8), v8::Number::New (v[8]));
               }
            }
         }
      }
      else { result = v8::Undefined (); }
   }

   return scope.Close (result);
}


dmz::Matrix
dmz::to_dmz_matrix (V8Value value) {

   v8::HandleScope scope;

   Matrix result;

   Float64 v[] = {
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0 };

   if (value.IsEmpty () == false) {

      V8Object mat;

      if (value->IsArray ()) {

          mat = value->ToObject ();
      }
      else if (value->IsObject ()) {

         V8Object obj = value->ToObject ();

         if (obj.IsEmpty () == false) {

            V8Value array = obj->Get (v8::String::New ("v"));

            if (is_object (array)) {

               mat = array->ToObject ();
            }
         }
      }

      if (mat.IsEmpty () == false) {

          v[0] = to_number (mat->Get (v8::Integer::New (0)));
          v[1] = to_number (mat->Get (v8::Integer::New (1)));
          v[2] = to_number (mat->Get (v8::Integer::New (2)));
          v[3] = to_number (mat->Get (v8::Integer::New (3)));
          v[4] = to_number (mat->Get (v8::Integer::New (4)));
          v[5] = to_number (mat->Get (v8::Integer::New (5)));
          v[6] = to_number (mat->Get (v8::Integer::New (6)));
          v[7] = to_number (mat->Get (v8::Integer::New (7)));
          v[8] = to_number (mat->Get (v8::Integer::New (8)));
      }
   }

   result.from_array (v);

   return result;
}

