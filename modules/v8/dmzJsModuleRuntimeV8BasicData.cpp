#include "dmzJsModuleRuntimeV8Basic.h"
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
dmz::JsModuleRuntimeV8Basic::_data_number (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Data *ptr = self->_to_data_ptr (Args.This ());

      if (ptr) {

         const Handle Attr = self->_to_handle (Args[0]);
         v8::Handle<v8::Integer> arg1 = v8::Handle<v8::Integer>::Cast (Args[1]);
         const Int32 Element = arg1.IsEmpty () ? -1 : arg1->Value ();

         if (Attr && (Element >= 0)) {

            if (Args.Length () <= 2) {

               Float64 num (0.0);

               if (ptr->lookup_float64 (Attr, Element, num)) {

                  result = v8::Number::New (num);
               }
            }
            else {

               V8Value val = Args[2];

               ptr->store_float64 (Attr, Element, val->NumberValue ());
               result = val;
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


// JsModuleRuntimeV8 Interface
v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_data (const Data *Value) {

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


void
dmz::JsModuleRuntimeV8Basic::_init_data () {

   v8::HandleScope scope;

   _dataFuncTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _dataFuncTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _dataFuncTemplate->PrototypeTemplate ();

   proto->Set ("toString", v8::FunctionTemplate::New (_data_to_string, _self));
   proto->Set ("number", v8::FunctionTemplate::New (_data_number, _self));

   _dataApi.add_function ("create", _create_data, _self);
   _dataApi.add_function ("isTypeOf", _data_is_type_of, _self);
}


V8Object
dmz::JsModuleRuntimeV8Basic::_to_data (V8Value value) {

   v8::HandleScope scope;

   V8Object result;

   V8Object obj = V8Object::Cast (value);

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

   V8Object obj = V8Object::Cast (value);

   if (obj.IsEmpty () == false) {

      if (_dataFuncTemplate->HasInstance (obj)) {

         result = (Data *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}
