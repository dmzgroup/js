#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleV8.h>
#include <dmzRuntimeTimeSlice.h>
#include <dmzSystem.h>
#include <dmzTypesDeleteListTemplate.h>

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

   v8::Context::Scope cscope (v8Context);
   v8::HandleScope scope;

   Boolean deleteTimer (get_time_slice_mode () == TimeSliceModeSingle);
   // Create values on the stack so that if the TimerStruct is delete in the
   // callback, values will still be valid.
   JsModuleRuntimeV8Basic *modulePtr = &module;
   V8Object localSelf = v8::Local<v8::Object>::New (self);
   V8Function localCallback = v8::Local<v8::Function>::New (callback);

   if (localSelf.IsEmpty () || localCallback.IsEmpty ()) {deleteTimer = True; }
   else {

      v8::TryCatch tc;
      V8Value argv[] = { v8::Number::New (DeltaTime), localSelf };
      localCallback->Call (localSelf, 2, argv);
      if (tc.HasCaught ()) {

         modulePtr->handle_v8_exception (get_time_slice_handle (), tc);
         deleteTimer = True;
      }
   }

   if (deleteTimer) {

      modulePtr->delete_timer (localSelf, localCallback);
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

   JsModuleRuntimeV8Basic *self = to_self (Args);

   String name ("<Unknown>");;

   if (self && self->_core) {

      const Boolean SetInterval = Args.Length () > 2 ? True : False;

      V8Object obj = v8_to_object (Args[0]);
      V8Function func = v8_to_function (Args[SetInterval ? 2 : 1]);
      Float64 interval = SetInterval ? Args[1]->NumberValue () : 0.0;

      if (obj.IsEmpty () || func.IsEmpty ()) {} // Do nothing
      else {

         const Handle TimerHandle = self->_core->get_instance_handle (obj);

         if (TimerHandle) {

            TimerStruct *ts = new TimerStruct (
               TimerHandle,
               TimeSliceTypeRuntime,
               Repeating ? TimeSliceModeRepeating : TimeSliceModeSingle,
               interval,
               self->get_plugin_runtime_context (),
               *self,
               self->_v8Context);

            TimerStruct *list = self->_timerTable.lookup (TimerHandle);

            if (list) {

               while (list->next) { list = list->next; }
               list->next = ts;
            }
            else {

               if (!self->_timerTable.store (TimerHandle, ts)) {

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

      if (result.IsEmpty ()) { name = self->_core->get_instance_name (obj); }
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
dmz::JsModuleRuntimeV8Basic::_cancel_timer (const v8::Arguments &Args) {

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
dmz::JsModuleRuntimeV8Basic::_cancel_all_timers (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::False ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      V8Object obj = v8_to_object (Args[0]);

      if (obj.IsEmpty ()) {} // Throw Exception
      else if (self->delete_all_timers (obj)) { result = v8::True (); }
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
   else if (_core) {

      const Handle TimerHandle = _core->get_instance_handle (self);

      TimerStruct *current = _timerTable.lookup (TimerHandle);
      TimerStruct *prev (0);

      while (current) {

         if (current->callback == callback) {

            if (prev) { prev->next = current->next; }
            else {

               _timerTable.remove (TimerHandle);
               if (current->next) { _timerTable.store (TimerHandle, current->next); }
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


dmz::Boolean
dmz::JsModuleRuntimeV8Basic::delete_all_timers (V8Object self) {

   Boolean result (False);

   v8::HandleScope scope;

   if (self.IsEmpty ()) {} // do nothing
   else if (_core) {

      const Handle TimerHandle = _core->get_instance_handle (self);

      TimerStruct *list = _timerTable.remove (TimerHandle);

      if (list) { delete_list (list); result = true; }
   }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_init_time () {

   _timeApi.add_function ("setTimer", _set_timer, _self);
   _timeApi.add_function ("setRepeatingTimer", _set_repeating_timer, _self);
   _timeApi.add_function ("cancelTimer", _cancel_timer, _self);
   _timeApi.add_function ("cancelAllTimers", _cancel_all_timers, _self);
   _timeApi.add_function ("getFrameDelta", _get_frame_delta, _self);
   _timeApi.add_function ("getFrameTime", _get_frame_time, _self);
   _timeApi.add_function ("getSystemTime", _get_system_time, _self);
}


void
dmz::JsModuleRuntimeV8Basic::_reset_time () {

   HashTableHandleIterator it;
   TimerStruct *current (0);

   while (_timerTable.get_next (it, current)) {

      delete_list (current);
   }

   _timerTable.clear ();
}


void
dmz::JsModuleRuntimeV8Basic::_release_timer (const Handle InstanceHandle) {

   TimerStruct *list = _timerTable.remove (InstanceHandle);

   if (list) { delete_list (list); }
}

