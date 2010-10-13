#include "dmzJsExtV8Session.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzRuntimeSession.h>

dmz::JsExtV8Session::JsExtV8Session (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _runtime (0) {

   _init (local);
}


dmz::JsExtV8Session::~JsExtV8Session () {

}


// Plugin Interface
void
dmz::JsExtV8Session::update_plugin_state (
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
dmz::JsExtV8Session::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Session::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Session::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Session::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/runtime/session",
            _sessionApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _sessionApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Session::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {;}


// JsExtV8Session Interface
dmz::V8Value
dmz::JsExtV8Session::_session_get (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Session *self = _to_self (Args);

   if (self && self->_runtime && (Args.Length () > 0)) {

      V8Value value = Args[0];

      if (value.IsEmpty () == false) {

         String name;

         if (value->IsString ()) { name = v8_to_string (value); }
         else if (value->IsObject () && self->_core) {

            name = self->_core->get_instance_name (value);
         }

         if (name) {

            Config data = get_session_config (name, self->get_plugin_runtime_context ());

            result = self->_runtime->create_v8_config (&data);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Session::_session_set (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Session *self = _to_self (Args);

   if (self && self->_runtime && (Args.Length () > 0)) {

      Config data;
      self->_runtime->to_dmz_config (Args[0], data);
      if (data) { set_session_config (self->get_plugin_runtime_context (), data); }

      result = self->_runtime->create_v8_config (&data);
   }

   return scope.Close (result);
}


void
dmz::JsExtV8Session::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _sessionApi.add_function ("get", _session_get, _self);
   _sessionApi.add_function ("set", _session_set, _self);

}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Session (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Session (Info, local);
}

};
