#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeConfigToMatrix.h>
#include <dmzRuntimeConfigToNamedHandle.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeConfigToVector.h>
#include <dmzRuntimeConfigWrite.h>
#include <dmzRuntimeEventType.h>
#include <dmzRuntimeMessaging.h>
#include <dmzRuntimeObjectType.h>
#include <dmzSystemStreamString.h>

using namespace dmz;

namespace {

void
local_config_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      Config *ptr = (Config *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};


V8Value
dmz::JsModuleRuntimeV8Basic::_create_config (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

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
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) { result = self->_to_config (Args[0]); }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_to_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         String str;
         StreamString out (str);

         out << *ptr;

         if (str) { result = v8::String::New (str.get_buffer ()); }
      }
   }

   return scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_config_add (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         if (Args.Length () == 1) {
         
            Config *data = self->_to_config_ptr (Args[0]);

            if (data) { ptr->add_config (*data); result = v8::Boolean::New (true); }
         }
         else if (Args.Length () > 1) {

            String param = v8_to_string (Args[0]);

            if (param) {

               self->_add_to_config (param, False, Args[1], *ptr);

               result = v8::Boolean::New (true);
            }
         }
      }
   }

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
dmz::JsModuleRuntimeV8Basic::_config_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

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
dmz::JsModuleRuntimeV8Basic::_config_number (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

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
dmz::JsModuleRuntimeV8Basic::_config_vector (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

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
dmz::JsModuleRuntimeV8Basic::_config_matrix (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_types) {

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


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_config_object_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         const ObjectType Type = config_to_object_type (
            v8_to_string (Args[0]),
            *ptr,
            Args.Length () > 1 ? v8_to_string (Args[1]) : String (),
            self->get_plugin_runtime_context ());

         if (Type) { result = self->create_v8_object_type (&Type); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_config_event_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         const EventType Type = config_to_event_type (
            v8_to_string (Args[0]),
            *ptr,
            Args.Length () > 1 ? v8_to_string (Args[1]) : String (),
            self->get_plugin_runtime_context ());

         if (Type) { result = self->create_v8_event_type (&Type); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_config_message (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         const Message Msg = config_create_message (
            v8_to_string (Args[0]),
            *ptr,
            Args.Length () > 1 ? v8_to_string (Args[1]) : String (),
            self->get_plugin_runtime_context ());

         if (Msg) { result = self->create_v8_message (Msg.get_name ()); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_config_named_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Config *ptr = self->_to_config_ptr (Args.This ());

      if (ptr) {

         const Handle Value = config_to_named_handle (
            v8_to_string (Args[0]),
            *ptr,
            Args.Length () > 1 ? v8_to_string (Args[1]) : String (),
            self->get_plugin_runtime_context ());

         if (Value) { result = v8::Integer::New (Value); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


// JsModuleRuntimeV8 Interface
v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_config (const Config *Value) {

   v8::Context::Scope cscope (_v8Context);
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


dmz::Boolean
dmz::JsModuleRuntimeV8Basic::to_dmz_config (v8::Handle<v8::Value> value, Config &out) {

   Boolean result (False);

   Config *ptr = _to_config_ptr (value);

   if (ptr)  { out = *ptr; result = True; }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_init_config () {

   v8::HandleScope scope;

   _configFuncTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _configFuncTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _configFuncTemplate->PrototypeTemplate ();

   proto->Set ("toString", v8::FunctionTemplate::New (_config_to_string, _self));
   proto->Set ("add", v8::FunctionTemplate::New (_config_add, _self));
   proto->Set ("get", v8::FunctionTemplate::New (_config_get, _self));
   proto->Set ("string", v8::FunctionTemplate::New (_config_string, _self));
   proto->Set ("number", v8::FunctionTemplate::New (_config_number, _self));
   proto->Set ("vector", v8::FunctionTemplate::New (_config_vector, _self));
   proto->Set ("matrix", v8::FunctionTemplate::New (_config_matrix, _self));
   proto->Set ("objectType", v8::FunctionTemplate::New (_config_object_type, _self));
   proto->Set ("eventType", v8::FunctionTemplate::New (_config_event_type, _self));
   proto->Set ("message", v8::FunctionTemplate::New (_config_message, _self));
   proto->Set ("namedHandle", v8::FunctionTemplate::New (_config_named_handle, _self));

   _configApi.add_function ("create", _create_config, _self);
   _configApi.add_function ("isTypeOf", _config_is_type_of, _self);
}


V8Object
dmz::JsModuleRuntimeV8Basic::_to_config (V8Value value) {

   v8::HandleScope scope;

   V8Object result;

   V8Object obj = v8_to_object (value);

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

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_configFuncTemplate->HasInstance (obj)) {

         result = (Config *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_add_to_config (
      const String &Name,
      const Boolean InArray,
      V8Value value,
      Config &parent) {

   v8::HandleScope scope;

   Boolean found (False);

   if (value.IsEmpty () == false) {

      if (value->IsNumber () || value->IsString () || value->IsBoolean () ||
            value->IsDate ()) {

         const String Value = v8_to_string (value->ToString ());

         if (InArray) {

            Config child (Name);
            child.set_in_array (True);
            child.store_attribute ("value", Value);
            parent.add_config (child);
         }
         else { parent.store_attribute (Name, Value); }

         found = True;
      }
      else if (value->IsArray ()) {

         V8Array array = v8_to_array (value);

         if (array.IsEmpty () == false) {

            const int Length = array->Length ();

            if (InArray) {

               Config child (Name);

               for (int ix = 0; ix < Length; ix++) {

                  _add_to_config (
                     "element",
                     True,
                     array->Get (v8::Integer::New (ix)),
                     child);
               }

               parent.add_config (child);
            }
            else {

               for (int ix = 0; ix < Length; ix++) {

                  _add_to_config (Name, True, array->Get (v8::Integer::New (ix)), parent);
               }
            }
         }

         found = True;
      }
   }

   V8Object obj = v8_to_object (value);

   if (!found && obj.IsEmpty () == false) {

      if (_types) {

         if (_types->is_v8_vector (obj)) {

            Config value = vector_to_config (Name, _types->to_dmz_vector (obj));
            parent.add_config (value);
            found = true;
         }
         else if (_types->is_v8_matrix (obj)) {

            Config value = matrix_to_config (Name, _types->to_dmz_matrix (obj));
            parent.add_config (value);
            found = true;
         }
         else if (_types->is_v8_mask (obj)) {

            found = true;
         }
      }

      if (!found) {

         Data data;
         EventType etype;
         ObjectType otype;

         if (to_dmz_data (obj, data)) {

            parent.add_config (
               data_to_config (Name, data, get_plugin_runtime_context ()));

            found = true;
         }
         else if (to_dmz_event_type (obj, etype)) {

            Config child (Name);
            child.store_attribute ("name", etype.get_name ());
            parent.add_config (child);
            found = true;
         }
         else if (to_dmz_object_type (obj, otype)) {

            Config child (Name);
            child.store_attribute ("name", otype.get_name ());
            parent.add_config (child);
            found = true;
         }
      }

      if (!found) {

         V8Array prop = obj->GetPropertyNames ();

         if (prop.IsEmpty () == false) {

            const int Length = prop->Length ();

            for (int ix = 0; ix < Length; ix++) {

               V8Value name = prop->Get (v8::Integer::New (ix));

               if ((name.IsEmpty () == false) && name->IsString () &&
                     obj->HasRealNamedProperty (name->ToString ())) {

                  Config data (Name);

                  _add_to_config (v8_to_string (name), False, obj->Get (name), data);

                  parent.add_config (data);
               }
            }
         }
      }
   }
}

