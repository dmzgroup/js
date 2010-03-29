#include <dmzEventCallbackMasks.h>
#include <dmzEventConsts.h>
#include <dmzEventModule.h>
#include <dmzEventModuleCommon.h>
#include "dmzJsExtV8EventCommon.h"
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


dmz::JsExtV8EventCommon::JsExtV8EventCommon (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _defs (Info),
      _event (0),
      _common (0),
      _core (0),
      _runtime (0),
      _types (0) {

   _init (local);
}


dmz::JsExtV8EventCommon::~JsExtV8EventCommon () {

}


// Plugin Interface
void
dmz::JsExtV8EventCommon::update_plugin_state (
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
dmz::JsExtV8EventCommon::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_event) { _event = EventModule::cast (PluginPtr); }
      if (!_common) { _common = EventModuleCommon::cast (PluginPtr); }
      if (!_types) { _types = JsModuleTypesV8::cast (PluginPtr); }
      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_event && (_event == EventModule::cast (PluginPtr))) { _event = 0; }
      if (_common && (_common == EventModuleCommon::cast (PluginPtr))) { _common = 0; }
      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8EventCommon::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8EventCommon::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8EventCommon::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/components/event/common",
            _eventApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      _eventApi.clear ();
      _v8Context.Clear ();
   }
}


// Event API Bindings
dmz::V8Value
dmz::JsExtV8EventCommon::_event_close (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8EventCommon *self = _to_self (Args);

   if (self && self->_event) {

      result = v8::Boolean::New (self->_event->close_event (v8_to_handle (Args[0])));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8EventCommon::_event_create_launch (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8EventCommon *self = _to_self (Args);

   if (self && self->_common) {

      const Handle Munition = v8_to_handle (Args[0]);
      const Handle Target = v8_to_handle (Args[1]);

      result = v8::Integer::New (self->_common->create_launch_event (Munition, Target));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8EventCommon::_event_create_open_launch (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8EventCommon *self = _to_self (Args);

   if (self && self->_common) {

      const Handle Munition = v8_to_handle (Args[0]);
      const Handle Target = v8_to_handle (Args[1]);

      result = v8::Integer::New (
         self->_common->create_open_launch_event (Munition, Target));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8EventCommon::_event_create_detonation (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8EventCommon *self = _to_self (Args);

   if (self && self->_common) {

      const Handle Munition = v8_to_handle (Args[0]);
      const Handle Target = v8_to_handle (Args[1]);

      result = v8::Integer::New (
         self->_common->create_detonation_event (Munition, Target));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8EventCommon::_event_create_open_detonation (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8EventCommon *self = _to_self (Args);

   if (self && self->_common) {

      const Handle Munition = v8_to_handle (Args[0]);
      const Handle Target = v8_to_handle (Args[1]);

      result = v8::Integer::New (
         self->_common->create_open_detonation_event (Munition, Target));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8EventCommon::_event_create_collision (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8EventCommon *self = _to_self (Args);

   if (self && self->_common) {

      const Handle Source = v8_to_handle (Args[0]);
      const Handle Target = v8_to_handle (Args[1]);

      result = v8::Integer::New (self->_common->create_collision_event (Source, Target));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8EventCommon::_event_create_open_collision (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8EventCommon *self = _to_self (Args);

   if (self && self->_common) {

      const Handle Source = v8_to_handle (Args[0]);
      const Handle Target = v8_to_handle (Args[1]);

      result = v8::Integer::New (
         self->_common->create_open_collision_event (Source, Target));
   }

   return scope.Close (result);
}


void
dmz::JsExtV8EventCommon::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Functions
   _eventApi.add_function ("close", _event_close, _self);
   _eventApi.add_function ("createLaunch", _event_create_launch, _self);
   _eventApi.add_function ("createOpenLaunch", _event_create_open_launch, _self);
   _eventApi.add_function ("createDetonation", _event_create_detonation, _self);
   _eventApi.add_function ("createOpenDetonation", _event_create_open_detonation, _self);
   _eventApi.add_function ("createCollision", _event_create_collision, _self);
   _eventApi.add_function ("createOpenCollision", _event_create_open_collision, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8EventCommon (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8EventCommon (Info, local);
}

};
