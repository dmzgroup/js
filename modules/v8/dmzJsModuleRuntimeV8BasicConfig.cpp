#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeConfigToMatrix.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeConfigToVector.h>
#include <dmzSystemStreamString.h>

using namespace dmz;

namespace {

void
local_config_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      Config *ptr = (Config *)param;
      delete ptr; ptr = 0;
   }
}

};


V8Value
dmz::JsModuleRuntimeV8Basic::_create_config (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      if (Args.Length () == 0) {

         return v8::ThrowException (
            v8::Exception::Error (v8::String::New ("Invalid parameters.")));
      }
      else if (Args[0]->IsString ()) {

          Config cd (*(v8::String::AsciiValue (Args[0])));
          result = self->create_v8_config (&cd);
      }
      else {

         Config *ptr = self->_to_config_ptr (Args[0]);

         result = self->create_v8_config (ptr);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_is_type_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) { result = self->_to_config (Args[0]); }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_get (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Array result = v8::Array::New ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());
      String param = v8_to_string (Args[0]);

      if (param && ptr) {

         Config list;

         if (ptr->lookup_all_config (param, list)) {

            ConfigIterator it;
            Config next;

            Int32 count = 0;

            while (list.get_next_config (it, next)) {

               result->Set (v8::Integer::New (count), self->create_v8_config (&next));
               count++;
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_to_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         if (Args.Length () == 0) {

            String str;
            StreamString out (str);
            out << *ptr;
            if (str) { result = v8::String::New (str.get_buffer ()); }
         }
         else {

            const String Value = config_to_string (
               v8_to_string (Args[0]),
               *ptr,
               Args.Length () > 1 ? v8_to_string (Args[1]) : String ());

            if (Value) { result = v8::String::New (Value.get_buffer ()); } 
            else { result = v8::String::New (""); } 
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_to_number (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         const Float64 Value = config_to_float64 (
            v8_to_string (Args[0]),
            *ptr,
            Args.Length () > 1 ? v8_to_number (Args[1]) : 0.0);

         result = v8::Number::New (Value);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_to_vector (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_types) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         const Vector Value = config_to_vector (
            v8_to_string (Args[0]),
            *ptr,
            Args.Length () > 1 ? self->_types->to_dmz_vector (Args[1]) : Vector ());

         result = self->_types->to_v8_vector (Value);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_to_matrix (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         const Matrix Value = config_to_matrix (
            v8_to_string (Args[0]),
            *ptr,
            Args.Length () > 1 ? self->_types->to_dmz_matrix (Args[1]) : Matrix ());

         result = self->_types->to_v8_matrix (Value);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


// JsModuleRuntimeV8 Interface
v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_config (const Config *Value) {

   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (_configFunc.IsEmpty () == false) {

      result = _configFunc->NewInstance ();

      if (!result.IsEmpty ()) {

         Config *ptr = new Config (Value ? *Value : 0);

         result->SetInternalField (0, v8::External::Wrap ((void *)ptr));

         v8::Persistent<v8::Object> persist = v8::Persistent<v8::Object>::New (result);
         persist.MakeWeak ((void *)ptr, local_config_delete);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsModuleRuntimeV8Basic::_init_config () {

   v8::HandleScope scope;

   _configFuncTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _configFuncTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _configFuncTemplate->PrototypeTemplate ();

   proto->Set ("get", v8::FunctionTemplate::New (_config_get, _self));
   proto->Set ("toString", v8::FunctionTemplate::New (_config_to_string, _self));
   proto->Set ("toNumber", v8::FunctionTemplate::New (_config_to_number, _self));
   proto->Set ("toVector", v8::FunctionTemplate::New (_config_to_vector, _self));
   proto->Set ("toMatrix", v8::FunctionTemplate::New (_config_to_matrix, _self));

   _configApi.add_function ("create", _create_config, _self);
   _configApi.add_function ("isTypeOf", _config_is_type_of, _self);
}


V8Object
dmz::JsModuleRuntimeV8Basic::_to_config (V8Value value) {

   v8::HandleScope scope;

   V8Object result;

   V8Object obj = V8Object::Cast (value);

   if (obj.IsEmpty () == false) {

      if (_configFuncTemplate->HasInstance (obj)) {

         result = obj;
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Config *
dmz::JsModuleRuntimeV8Basic::_to_config_ptr (V8Value value) {

   v8::HandleScope scope;

   Config *result (0);

   V8Object obj = V8Object::Cast (value);

   if (obj.IsEmpty () == false) {

      if (_configFuncTemplate->HasInstance (obj)) {

         result = (Config *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}

