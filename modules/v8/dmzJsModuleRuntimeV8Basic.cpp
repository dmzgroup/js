#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

#include <qdb.h>
static dmz::qdb out;

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

      _dataFunc.Dispose (); _dataFunc.Clear ();

      if (_dataFuncTemplate.IsEmpty () == false) {

         _dataFunc = V8FunctionPersist::New (_dataFuncTemplate->GetFunction ());
      }

      _logFunc.Dispose (); _logFunc.Clear ();

      if (_logFuncTemplate.IsEmpty () == false) {

         _logFunc = V8FunctionPersist::New (_logFuncTemplate->GetFunction ());
      }

      _msgFunc.Dispose (); _msgFunc.Clear ();

      if (_msgFuncTemplate.IsEmpty () == false) {

         _msgFunc = V8FunctionPersist::New (_msgFuncTemplate->GetFunction ());
      }

      _logFunc.Dispose (); _logFunc.Clear ();

      if (_logFuncTemplate.IsEmpty () == false) {

         _logFunc = V8FunctionPersist::New (_logFuncTemplate->GetFunction ());
      }

      if (_core) {

         _core->register_interface (
            "dmz/runtime/data",
            _dataApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/definitions",
            _defsApi.get_new_instance ());

         _core->register_interface (
            "dmz/runtime/message",
            _msgApi.get_new_instance ());

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

      _v8Context.Clear ();
   }
}


// JsModuleRuntimeV8Basic Interface
void
dmz::JsModuleRuntimeV8Basic::handle_v8_exception (v8::TryCatch &tc) {

   if (_core) { _core->handle_v8_exception (tc); }
}


void
dmz::JsModuleRuntimeV8Basic::_clear () {

}


void
dmz::JsModuleRuntimeV8Basic::_reset () {

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


dmz::Int32
dmz::JsModuleRuntimeV8Basic::_to_int32 (V8Value value, const Int32 Default) {

   Int32 result (Default);

   v8::Handle<v8::Integer> arg = v8::Handle<v8::Integer>::Cast (value);
   if (arg.IsEmpty () == false) { result = arg->Value (); }

   return result;
}


void
dmz::JsModuleRuntimeV8Basic::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   _init_data ();
   _init_definitions ();
   _init_log ();
   _init_messaging ();
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
