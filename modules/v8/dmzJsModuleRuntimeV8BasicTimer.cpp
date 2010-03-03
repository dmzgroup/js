#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzRuntimeTimeSlice.h>

struct dmz::JsModuleRuntimeV8Basic::TimerStruct : public TimeSlice {

   TimerStruct *next;
   JsModuleRuntimeV8Basic &module;
   v8::Handle<v8::Context> v8Context;
   V8ObjectPersist self;
   V8FunctionPersist callback;

   virtual void update_time_slice (const Float64 DeltaTime);

   TimerStruct (
         const Handle TimeSliceHandle,
         const TimeSliceTypeEnum Type,
         const TimeSliceModeEnum Mode,
         const Float64 TimeInterval,
         RuntimeContext *context,
         JsModuleRuntimeV8Basic &theModule,
         v8::Handle<v8::Context> theV8Context) :
         TimeSlice (TimeSliceHandle, Type, Mode, TimeInterval, context),
         next (0),
         module (theModule),
         v8Context (theV8Context) {;}

   virtual ~TimerStruct () {

      self.Dispose (); self.Clear ();
      callback.Dispose (); callback.Clear ();
   }
};


void
dmz::JsModuleRuntimeV8Basic::TimerStruct::update_time_slice (const Float64 DeltaTime) {

   if (v8Context.IsEmpty () || self.IsEmpty () || callback.IsEmpty ()) {} // do nothing
   else {

      v8::Context::Scope cscope (v8Context);
      v8::HandleScope scope;
      V8Value argv[] = { self, v8::Number::New (DeltaTime) };
      callback->Call (self, 2, argv);
   }

   if (get_time_slice_mode () == TimeSliceModeSingle) {

      module.delete_timer (self, callback);
   }
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_register_timer (const v8::Arguments &Args) {

   return _register_base_timer (Args, False);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_register_repeating_timer (const v8::Arguments &Args) {

   return _register_base_timer (Args, True);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_register_base_timer (
      const v8::Arguments &Args,
      const Boolean Repeating) {

   v8::HandleScope scope;
   V8Value result;

   String name ("<Unknown>");

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      V8Object obj = V8Object::Cast (Args[0]);
      V8Function func = V8Function::Cast (Args[1]);

      if (obj.IsEmpty () || func.IsEmpty ()) {} // Do nothing
      else {

         name = v8_to_string (obj->Get (v8::String::New ("name")));

         if (name) {

         }
         else {

         }
      }
   }

   if (result.IsEmpty ()) {

      String msg ("Failed to register ");
      msg << (Repeating ? "repeating " : "") << "timer for instance: " << name;
     
      V8String str = v8::String::New (msg.get_buffer ());
      result = v8::Exception::Error (str);
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::Boolean
dmz::JsModuleRuntimeV8Basic::delete_timer (V8Object self, V8Function callback) {

   Boolean result (False);

   v8::HandleScope scope;

   if (self.IsEmpty () || callback.IsEmpty ()) {} // do nothing
   else {

      const String Name = v8_to_string (self->Get (v8::String::NewSymbol ("name")));

      TimerStruct *current = _timerTable.lookup (Name);
      TimerStruct *prev (0);
      TimerStruct *found (0);

      while (current && !found) {

         if (current->callback == callback) {

            found = current;
         }
         else {

            prev = current;
            current = current->next;
         }
      }

      if (found) {

         if (prev) { prev->next = found->next; }
         else {

            _timerTable.remove (Name);
            if (found->next) { _timerTable.store (Name, found->next); }
         }

         found->next = 0;

         delete found; found = 0;
         result = True;
      }
   }

   return result;
}

