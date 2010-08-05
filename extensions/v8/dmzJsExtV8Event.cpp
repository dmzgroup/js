#include <dmzEventCallbackMasks.h>
#include <dmzEventConsts.h>
#include <dmzEventModule.h>
#include "dmzJsExtV8Event.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimeData.h>
#include <dmzRuntimeEventType.h>
#include <dmzRuntimeObjectType.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

#include <qdb.h>
static dmz::qdb out;


dmz::JsExtV8Event::JsExtV8Event (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      EventObserverUtil (Info, local),
      JsExtV8 (Info),
      _log (Info),
      _defs (Info),
      _event (0),
      _core (0),
      _runtime (0),
      _types (0),
      _defaultAttr (0) {

   _init (local);
}


dmz::JsExtV8Event::~JsExtV8Event () {

}


// Plugin Interface
void
dmz::JsExtV8Event::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

   }
}


void
dmz::JsExtV8Event::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_types) { _types = JsModuleTypesV8::cast (PluginPtr); }
      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
   }
}


// Event Observer Interface
void
dmz::JsExtV8Event::create_event (
      const Handle EventHandle,
      const EventType &Type,
      const EventLocalityEnum Locality) {

   _do_callback (EventHandle, Locality, _createTable.lookup (Type.get_handle ()));
}


void
dmz::JsExtV8Event::close_event (
      const Handle EventHandle,
      const EventType &Type,
      const EventLocalityEnum Locality) {

   _do_callback (EventHandle, Locality, _closeTable.lookup (Type.get_handle ()));
}


// JsExtV8 Interface
void
dmz::JsExtV8Event::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Event::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Event::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/components/event",
            _eventApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

      deactivate_all_event_callbacks ();
   }
   else if (State == JsExtV8::Shutdown) {

      _eventApi.clear ();
      _obsTable.empty ();
      _createTable.empty ();
      _closeTable.empty ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Event::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

   ObsStruct *os = _obsTable.remove (InstanceHandle);

   if (os) {

      if (_v8Context.IsEmpty () == false) {

         v8::Context::Scope cscope (_v8Context);

         V8Function func;
         _release_callback (EventCreateMask, InstanceHandle, os->create, func);
         _release_callback (EventCloseMask, InstanceHandle, os->close, func);
      }

      delete os; os = 0;
   }
}


dmz::Boolean
dmz::JsExtV8Event::_to_event (
      const v8::Arguments &Args,
      JsExtV8Event *&self,
      Handle &event,
      Handle &attr) {

   self = _to_self (Args);

   if (self) {

      event = v8_to_handle (Args[0]);

      if (Args.Length () > 1) {

         if (Args[1]->IsNumber ()) { attr = Args[1]->Uint32Value (); }
         else if (Args[1]->IsString ()) {

            const String Name (*(v8::String::AsciiValue (Args[1])));

            attr = self->_defs.create_named_handle (Name);
         }
         else { attr = self->_defaultAttr; }
      }
      else { attr = self->_defaultAttr; }
   }

   return self && event && attr;
}


dmz::V8Function
dmz::JsExtV8Event::_register_observer_static (
      const v8::Arguments &Args,
      const Mask &EventMask) {

   v8::HandleScope scope;
   V8Function result;

   JsExtV8Event *self = _to_self (Args);
   if (self) { result = self->_register_observer (Args, EventMask); }

   return result.IsEmpty () ? result : scope.Close (result);
}


