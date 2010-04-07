#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzRuntimeData.h>
#include <dmzSystemStreamString.h>

using namespace dmz;

namespace {

void
local_data_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      Data *ptr = (Data *)param;
      delete ptr; ptr = 0;
   }
}

};


V8Value
dmz::JsModuleRuntimeV8Basic::_create_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args[0]);

      result = self->create_v8_data (ptr);
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_is_type_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) { result = self->_to_data (Args[0]); }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_wrap_boolean (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Boolean Value = v8_to_boolean (Args[0]);
      Data data = self->_convertBool.to_data (Value);
      result = self->create_v8_data (&data);
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_unwrap_boolean (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args[0]);
      if (ptr) { result = v8::Boolean::New (self->_convertBool.to_boolean (*ptr)); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_wrap_number (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Float64 Value = v8_to_number (Args[0]);
      Data data = self->_convertNum.to_data (Value);
      result = self->create_v8_data (&data);
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_unwrap_number (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args[0]);
      if (ptr) { result = v8::Number::New (self->_convertNum.to_float64 (*ptr)); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_wrap_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const String Value = v8_to_string (Args[0]);
      Data data = self->_convertStr.to_data (Value);
      result = self->create_v8_data (&data);
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_unwrap_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args[0]);
      if (ptr) {

         const String Str = self->_convertStr.to_string (*ptr);
         result = v8::String::New (Str ? Str.get_buffer () : "");
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_wrap_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Handle Value = v8_to_handle (Args[0]);
      Data data = self->_convertHandle.to_data (Value);
      result = self->create_v8_data (&data);
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_unwrap_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args[0]);

      if (ptr) {

         const Handle Value = self->_convertHandle.to_handle (*ptr);
         result = v8::Integer::New (Value);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_to_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         String str;
         StreamString out (str);
         out << *ptr;

         if (str) { result = v8::String::New (str.get_buffer ()); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_boolean (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         const Handle Attr = self->_to_handle (Args[0]);
         const Int32 Element = v8_to_int32 (Args[1]);

         if (Attr && (Element >= 0)) {

            if (Args.Length () <= 2) {

               Boolean val (False);

               if (ptr->lookup_boolean (Attr, Element, val)) {

                  result = v8::Boolean::New (val);
               }
            }
            else {

               V8Value val = Args[2];

               if (ptr->store_boolean (Attr, Element, val->BooleanValue ())) {

                  result = val;
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_number (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         const Handle Attr = self->_to_handle (Args[0]);
         const Int32 Element = v8_to_int32 (Args[1]);

         if (Attr && (Element >= 0)) {

            if (Args.Length () <= 2) {

               Float64 num (0.0);

               if (ptr->lookup_float64 (Attr, Element, num)) {

                  result = v8::Number::New (num);
               }
            }
            else {

               V8Value val = Args[2];

               if (ptr->store_float64 (Attr, Element, val->NumberValue ())) {

                  result = val;
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         const Handle Attr = self->_to_handle (Args[0]);
         const Int32 Element = v8_to_int32 (Args[1]);

         if (Attr && (Element >= 0)) {

            if (Args.Length () <= 2) {

               Handle val;

               if (ptr->lookup_handle (Attr, Element, val)) {

                  result = v8::Number::New ((double)val);
               }
            }
            else {

               const Handle Val = self->_to_handle (Args[2]);

               if (Val && ptr->store_handle (Attr, Element, Val)) {

                  result = Args[2];
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         const Handle Attr = self->_to_handle (Args[0]);
         const Int32 Element = v8_to_int32 (Args[1]);

         if (Attr && (Element >= 0)) {

            if (Args.Length () <= 2) {

               String str;

               if (ptr->lookup_string (Attr, Element, str)) {

                  result = v8::String::New (str ? str.get_buffer () : "");
               }
            }
            else {

               String val = v8_to_string (Args[2]);

               if (ptr->store_string (Attr, Element, val)) {

                  result = Args[2];
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_matrix (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_types) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         const Handle Attr = self->_to_handle (Args[0]);
         const Int32 Element = v8_to_int32 (Args[1]);

         if (Attr && (Element >= 0)) {

            if (Args.Length () <= 2) {

               Matrix mat;

               if (ptr->lookup_matrix (Attr, Element, mat)) {

                  result = self->_types->to_v8_matrix (mat);
               }
            }
            else {

               Matrix mat = self->_types->to_dmz_matrix (Args[2]);

               if (ptr->store_matrix (Attr, Element, mat)) {

                  result = self->_types->to_v8_matrix (mat);
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_data_vector (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_types) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         const Handle Attr = self->_to_handle (Args[0]);
         const Int32 Element = v8_to_int32 (Args[1]);

         if (Attr && (Element >= 0)) {

            if (Args.Length () <= 2) {

               Vector vec;

               if (ptr->lookup_vector (Attr, Element, vec)) {

                  result = self->_types->to_v8_vector (vec);
               }
            }
            else {

               Vector vec = self->_types->to_dmz_vector (Args[2]);

               if (ptr->store_vector (Attr, Element, vec)) {

                  result = self->_types->to_v8_vector (vec);
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


// JsModuleRuntimeV8 Interface
v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_data (const Data *Value) {

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (_dataFunc.IsEmpty () == false) {

      result = _dataFunc->NewInstance ();

      if (!result.IsEmpty ()) {

         Data *ptr = new Data (Value ? *Value : 0);

         if (ptr) { ptr->set_runtime_context (get_plugin_runtime_context ()); }

         result->SetInternalField (0, v8::External::Wrap ((void *)ptr));

         v8::Persistent<v8::Object> persist = v8::Persistent<v8::Object>::New (result);
         persist.MakeWeak ((void *)ptr, local_data_delete);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Boolean
dmz::JsModuleRuntimeV8Basic::to_dmz_data (V8Value value, Data &out) {

   Boolean result (False);

   Data *ptr = _to_data_ptr (value);

   if (ptr) { out = *ptr; result = True; }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_init_data () {

   v8::HandleScope scope;

   _dataFuncTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _dataFuncTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _dataFuncTemplate->PrototypeTemplate ();

   proto->Set ("toString", v8::FunctionTemplate::New (_data_to_string, _self));
   proto->Set ("boolean", v8::FunctionTemplate::New (_data_boolean, _self));
   proto->Set ("number", v8::FunctionTemplate::New (_data_number, _self));
   proto->Set ("handle", v8::FunctionTemplate::New (_data_handle, _self));
   proto->Set ("string", v8::FunctionTemplate::New (_data_string, _self));
   proto->Set ("matrix", v8::FunctionTemplate::New (_data_matrix, _self));
   proto->Set ("vector", v8::FunctionTemplate::New (_data_vector, _self));

   _dataApi.add_function ("create", _create_data, _self);
   _dataApi.add_function ("isTypeOf", _data_is_type_of, _self);
   _dataApi.add_function ("wrapBoolean", _data_wrap_boolean, _self);
   _dataApi.add_function ("unwrapBoolean", _data_unwrap_boolean, _self);
   _dataApi.add_function ("wrapNumber", _data_wrap_number, _self);
   _dataApi.add_function ("unwrapNumber", _data_unwrap_number, _self);
   _dataApi.add_function ("wrapString", _data_wrap_string, _self);
   _dataApi.add_function ("unwrapString", _data_unwrap_string, _self);
   _dataApi.add_function ("wrapHandle", _data_wrap_handle, _self);
   _dataApi.add_function ("unwrapHandle", _data_unwrap_handle, _self);
}


V8Object
dmz::JsModuleRuntimeV8Basic::_to_data (V8Value value) {

   v8::HandleScope scope;

   V8Object result;

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_dataFuncTemplate->HasInstance (obj)) {

         result = obj;
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Data *
dmz::JsModuleRuntimeV8Basic::_to_data_ptr (V8Value value) {

   v8::HandleScope scope;

   Data *result (0);

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_dataFuncTemplate->HasInstance (obj)) {

         result = (Data *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}

