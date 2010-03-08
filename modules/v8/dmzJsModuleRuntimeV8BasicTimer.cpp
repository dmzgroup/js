#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzRuntimeTimeSlice.h>
#include <dmzSystem.h>

#include <qdb.h>
static dmz::qdb out;

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

   Boolean deleteTimer (False);

   if (v8Context.IsEmpty () || self.IsEmpty () || callback.IsEmpty ()) {} // do nothing
   else {

      v8::Context::Scope cscope (v8Context);
      v8::HandleScope scope;
      v8::TryCatch tc;
      V8Value argv[] = { self, v8::Number::New (DeltaTime) };
      callback->Call (self, 2, argv);
      if (tc.HasCaught ()) {

         module.handle_v8_exception (tc);
         deleteTimer = True;
      }
   }

   if (deleteTimer || (get_time_slice_mode () == TimeSliceModeSingle)) {

      module.delete_timer (self, callback);
   }
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_set_timer (const v8::Arguments &Args) {

   return _set_base_timer (Args, False);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_set_repeating_timer (const v8::Arguments &Args) {

   return _set_base_timer (Args, True);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_set_base_timer (
      const v8::Arguments &Args,
      const Boolean Repeating) {

   v8::HandleScope scope;
   V8Value result;

   String name ("<Unknown>");

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Boolean SetInterval = Args.Length () > 2 ? True : False;

      V8Object obj = v8_to_object (Args[0]);
      V8Function func = v8_to_function (Args[SetInterval ? 2 : 1]);
      Float64 interval = SetInterval ? Args[1]->NumberValue () : 0.0;

      if (obj.IsEmpty () || func.IsEmpty ()) {} // Do nothing
      else {

         name = v8_to_string (obj->Get (v8::String::New ("name")));

         if (name) {

            TimerStruct *ts = new TimerStruct (
               self->_defs.create_named_handle (name),
               TimeSliceTypeRuntime,
               Repeating ? TimeSliceModeRepeating : TimeSliceModeSingle,
               interval,
               self->get_plugin_runtime_context (),
               *self,
               self->_v8Context);

            TimerStruct *list = self->_timerTable.lookup (name);

            if (list) {

               while (list->next) { list = list->next; }
               list->next = ts;
            }
            else {

               if (!self->_timerTable.store (name, ts)) {

                  delete ts; ts = 0;
               }
            }

            if (ts) {

               ts->self = V8ObjectPersist::New (obj);
               ts->callback = V8FunctionPersist::New (func);
               result = func;
            }

            if (ts && !Repeating) { ts->start_time_slice (); }
         }
         else {

         }
      }
   }

   if (result.IsEmpty ()) {

      String msg ("Failed to register ");
      msg << (Repeating ? "repeating " : "") << "timer for instance: " << name;
     
      V8String str = v8::String::New (msg.get_buffer ());
      return v8::ThrowException (v8::Exception::Error (str));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_cancle_timer (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      V8Object obj = v8_to_object (Args[0]);
      V8Function func = v8_to_function (Args[1]);

      if (obj.IsEmpty () || func.IsEmpty ()) {} // Throw Exception
      else {

         result = func;
         self->delete_timer (obj, func);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_get_frame_delta (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) { result = v8::Number::New (self->_time.get_frame_delta ()); }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_get_frame_time (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) { result = v8::Number::New (self->_time.get_frame_time ()); }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_get_system_time (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Number::New (dmz::get_time ());

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

      while (current) {

         if (current->callback == callback) {

            if (prev) { prev->next = current->next; }
            else {

               _timerTable.remove (Name);
               if (current->next) { _timerTable.store (Name, current->next); }
            }

            TimerStruct *tmp = current;
            current = current->next;
            tmp->next = 0;

            delete tmp; tmp = 0;
            result = True;
         }
         else {

            prev = current;
            current = current->next;
         }
      }
   }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_init_time () {

   _timeApi.add_function ("setTimer", _set_timer, _self);
   _timeApi.add_function ("setRepeatingTimer", _set_repeating_timer, _self);
   _timeApi.add_function ("cancleTimer", _cancle_timer, _self);
   _timeApi.add_function ("getFrameDelta", _get_frame_delta, _self);
   _timeApi.add_function ("getFrameTime", _get_frame_time, _self);
   _timeApi.add_function ("getSystemTime", _get_system_time, _self);
}
