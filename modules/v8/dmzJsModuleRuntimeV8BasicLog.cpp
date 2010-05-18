#include "dmzJsModuleRuntimeV8Basic.h"

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


v8::Persistent<v8::FunctionTemplate>
local_create_log_template () {

   v8::HandleScope scope;

   v8::Handle<v8::FunctionTemplate> result = v8::FunctionTemplate::New ();

   v8::Handle<v8::ObjectTemplate> instance = result->InstanceTemplate ();

   instance->SetInternalFieldCount (1);

   v8::Handle<v8::ObjectTemplate> proto = result->PrototypeTemplate ();

   proto->Set ("error", v8::FunctionTemplate::New (local_log_error));
   proto->Set ("warn", v8::FunctionTemplate::New (local_log_warn));
   proto->Set ("info", v8::FunctionTemplate::New (local_log_info));
   proto->Set ("debug", v8::FunctionTemplate::New (local_log_debug));
   proto->Set ("out", v8::FunctionTemplate::New (local_log_out));

   return v8::Persistent<v8::FunctionTemplate>::New (result);
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


void
dmz::JsModuleRuntimeV8Basic::_init_log () {

   _logFuncTemplate = local_create_log_template ();
}