// Event API Bindings
dmz::V8Value
dmz::JsExtV8Event::_event_release (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self = _to_self (Args);

   if (self && Args[0]->IsObject ()) {

      V8Object src = V8Object::Cast (Args[0]);

      const Handle Obs = self->_core ? self->_core->get_instance_handle (src) : 0;
      ObsStruct *os = self->_obsTable.lookup (Obs);

      if (os) {

         if (Args[1]->IsFunction ()) {

            V8Function func = V8Function::Cast (Args[1]);
            self->_release_callback (EventCreateMask, Obs, os->create, func);
            self->_release_callback (EventCloseMask, Obs, os->close, func);

            result = func;
         }
         else if (Args.Length () == 1) {

            V8Function func;
            self->_release_callback (EventCreateMask, Obs, os->create, func);
            self->_release_callback (EventCloseMask, Obs, os->close, func);
            result = v8::True ();
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_create (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self = _to_self (Args);

   if (self && self->_runtime && self->_event) {

      EventType type;

      if (self->_runtime->to_dmz_event_type (Args[0], type)) {

         EventLocalityEnum locality = EventLocal;

         if (Args.Length () > 1) {

            locality = (EventLocalityEnum)v8_to_uint32 (Args[1]);

            if (locality == EventLocalityUnknown) { locality = EventLocal; }
         }

         const Handle Event = self->_event->create_event (type, locality);

         if (Event) { result = v8::Integer::New (Event); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_create_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, EventCreateMask);
   if (result.IsEmpty ()) { result = v8::Undefined (); }
   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_close (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self = _to_self (Args);

   if (self && self->_event) {

      result = v8::Boolean::New (self->_event->close_event (v8_to_handle (Args[0])));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_close_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_observer_static (Args, EventCloseMask);
   if (result.IsEmpty ()) { result = v8::Undefined (); }
   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self = _to_self (Args);

   if (self && self->_event && self->_runtime) {

      EventType type;

      if (self->_event->lookup_event_type (v8_to_handle (Args[0]), type)) {

         result = self->_runtime->create_v8_event_type (&type);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_locality (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self = _to_self (Args);

   if (self && self->_event) {

      const EventLocalityEnum Locality =
         self->_event->lookup_locality (v8_to_handle (Args[0]));

      if (Locality == EventLocal) { result = v8::Integer::New (EventLocal); }
      else if (Locality == EventRemote) { result = v8::Integer::New (EventRemote); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event) {

      if (Args.Length () > 2) {

         const Handle Value = v8_to_handle (Args[2]);

         if (self->_event->store_handle (event, attr, Value)) {

            result = v8::Integer::New (Value);
         }
      }
      else {

         Handle value (0);

         if (self->_event->lookup_handle (event, attr, value)) {

            result = v8::Integer::New (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_object_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event) {

      if (Args.Length () > 2) {

         const Handle Value = v8_to_handle (Args[2]);

         if (self->_event->store_object_handle (event, attr, Value)) {

            result = v8::Integer::New (Value);
         }
      }
      else {

         Handle value (0);

         if (self->_event->lookup_object_handle (event, attr, value)) {

            result = v8::Integer::New (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_object_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_runtime) {

      if (Args.Length () > 2) {

         ObjectType type;

         if (self->_runtime->to_dmz_object_type (Args[2], type)) {

            if (self->_event->store_object_type (event, attr, type)) {

               result = self->_runtime->create_v8_object_type (&type);
            }
         }
      }
      else {

         ObjectType type;

         if (self->_event->lookup_object_type (event, attr, type)) {

            result = self->_runtime->create_v8_object_type (&type);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_state (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_types) {

      if (Args.Length () > 2) {

         V8Value arg = Args[2];

         Mask state;

         if (arg->IsString ()) {

            self->_defs.lookup_state (*(v8::String::AsciiValue (arg)), state);
         }
         else { state = self->_types->to_dmz_mask (arg); }

         if (self->_event->store_state (event, attr, state)) {

            result = self->_types->to_v8_mask (state);
         }
      }
      else {

         Mask state;

         if (self->_event->lookup_state (event, attr, state)) {

            result = self->_types->to_v8_mask (state);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_time_stamp (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event) {

      if (Args.Length () > 2) {

         const Float64 Value = v8_to_number (Args[2]);

         if (self->_event->store_time_stamp (event, attr, Value)) {

            result = v8::Number::New (Value);
         }
      }
      else {

         Float64 value (0.0);

         if (self->_event->lookup_time_stamp (event, attr, value)) {

            result = v8::Number::New (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_position (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_types) {

      if (Args.Length () > 2) {

         const Vector Value = self->_types->to_dmz_vector (Args[2]);

         if (self->_event->store_position (event, attr, Value)) {

            result = self->_types->to_v8_vector (Value);
         }
      }
      else {

         Vector value;

         if (self->_event->lookup_position (event, attr, value)) {

            result = self->_types->to_v8_vector (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_orientation (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_types) {

      if (Args.Length () > 2) {

         const Matrix Value = self->_types->to_dmz_matrix (Args[2]);

         if (self->_event->store_orientation (event, attr, Value)) {

            result = self->_types->to_v8_matrix (Value);
         }
      }
      else {

         Matrix value;

         if (self->_event->lookup_orientation (event, attr, value)) {

            result = self->_types->to_v8_matrix (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_velocity (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_types) {

      if (Args.Length () > 2) {

         const Vector Value = self->_types->to_dmz_vector (Args[2]);

         if (self->_event->store_velocity (event, attr, Value)) {

            result = self->_types->to_v8_vector (Value);
         }
      }
      else {

         Vector value;

         if (self->_event->lookup_velocity (event, attr, value)) {

            result = self->_types->to_v8_vector (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_acceleration (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_types) {

      if (Args.Length () > 2) {

         const Vector Value = self->_types->to_dmz_vector (Args[2]);

         if (self->_event->store_acceleration (event, attr, Value)) {

            result = self->_types->to_v8_vector (Value);
         }
      }
      else {

         Vector value;

         if (self->_event->lookup_acceleration (event, attr, value)) {

            result = self->_types->to_v8_vector (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_scale (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_types) {

      if (Args.Length () > 2) {

         const Vector Value = self->_types->to_dmz_vector (Args[2]);

         if (self->_event->store_scale (event, attr, Value)) {

            result = self->_types->to_v8_vector (Value);
         }
      }
      else {

         Vector value;

         if (self->_event->lookup_scale (event, attr, value)) {

            result = self->_types->to_v8_vector (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_vector (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_types) {

      if (Args.Length () > 2) {

         const Vector Value = self->_types->to_dmz_vector (Args[2]);

         if (self->_event->store_vector (event, attr, Value)) {

            result = self->_types->to_v8_vector (Value);
         }
      }
      else {

         Vector value;

         if (self->_event->lookup_vector (event, attr, value)) {

            result = self->_types->to_v8_vector (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_scalar (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event) {

      if (Args.Length () > 2) {

         const Float64 Value = v8_to_number (Args[2]);

         if (self->_event->store_scalar (event, attr, Value)) {

            result = v8::Number::New (Value);
         }
      }
      else {

         Float64 value (0.0);

         if (self->_event->lookup_scalar (event, attr, value)) {

            result = v8::Number::New (value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_counter (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event) {

      if (Args.Length () > 2) {

         const Int64 Value = v8_to_handle (Args[2]);

         if (self->_event->store_counter (event, attr, Value)) {

            result = v8::Integer::New ((int32_t)Value);
         }
      }
      else {

         Int64 value (0);

         if (self->_event->lookup_counter (event, attr, value)) {

            result = v8::Integer::New ((int32_t)value);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event) {

      if (Args.Length () > 2) {

         const String Value = v8_to_string (Args[2]);

         if (self->_event->store_text (event, attr, Value)) {

            result = Args[2];
         }
      }
      else {

         String value;

         if (self->_event->lookup_text (event, attr, value)) {

            if (!value.is_null ()) { result = v8::String::New (value.get_buffer ()); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Event::_event_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Event *self (0);
   Handle event (0), attr (0);

   if (_to_event (Args, self, event, attr) && self->_event && self->_runtime) {

      if (Args.Length () > 2) {

         Data value;

         if (self->_runtime->to_dmz_data (Args[2], value)) {

            if (self->_event->store_data (event, attr, value)) {

               result = self->_runtime->create_v8_data (&value);
            }
         }
      }
      else {

         Data value;

         if (self->_event->lookup_data (event, attr, value)) {

            result = self->_runtime->create_v8_data (&value);
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsExtV8Event::_store_event_module (EventModule &module) { _event = &module; }


void
dmz::JsExtV8Event::_remove_event_module (EventModule &) { _event = 0; }


dmz::V8Function
dmz::JsExtV8Event::_register_observer (const v8::Arguments &Args, const Mask &AttrMask) {

   v8::HandleScope scope;

   V8Function result;

   const int Length = Args.Length ();

   if (Length >= 3) {

      V8Object src = v8_to_object (Args[0]);
      const Handle Obs = _core ? _core->get_instance_handle (src) : 0;
      ObsStruct *os = _obsTable.lookup (Obs);

      if (!os) {

         os = new ObsStruct;
         if (os && !_obsTable.store (Obs, os)) { delete os; os = 0; }
      }

      EventType type;
      if (_runtime) { _runtime->to_dmz_event_type (Args[1], type); }
      V8Function func = v8_to_function (Args[2]);

      if (os && Obs && type && (func.IsEmpty () == false)) {

         CallbackTable *ct (0);

         if (AttrMask.contains (EventCreateMask)) {

            ct = _createTable.lookup (type.get_handle ());

            if (!ct) {

               ct = new CallbackTable (type);

               if (ct && !_createTable.store (type.get_handle (), ct)) {

                  delete ct; ct = 0;
               }
            }
         }
         else if (AttrMask.contains (EventCloseMask)) {

            ct = _closeTable.lookup (type.get_handle ());

            if (!ct) {

               ct = new CallbackTable (type);

               if (ct && !_closeTable.store (type.get_handle (), ct)) {

                  delete ct; ct = 0;
               }
            }
         }

         if (ct) {

            if (ct->table.get_count () == 0) {

               activate_event_callback (type, AttrMask);
            }

            CallbackStruct *cs = ct->table.lookup (Obs);

            if (!cs) {

               cs = new CallbackStruct (Obs);
               if (cs && !ct->table.store (Obs, cs)) { delete cs; cs = 0; }
            }

            if (cs) {

               cs->self.Dispose (); cs->self.Clear ();
               cs->self = V8ObjectPersist::New (src);
               cs->func.Dispose (); cs->func.Clear ();
               cs->func = V8FunctionPersist::New (func);
               result = func;

               if (AttrMask.contains (EventCreateMask)) {

                  os->create.store (type.get_handle (), ct);
               }
               else if (AttrMask.contains (EventCloseMask)) {

                  os->close.store (type.get_handle (), ct);
               }
            }
         }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsExtV8Event::_do_callback (
      const Handle EventHandle,
      const EventLocalityEnum Locality,
      CallbackTable *table) {

   if (table && (_v8Context.IsEmpty () == false)) {

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      const int Argc (4);
      V8Value argv[Argc];

      argv[0] = v8::Integer::New (EventHandle);
      argv[1] = v8::Undefined ();
      if (_runtime) { argv[1] = _runtime->create_v8_event_type (&(table->Type)); }
      argv[2] = v8::Integer::New (Locality);

      HashTableHandleIterator it;
      CallbackStruct *cs (0);

      while (table->table.get_next (it, cs)) {

         if ((cs->self.IsEmpty () == false) && (cs->func.IsEmpty () == false)) {

            // Copy the observer handle onto the stack in case the CallbackStruct
            // is deleted in the callback
            const Handle Observer = cs->Observer;

            argv[3] = cs->self;

            v8::TryCatch tc;
            // The CallbackStruct cs may not be valid after this call and should not
            // be referenced after this point.
            cs->func->Call (cs->self, Argc, argv);

            if (tc.HasCaught ()) {

               if (_core) { _core->handle_v8_exception (tc); }

               cs = table->table.remove (Observer);

               if (cs) { delete cs; cs = 0; }
            }
         }
      }
   } 
}


void
dmz::JsExtV8Event::_release_callback (
      const Mask &CBMask,
      const Handle Obs,
      HashTableHandleTemplate<CallbackTable> &table,
      V8Function func) {

   HashTableHandleIterator it;
   CallbackTable *ct (0);

   while (table.get_next (it, ct)) {

      CallbackStruct *cs = ct->table.lookup (Obs);

      if (cs && (func.IsEmpty () || (cs->func == func))) {

         cs = ct->table.remove (Obs);

         if (cs) { delete cs; cs = 0; }

         if (ct->table.get_count () == 0) {

            deactivate_event_callback (ct->Type, EventCreateMask);
         }
      }
   }
}


void
dmz::JsExtV8Event::_init (Config &local) {

   v8::HandleScope scope;

   _defaultAttr = _defs.create_named_handle (EventAttributeDefaultName);

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Constants
   _eventApi.add_constant ("LocalityUnknown", (UInt32)EventLocalityUnknown);
   _eventApi.add_constant ("Local", (UInt32)EventLocal);
   _eventApi.add_constant ("Remote", (UInt32)EventRemote);
   _eventApi.add_constant ("DefaultAttribute", _defaultAttr);
   _eventApi.add_constant (
      "SourceAttribute",
      _defs.create_named_handle (EventAttributeSourceName));
   _eventApi.add_constant (
      "TargetAttribute",
      _defs.create_named_handle (EventAttributeTargetName));
   _eventApi.add_constant (
      "MunitionsAttribute",
      _defs.create_named_handle (EventAttributeMunitionsName));
   // Functions
   _eventApi.add_function ("release", _event_release, _self);
   _eventApi.add_function ("create", _event_create, _self);
   _eventApi.add_function ("create.observe", _event_create_observe, _self);
   _eventApi.add_function ("close", _event_close, _self);
   _eventApi.add_function ("close.observe", _event_close_observe, _self);
   _eventApi.add_function ("type", _event_type, _self);
   _eventApi.add_function ("locality", _event_locality, _self);
   _eventApi.add_function ("handle", _event_handle, _self);
   _eventApi.add_function ("objectHandle", _event_object_handle, _self);
   _eventApi.add_function ("objectType", _event_object_type, _self);
   _eventApi.add_function ("state", _event_state, _self);
   _eventApi.add_function ("timeStamp", _event_time_stamp, _self);
   _eventApi.add_function ("position", _event_position, _self);
   _eventApi.add_function ("orientation", _event_orientation, _self);
   _eventApi.add_function ("velocity", _event_velocity, _self);
   _eventApi.add_function ("acceleration", _event_acceleration, _self);
   _eventApi.add_function ("scale", _event_scale, _self);
   _eventApi.add_function ("vector", _event_vector, _self);
   _eventApi.add_function ("scalar", _event_scalar, _self);
   _eventApi.add_function ("counter", _event_counter, _self);
   _eventApi.add_function ("text", _event_text, _self);
   _eventApi.add_function ("data", _event_data, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Event (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Event (Info, local);
}

};
