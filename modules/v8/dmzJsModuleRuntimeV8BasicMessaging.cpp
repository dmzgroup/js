#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzTypesHashTableHandleTemplate.h>

using namespace dmz;

namespace {

struct CallbackStruct {

   V8FunctionPersist func;
   V8ObjectPersist self;

   ~CallbackStruct () { func.Dispose (); func.Clear (); self.Dispose (); self.Clear (); }
};

void
local_message_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      Message *ptr = (Message *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};

struct dmz::JsModuleRuntimeV8Basic::MessageStruct : public MessageObserver {

   JsModuleRuntimeV8Basic &module;
   v8::Handle<v8::Context> v8Context;
   HashTableHandleTemplate<CallbackStruct> cbTable;

   MessageStruct (
         const Handle ObsHandle,
         const String &Name, RuntimeContext *context,
         JsModuleRuntimeV8Basic &theModule) :
         MessageObserver (ObsHandle, Name, context),
         module (theModule) {;}

   ~MessageStruct () { cbTable.empty (); }

   void receive_message (
      const Message &Type,
      const Handle MessageSendHandle,
      const Handle TargetObserverHandle,
      const Data *InData,
      Data *outData);
};


void
dmz::JsModuleRuntimeV8Basic::MessageStruct::receive_message (
      const Message &Type,
      const Handle MessageSendHandle,
      const Handle TargetObserverHandle,
      const Data *InData,
      Data *outData) {

   Message current = Type;

   CallbackStruct *cb = cbTable.lookup (current.get_handle ());

   if (!cb) {

      current.become_parent ();

      while (!cb && current) {

         cb = cbTable.lookup (current.get_handle ());
         if (!cb) { current.become_parent (); }
      }
   }

   if (cb && (cb->self.IsEmpty () == false) && (cb->func.IsEmpty () == false) &&
         (v8Context.IsEmpty () == false)) {

      v8::Context::Scope cscope (v8Context);
      v8::HandleScope scope;

      V8Object outObj = v8_to_object (module.create_v8_data (outData));

      v8::Handle<v8::Value> argv[] = {
         module.create_v8_data (InData),
         outObj,
         v8::Integer::New (MessageSendHandle),
         v8::Integer::New (TargetObserverHandle),
         module.create_v8_message (Type.get_name ()),
         cb->self
      };

      v8::TryCatch tc;

      cb->func->Call (cb->self, 6, argv);

      if (tc.HasCaught ()) {

         module.handle_v8_exception (tc);

         cb = cbTable.remove (current.get_handle ());
         if (cb) { delete cb; cb = 0; }
      }
      else if (outData && (outObj.IsEmpty () == false)) {

         Data *ptr = (Data *)v8::External::Unwrap (outObj->GetInternalField (0));

         if (ptr) { *outData = *ptr; }
      }
   }
}


V8Value
dmz::JsModuleRuntimeV8Basic::_create_message (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Message *ptr = self->_to_message_ptr (Args[0]);
      if (ptr) { result = self->create_v8_message (ptr->get_name ()); }
      else if (Args[0]->IsString ()) {

         result = self->create_v8_message (*(v8::String::AsciiValue (Args[0])));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_message_is_type_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_to_message_ptr (Args[0])) { result = Args[0]; }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_message_global_subscribe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_core && (Args.Length () >= 2)) {

      V8Object obj = v8_to_object (Args[0]);
      const String MsgName = v8_to_string (Args[1]);

      const Handle ObsHandle = self->_core->get_instance_handle (obj);
      const String Name = self->_core->get_instance_name (obj);;

      Message msg;
      self->_defs.create_message (MsgName, msg);

      if (ObsHandle && msg) {

         MessageStruct *ms = self->_msgTable.lookup (ObsHandle);

         if (!ms) {

            ms = new MessageStruct (
               ObsHandle,
               Name,
               self->get_plugin_runtime_context (),
               *self);

            if (ms) {

               ms->v8Context = self->_v8Context;

               if (!self->_msgTable.store (ObsHandle, ms)) { delete ms; ms = 0; }
            }
         }

         if (ms) {

            const Handle MsgHandle = msg.get_handle ();

            CallbackStruct *cb = ms->cbTable.lookup (MsgHandle);

            if (!cb) {

               cb = new CallbackStruct;

               if (cb && !ms->cbTable.store (MsgHandle, cb)) { delete cb; cb = 0; }
            }

            if (cb && Args[2]->IsFunction ()) {

               cb->self.Dispose (); cb->self.Clear ();
               cb->func.Dispose (); cb->func.Clear ();

               cb->self = V8ObjectPersist::New (obj);
               cb->func = V8FunctionPersist::New (v8_to_function (Args[2]));
               result = v8::Local<v8::Function>::New (cb->func);
               ms->subscribe_to_message (msg);
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_message_global_unsubscribe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::False ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_core) {

      String name;

      V8Object obj = v8_to_object (Args[0]);
      Message msg;

      if (Args.Length () > 1) {

         const String MsgName = v8_to_string (Args[1]);
         self->_defs.lookup_message (MsgName, msg);
      }

      const Handle ObsHandle = self->_core->get_instance_handle (obj);

      if (ObsHandle) {

         MessageStruct *ms = self->_msgTable.lookup (ObsHandle);

         if (ms) {

            result = v8::True ();

            if (msg) {

               ms->unsubscribe_to_message (msg);
               CallbackStruct *cb = ms->cbTable.remove(msg.get_handle ());
               if (cb) { delete cb; cb = 0; }
            }
            else if (Args.Length () == 1) {

               ms->cbTable.empty ();
               ms->unsubscribe_to_all_messages ();
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_message_to_string (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Message *ptr = self->_to_message_ptr (Args.This ());

      if (ptr) {

         String str (ptr->get_name ());

         if (str) { result = v8::String::New (str.get_buffer ()); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_message_send (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      Message *ptr = self->_to_message_ptr (Args.This ());

      if (ptr) {

         Handle target (0);
         Data *inData (0);
         Data *outData (0);

         if (Args.Length () == 1) {

            inData = self->_to_data_ptr (Args[0]);
            if (!inData) { target = v8_to_handle (Args[0]); }
         }
         else if (Args.Length () > 1) {

            target = v8_to_handle (Args[0]);
            inData = self->_to_data_ptr (Args[1]);
            outData = self->_to_data_ptr (Args[2]);
         }

         result = v8::Number::New ((double)ptr->send (target, inData, outData));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_message_subscribe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_core && (Args.Length () >= 2)) {

      V8Object obj = v8_to_object (Args[0]);

      const Handle ObsHandle = self->_core->get_instance_handle (obj);
      const String Name = self->_core->get_instance_name (obj);;

      Message *ptr = self->_to_message_ptr (Args.This ());

      if (ObsHandle && ptr) {

         MessageStruct *ms = self->_msgTable.lookup (ObsHandle);

         if (!ms) {

            ms = new MessageStruct (
               ObsHandle,
               Name,
               self->get_plugin_runtime_context (),
               *self);

            if (ms) {

               ms->v8Context = self->_v8Context;

               if (!self->_msgTable.store (ObsHandle, ms)) { delete ms; ms = 0; }
            }
         }

         if (ms) {

            const Handle MsgHandle = ptr->get_handle ();

            CallbackStruct *cb = ms->cbTable.lookup (MsgHandle);

            if (!cb) {

               cb = new CallbackStruct;

               if (cb && !ms->cbTable.store (MsgHandle, cb)) { delete cb; cb = 0; }
            }

            if (cb && Args[1]->IsFunction ()) {

               cb->self.Dispose (); cb->self.Clear ();
               cb->func.Dispose (); cb->func.Clear ();

               cb->self = V8ObjectPersist::New (obj);
               cb->func = V8FunctionPersist::New (v8_to_function (Args[1]));
               result = v8::Local<v8::Function>::New (cb->func);
               ms->subscribe_to_message (*ptr);
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


V8Value
dmz::JsModuleRuntimeV8Basic::_message_unsubscribe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_core) {

      String name;

      V8Object obj = v8_to_object (Args[0]);

      const Handle ObsHandle = self->_core->get_instance_handle (obj);

      Message *ptr = self->_to_message_ptr (Args.This ());

      if (ObsHandle && ptr) {

         MessageStruct *ms = self->_msgTable.lookup (ObsHandle);

         if (ms) {

            CallbackStruct *cb = ms->cbTable.remove (ptr->get_handle ());

            if (cb) {

               result = v8::Local<v8::Function>::New (cb->func);
               delete cb; cb = 0;
            }

            ms->unsubscribe_to_message (*ptr);
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


// JsModuleRuntimeV8 Interface
v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_message (const String &Name) {

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (Name && _msgFunc.IsEmpty () == false) {

      result = _msgFunc->NewInstance ();

      if (result.IsEmpty () == false) {

         Message *ptr = new Message;

         if (ptr) { _defs.create_message (Name, *ptr); }

         result->SetInternalField (0, v8::External::Wrap ((void *)ptr));

         v8::Persistent<v8::Object> persist = v8::Persistent<v8::Object>::New (result);
         persist.MakeWeak ((void *)ptr, local_message_delete);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsModuleRuntimeV8Basic::_init_messaging () {

   v8::HandleScope scope;

   _msgFuncTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _msgFuncTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _msgFuncTemplate->PrototypeTemplate ();

   proto->Set ("toString", v8::FunctionTemplate::New (_message_to_string, _self));
   proto->Set ("send", v8::FunctionTemplate::New (_message_send, _self));
   proto->Set ("subscribe", v8::FunctionTemplate::New (_message_subscribe, _self));
   proto->Set ("unsubscribe", v8::FunctionTemplate::New (_message_unsubscribe, _self));

   _msgApi.add_function ("create", _create_message, _self);
   _msgApi.add_function ("isTypeOf", _message_is_type_of, _self);
   _msgApi.add_function ("subscribe", _message_global_subscribe, _self);
   _msgApi.add_function ("unsubscribe", _message_global_unsubscribe, _self);
}


void
dmz::JsModuleRuntimeV8Basic::_reset_messaging () { _msgTable.empty (); }


dmz::Message *
dmz::JsModuleRuntimeV8Basic::_to_message_ptr (V8Value value) {

   v8::HandleScope scope;

   Message *result (0);

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_msgFuncTemplate->HasInstance (obj)) {

         result = (Message *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}

