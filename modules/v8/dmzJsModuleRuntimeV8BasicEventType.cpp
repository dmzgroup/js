#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzRuntimeEventType.h>
#include <dmzSystemStreamString.h>

using namespace dmz;

namespace {

void
local_event_type_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      EventType *ptr = (EventType *)param;
      delete ptr; ptr = 0;
   }
}

};


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_lookup (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType type;

      if (self->_defs.lookup_event_type (v8_to_string (Args[0]), type)) {

         result = self->create_v8_event_type (&type);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_is_type_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) { result = self->_to_event_type (Args[0]); }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_to_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args.This ());

      if (ptr && *ptr) {

         result = v8::String::New (ptr->get_name ().get_buffer ());
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}

V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_get_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args.This ());

      if (ptr) {

         const String Name = ptr->get_name ();

         result = v8::String::New (Name ? Name.get_buffer () : "");
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_get_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args.This ());

      if (ptr) {

         result = v8::Number::New ((double)ptr->get_handle ());
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_get_parent (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args.This ());

      if (ptr) {

         EventType parent = ptr->get_parent ();

         if (parent) { result = self->create_v8_event_type (&parent); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_get_children (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Array result = v8::Array::New ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args.This ());

      if (ptr) {

         EventTypeIterator it;
         EventType next;

         Int32 count (0);

         while (ptr->get_next_child (it, next)) {

            result->Set (v8::Integer::New (count), self->create_v8_event_type (&next));
            count++;
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_is_of_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args.This ());
      EventType *arg = self->_to_event_type_ptr (Args[0]);

      if (ptr && arg) {

         result = v8::Boolean::New (ptr->is_of_type (*arg));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_is_of_exact_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args.This ());
      EventType *arg = self->_to_event_type_ptr (Args[0]);

      if (ptr && arg) {

         result = v8::Boolean::New (ptr->is_of_exact_type (*arg));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_event_type_get_config (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      EventType *ptr = self->_to_event_type_ptr (Args[0]);

      if (ptr) {

         Config cd = ptr->get_config ();

         if (!cd) { cd = Config ("empty"); }

         result = self->create_v8_config (&cd);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


// JsModuleRuntimeV8 Interface
v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_event_type (const EventType *Value) {

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (_eventTypeFunc.IsEmpty () == false) {

      result = _eventTypeFunc->NewInstance ();

      if (!result.IsEmpty ()) {

         EventType *ptr = new EventType (Value ? *Value : 0);

         result->SetInternalField (0, v8::External::Wrap ((void *)ptr));

         v8::Persistent<v8::Object> persist = v8::Persistent<v8::Object>::New (result);
         persist.MakeWeak ((void *)ptr, local_event_type_delete);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Boolean
dmz::JsModuleRuntimeV8Basic::to_dmz_event_type (V8Value value, EventType &out) {

   Boolean result (False);

   if ((value.IsEmpty () == false) && value->IsString ()) {

      result = _defs.lookup_event_type (*(v8::String::AsciiValue (value)), out);
   }
   else {

      EventType *ptr = _to_event_type_ptr (value);

      if (ptr) { out = *ptr; result = True; }
   }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_init_event_type () {

   v8::HandleScope scope;

   _eventTypeFuncTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _eventTypeFuncTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _eventTypeFuncTemplate->PrototypeTemplate ();

   proto->Set ("toString", v8::FunctionTemplate::New (_event_type_to_string, _self));
   proto->Set ("getName", v8::FunctionTemplate::New (_event_type_get_name, _self));
   proto->Set ("getHandle", v8::FunctionTemplate::New (_event_type_get_handle, _self));
   proto->Set ("getParent", v8::FunctionTemplate::New (_event_type_get_parent, _self));

   proto->Set (
      "getChildren",
      v8::FunctionTemplate::New (_event_type_get_children, _self));

   proto->Set ("isOfType", v8::FunctionTemplate::New (_event_type_is_of_type, _self));

   proto->Set (
      "isOfExactType",
      v8::FunctionTemplate::New (_event_type_is_of_exact_type, _self));

   proto->Set ("getConfig", v8::FunctionTemplate::New (_event_type_get_config, _self));

   _eventTypeApi.add_function ("lookup", _event_type_lookup, _self);
   _eventTypeApi.add_function ("isTypeOf", _event_type_is_type_of, _self);
}


V8Object
dmz::JsModuleRuntimeV8Basic::_to_event_type (V8Value value) {

   v8::HandleScope scope;

   V8Object result;

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_eventTypeFuncTemplate->HasInstance (obj)) {

         result = obj;
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::EventType *
dmz::JsModuleRuntimeV8Basic::_to_event_type_ptr (V8Value value) {

   v8::HandleScope scope;

   EventType *result (0);

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_eventTypeFuncTemplate->HasInstance (obj)) {

         result = (EventType *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}

