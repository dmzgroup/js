#include "dmzJsExtV8Resources.h"
#include <dmzJsModuleV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzRuntimeResources.h>
#include <dmzRuntimeResourcesUpdate.h>


dmz::JsExtV8Resources::JsExtV8Resources (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _runtime (0) {

   _init (local);
}


dmz::JsExtV8Resources::~JsExtV8Resources () {

}


// Plugin Interface
void
dmz::JsExtV8Resources::update_plugin_state (
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
dmz::JsExtV8Resources::discover_plugin (
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
dmz::JsExtV8Resources::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Resources::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Resources::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/runtime/resources",
            _resourcesApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _resourcesApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Resources::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {;}


// JsExtV8Resources Interface
dmz::V8Value
dmz::JsExtV8Resources::_rc_find_file (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Resources *self = _to_self (Args);
   if (self && (Args.Length () > 0)) {

      const String Name (v8_to_string (Args[0]));
      if (Name) {

         Resources rc (self->get_plugin_runtime_context ());
         const String FileName (rc.find_file (Name));

         if (FileName) {

            result = v8::String::New (FileName.get_buffer ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Resources::_rc_store_config (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Resources *self = _to_self (Args);
   if (self && self->_runtime && (Args.Length () > 1)) {

      const String Name (v8_to_string (Args[0]));

      Config resource;
      self->_runtime->to_dmz_config (Args[1], resource);

      if (resource) {

         ResourcesUpdate rc (self->get_plugin_runtime_context ());
         if (rc.update_resource (Name, resource)) {

            result = v8::Boolean::New (True);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Resources::_rc_lookup_config (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Resources *self = _to_self (Args);
   if (self && self->_runtime && (Args.Length () > 0)) {

      const String Name (v8_to_string (Args[0]));
      if (Name) {

         Resources rc (self->get_plugin_runtime_context ());
         Config resource;

         if (rc.lookup_resource_config (Name, resource)) {

            result = self->_runtime->create_v8_config (&resource);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Resources::_rc_remove_config (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Resources *self = _to_self (Args);
   if (self && self->_runtime && (Args.Length () > 0)) {

      const String Name (v8_to_string (Args[0]));
      if (Name) {

         ResourcesUpdate rc (self->get_plugin_runtime_context ());
         if (rc.remove_resource (Name)) {

            result = v8::Boolean::New (True);
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsExtV8Resources::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _resourcesApi.add_function ("findFile", _rc_find_file, _self);
   _resourcesApi.add_function ("storeConfig", _rc_store_config, _self);
   _resourcesApi.add_function ("lookupConfig", _rc_lookup_config, _self);
   _resourcesApi.add_function ("removeConfig", _rc_remove_config, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Resources (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Resources (Info, local);
}

};
