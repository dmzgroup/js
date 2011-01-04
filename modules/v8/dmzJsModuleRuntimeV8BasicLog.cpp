#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleV8.h>

namespace {

void
local_to_log (dmz::StreamLog &log, const v8::Arguments &args) {

   v8::HandleScope scope;

   const int Length = args.Length ();

   for (int ix = 0; ix < Length; ix++) {

      if (ix > 0) { log << " "; }
      v8::String::AsciiValue str (args[ix]);
      log << *str;
   }

   log << dmz::endl;
}


v8::Handle<v8::Value>
local_log_error (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::Log *log = (dmz::Log *)args.This ()->GetPointerFromInternalField (0);

   if (log) { local_to_log (log->error, args); }

   return args.This ();
}


v8::Handle<v8::Value>
local_log_warn (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::Log *log = (dmz::Log *)args.This ()->GetPointerFromInternalField (0);

   if (log) { local_to_log (log->warn, args); }

   return args.This ();
}


v8::Handle<v8::Value>
local_log_info (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::Log *log = (dmz::Log *)args.This ()->GetPointerFromInternalField (0);

   if (log) { local_to_log (log->info, args); }

   return args.This ();
}


v8::Handle<v8::Value>
local_log_debug (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::Log *log = (dmz::Log *)args.This ()->GetPointerFromInternalField (0);

   if (log) { local_to_log (log->debug, args); }

   return args.This ();
}


v8::Handle<v8::Value>
local_log_out (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::Log *log = (dmz::Log *)args.This ()->GetPointerFromInternalField (0);

   if (log) { local_to_log (log->out, args); }

   return args.This ();
}


void
local_log_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      dmz::Log *ptr = (dmz::Log *)param;

      ptr->debug << "Deleting JavaScript instance Log." << dmz::endl;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};


// JsModuleRuntimeV8 Interface
v8::Handle<v8::Value>
dmz::JsModuleRuntimeV8Basic::create_v8_log (const String &Name) {

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   v8::Handle<v8::Object> result;

   if (_logFunc.IsEmpty () == false) {

      result = _logFunc->NewInstance ();

      if (!result.IsEmpty ()) {

         dmz::Log *log = new dmz::Log (Name, get_plugin_runtime_context ());

         result->SetInternalField (0, v8::External::New ((void *)log));

         v8::Persistent<v8::Object> ptr = v8::Persistent<v8::Object>::New (result);
         ptr.MakeWeak ((void *)log, local_log_delete);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Log *
dmz::JsModuleRuntimeV8Basic::to_dmz_log (v8::Handle<v8::Value> value) {

   Log *result (0);

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_logFuncTemplate->HasInstance (obj)) {

         result = (Log *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_create_log (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      String name;

      V8Value value = Args[0];

      if (v8_is_valid (value)) {

         if (value->IsString ()) { name = v8_to_string (value); }
         else if (value->IsObject ()) {

            if (self->_core) { name = self->_core->get_instance_name (value); }
         }
      }

      if (name) { result = self->create_v8_log (name); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsModuleRuntimeV8Basic::_init_log () {

   v8::HandleScope scope;

   _logFuncTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   v8::Handle<v8::ObjectTemplate> instance = _logFuncTemplate->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = _logFuncTemplate->PrototypeTemplate ();

   proto->Set ("error", v8::FunctionTemplate::New (local_log_error));
   proto->Set ("warn", v8::FunctionTemplate::New (local_log_warn));
   proto->Set ("info", v8::FunctionTemplate::New (local_log_info));
   proto->Set ("debug", v8::FunctionTemplate::New (local_log_debug));
   proto->Set ("out", v8::FunctionTemplate::New (local_log_out));

   _logApi.add_function ("create", _create_log, _self);
}
