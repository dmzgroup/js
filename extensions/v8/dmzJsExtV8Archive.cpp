#include <dmzArchiveModule.h>
#include "dmzJsExtV8Archive.h"
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsExtV8Archive::JsExtV8Archive (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      ArchiveObserverUtil (Info, local),
      _log (Info),
      _archive (0),
      _core (0),
      _runtime (0) {

   _init (local);
}


dmz::JsExtV8Archive::~JsExtV8Archive () {

}


// Plugin Interface
void
dmz::JsExtV8Archive::update_plugin_state (
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
dmz::JsExtV8Archive::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_archive) { _archive = ArchiveModule::cast (PluginPtr); }
      if (!_runtime) { _runtime = JsModuleRuntimeV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_archive && (_archive = ArchiveModule::cast (PluginPtr))) { _archive = 0; }
      if (_runtime && (_runtime == JsModuleRuntimeV8::cast (PluginPtr))) { _runtime = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Archive::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Archive::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Archive::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/components/archive",
            _archiveApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _instanceTable.empty ();
      _archiveApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Archive::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

   InstanceStruct *is = _instanceTable.remove (InstanceHandle);

   if (is) { delete is; is = 0; }
}


// ArchiveObserver Interface.
void
dmz::JsExtV8Archive::pre_create_archive (
      const Handle ArchiveHandle,
      const Int32 Version) {

}

   
void
dmz::JsExtV8Archive::create_archive (
      const Handle ArchiveHandle,
      const Int32 Version,
      Config &local,
      Config &global) {

}

         
void
dmz::JsExtV8Archive::post_create_archive (
      const Handle ArchiveHandle,
      const Int32 Version) {

}

         
void
dmz::JsExtV8Archive::pre_process_archive (
      const Handle ArchiveHandle,
      const Int32 Version) {

}

         
void
dmz::JsExtV8Archive::process_archive (
      const Handle ArchiveHandle,
      const Int32 Version,
      Config &local,
      Config &global) {

}


void
dmz::JsExtV8Archive::post_process_archive (
      const Handle ArchiveHandle,
      const Int32 Version) {

}


// JsExtV8Archive Interface
dmz::V8Value
dmz::JsExtV8Archive::_archive_release (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self && self->_archive && self->_runtime) {

   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_create (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self && self->_archive && self->_runtime) {

      Config archive = self->_archive->create_archive (v8_to_handle (Args[0]));

      if (archive) { result = self->_runtime->create_v8_config (&archive); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_create_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self && self->_archive && self->_runtime) {

   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_process (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self && self->_archive && self->_runtime) {

      Config archive;

      if (self->_runtime->to_dmz_config (Args[1], archive)) {

         self->_archive->process_archive (v8_to_handle (Args[0]), archive);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_process_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self && self->_archive && self->_runtime) {

   }

   return scope.Close (result);
}


void
dmz::JsExtV8Archive::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _archiveApi.add_function ("release", _archive_release, _self);
   _archiveApi.add_function ("create", _archive_create, _self);
   _archiveApi.add_function ("create.observe", _archive_create_observe, _self);
   _archiveApi.add_function ("process", _archive_process, _self);
   _archiveApi.add_function ("process.observe", _archive_process_observe, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Archive (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Archive (Info, local);
}

};
