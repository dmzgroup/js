#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsModuleRuntimeV8Basic::JsModuleRuntimeV8Basic (
      const PluginInfo &Info,
      Config &local) :
      Plugin (Info),
      JsModuleRuntimeV8 (Info),
      JsExtV8 (Info),
      _log (Info),
      _time (Info),
      _defs (Info, &_log),
      _undo (Info),
      _convertBool (Info),
      _convertNum (Info),
      _convertStr (Info),
      _convertHandle (Info),
      _core (0),
      _types (0) {

   _init (local);
}


dmz::JsModuleRuntimeV8Basic::~JsModuleRuntimeV8Basic () {

}


// Plugin Interface
void
dmz::JsModuleRuntimeV8Basic::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

      _self.Dispose (); _self.Clear ();
   }
}


void
dmz::JsModuleRuntimeV8Basic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_types) {

         _types = JsModuleTypesV8::cast (PluginPtr);
      }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) {

         _types = 0;
      }
   }
}


// JsExtV8 Interface
void
dmz::JsModuleRuntimeV8Basic::update_js_module_v8 (
      const ModeEnum Mode,
      JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { 

      if (!_core) { _core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {

      if (_core == &module) { _core = 0; }
   }
}


void
dmz::JsModuleRuntimeV8Basic::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsModuleRuntimeV8Basic::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_configFuncTemplate.IsEmpty () == false) {

         _configFunc = V8FunctionPersist::New (_configFuncTemplate->GetFunction ());
      }

      if (_dataFuncTemplate.IsEmpty () == false) {

         _dataFunc = V8FunctionPersist::New (_dataFuncTemplate->GetFunction ());
      }

      if (_eventTypeFuncTemplate.IsEmpty () == false) {

         _eventTypeFunc = V8FunctionPersist::New (_eventTypeFuncTemplate->GetFunction ());
      }

      if (_logFuncTemplate.IsEmpty () == false) {

         _logFunc = V8FunctionPersist::New (_logFuncTemplate->GetFunction ());
      }

      if (_msgFuncTemplate.IsEmpty () == false) {

         _msgFunc = V8FunctionPersist::New (_msgFuncTemplate->GetFunction ());
      }

      if (_objTypeFuncTemplate.IsEmpty () == false) {

         _objTypeFunc = V8FunctionPersist::New (_objTypeFuncTemplate->GetFunction ());
      }

      if (_core) {

         _core->register_interface (
            "dmz/runtime/config",
            _configApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/data",
            _dataApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/definitions",
            _defsApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/eventType",
            _eventTypeApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/messaging",
            _msgApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/objectType",
            _objTypeApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/time",
            _timeApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/undo",
            _undoApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      _configFunc.Dispose (); _configFunc.Clear ();
      _dataFunc.Dispose (); _dataFunc.Clear ();
      _eventTypeFunc.Dispose (); _eventTypeFunc.Clear ();
      _logFunc.Dispose (); _logFunc.Clear ();
      _msgFunc.Dispose (); _msgFunc.Clear ();
      _objTypeFunc.Dispose (); _objTypeFunc.Clear ();

      _configApi.clear (); 
      _dataApi.clear (); 
      _defsApi.clear (); 
      _eventTypeApi.clear (); 
      _msgApi.clear (); 
      _objTypeApi.clear (); 
      _timeApi.clear (); 
      _undoApi.clear (); 

      _reset_time ();
      _reset_messaging ();

      _v8Context.Clear ();
   }
}


void
dmz::JsModuleRuntimeV8Basic::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

   _release_message_observer (InstanceHandle);
}


// JsModuleRuntimeV8Basic Interface
void
dmz::JsModuleRuntimeV8Basic::handle_v8_exception (v8::TryCatch &tc) {

   if (_core) { _core->handle_v8_exception (tc); }
}


dmz::Handle
dmz::JsModuleRuntimeV8Basic::_to_handle (V8Value value) {

   Handle result (0);

   if (value.IsEmpty () == false) {

      if (value->IsInt32 ()) { result = value->Uint32Value (); }
      else if (value->IsString ()) {

         const String Str = *(v8::String::AsciiValue (value));

         if (Str) { result = _defs.create_named_handle (Str); }
      }
   }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   _init_config ();
   _init_data ();
   _init_definitions ();
   _init_event_type ();
   _init_log ();
   _init_messaging ();
   _init_object_type ();
   _init_time ();
   _init_undo ();
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleRuntimeV8Basic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleRuntimeV8Basic (Info, local);
}

};
