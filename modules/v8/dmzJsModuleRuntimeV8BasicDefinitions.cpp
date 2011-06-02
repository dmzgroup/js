#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>

#include <qdb.h>
static dmz::qdb out;


// Definitions bindings
dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_create_named_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const String Name = v8_to_string (Args[0]);

      if (Name) {

         Handle val = self->_defs.create_named_handle (Name);
         result = v8::Number::New ((double)val);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_lookup_named_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const String Name = v8_to_string (Args[0]);

      if (Name) {

         result = v8::Number::New ((double)self->_defs.lookup_named_handle (Name));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_lookup_named_handle_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const UInt32 Value = v8_to_uint32 (Args[0]);

      if (Value) {

         const String Name = self->_defs.lookup_named_handle_name (Value);

         if (Name) { result = v8::String::New (Name.get_buffer ()); }
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_lookup_state (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_types) {

      const String Name = v8_to_string (Args[0]);

      Mask state;

      if (self->_defs.lookup_state (Name, state)) {

         result = self->_types->to_v8_mask (state);
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_lookup_state_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_types) {

      const Mask State = self->_types->to_dmz_mask (Args[0]);
      String name;
      self->_defs.lookup_state_name (State, name);
      if (name) { result = v8::String::New (name.get_buffer ()); }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsModuleRuntimeV8Basic::_init_definitions () {

   v8::HandleScope scope;

   _defsApi.add_constant ("DeadStateName", DefaultStateNameDead);
   _defsApi.add_constant ("DeactivateStateName", DefaultStateNameDeactivate);
   _defsApi.add_constant ("SmokingStateName", DefaultStateNameSmoking);
   _defsApi.add_constant ("FireStateName", DefaultStateNameFire);
   _defsApi.add_constant ("DustTrailStateName", DefaultStateNameDustTrail);
   _defsApi.add_constant ("AirBornStateName", DefaultStateNameAirBorn);
   _defsApi.add_function ("createNamedHandle", _create_named_handle, _self);
   _defsApi.add_function ("lookupNamedHandle", _lookup_named_handle, _self);
   _defsApi.add_function ("lookupNamedHandleName", _lookup_named_handle_name, _self);
   _defsApi.add_function ("lookupState", _lookup_state, _self);
   _defsApi.add_function ("lookupStateName", _lookup_state_name, _self);

   UInt32 OS = 0;
#ifdef _WIN32
   OS = 1;
#endif
#ifdef __APPLE__
   OS = 2;
#endif
   _defsApi.add_constant ("OperatingSystem", OS);
   _defsApi.add_constant ("Win32", (UInt32)1);
   _defsApi.add_constant ("OSX", (UInt32)2);
}
