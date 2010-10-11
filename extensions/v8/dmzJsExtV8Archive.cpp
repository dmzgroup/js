#include <dmzArchiveModule.h>
#include "dmzJsExtV8Archive.h"
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

namespace {

static const char LocalPre[] = "pre";
static const char LocalPost[] = "post";
};


dmz::JsExtV8Archive::JsExtV8Archive (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      ArchiveObserverUtil (Info, local),
      _log (Info),
      _defs (Info),
      _defaultArchive (0),
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

      _preCreateTable.empty ();
      _createTable.empty ();
      _postCreateTable.empty ();

      _preProcessTable.empty ();
      _processTable.empty ();
      _postProcessTable.empty ();

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

   CallbackTable *ct = _preCreateTable.lookup (ArchiveHandle);

   if (ct) { _do_pre_post_callback (ArchiveHandle, Version, *ct); }
}

   
void
dmz::JsExtV8Archive::create_archive (
      const Handle ArchiveHandle,
      const Int32 Version,
      Config &local,
      Config &global) {

   CallbackTable *ct = _createTable.lookup (ArchiveHandle);

   if (ct && (_v8Context.IsEmpty () == false) && _runtime) {

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      const int Argc = 5;
      V8Value argv[Argc];
      argv[1] = v8::Integer::New (Version);
      argv[2] = v8::Integer::NewFromUnsigned (ArchiveHandle);
      argv[3] = _runtime->create_v8_config (&global);

      HashTableHandleIterator it;
      CallbackStruct *cb (0);

      while (ct->table.get_next (it, cb)) {

         if ((cb->self.IsEmpty () == false) && (cb->func.IsEmpty () == false)) {

            // Copy self and func onto stack because CallbackStruct  may not be valid
            // after the callback is made.
            V8Object localSelf = v8::Local<v8::Object>::New (cb->self);
            V8Function localFunc = v8::Local<v8::Function>::New (cb->func);
            const Handle Instance = cb->is.Instance;

            v8::TryCatch tc;

            Config data (cb->is.Name);
            argv[0] = _runtime->create_v8_config (&data);
            argv[Argc - 1] = localSelf;
            // CallbackStruct cs may not be valid after this call and should not be
            // referenced after this point.
            localFunc->Call (localSelf, Argc, argv);

            global.add_config (data);

            if (tc.HasCaught ()) {

               if (_core) { _core->handle_v8_exception (it.get_hash_key (), tc); }

               cb = ct->table.lookup (Instance);

               if (cb) { delete cb; cb = 0; }
            }
         }
      }
   }
}

         
void
dmz::JsExtV8Archive::post_create_archive (
      const Handle ArchiveHandle,
      const Int32 Version) {

   CallbackTable *ct = _postCreateTable.lookup (ArchiveHandle);

   if (ct) { _do_pre_post_callback (ArchiveHandle, Version, *ct); }
}

         
void
dmz::JsExtV8Archive::pre_process_archive (
      const Handle ArchiveHandle,
      const Int32 Version) {

   CallbackTable *ct = _preProcessTable.lookup (ArchiveHandle);

   if (ct) { _do_pre_post_callback (ArchiveHandle, Version, *ct); }
}

         
void
dmz::JsExtV8Archive::process_archive (
      const Handle ArchiveHandle,
      const Int32 Version,
      Config &local,
      Config &global) {

   CallbackTable *ct = _createTable.lookup (ArchiveHandle);

   if (ct && (_v8Context.IsEmpty () == false) && _runtime) {

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      const int Argc = 5;
      V8Value argv[Argc];
      argv[1] = v8::Integer::New (Version);
      argv[2] = v8::Integer::NewFromUnsigned (ArchiveHandle);
      argv[3] = _runtime->create_v8_config (&global);

      HashTableHandleIterator it;
      CallbackStruct *cb (0);

      while (ct->table.get_next (it, cb)) {

         if ((cb->self.IsEmpty () == false) && (cb->func.IsEmpty () == false)) {

            // Copy self and func onto stack because CallbackStruct  may not be valid
            // after the callback is made.
            V8Object localSelf = v8::Local<v8::Object>::New (cb->self);
            V8Function localFunc = v8::Local<v8::Function>::New (cb->func);
            const Handle Instance = cb->is.Instance;

            v8::TryCatch tc;

            Config data (cb->is.Name);
            global.lookup_all_config_merged (cb->is.Name, data);
            argv[0] = _runtime->create_v8_config (&data);
            argv[Argc - 1] = localSelf;
            // CallbackStruct cs may not be valid after this call and should not be
            // referenced after this point.
            localFunc->Call (localSelf, Argc, argv);

            if (tc.HasCaught ()) {

               if (_core) { _core->handle_v8_exception (it.get_hash_key (), tc); }

               cb = ct->table.lookup (Instance);

               if (cb) { delete cb; cb = 0; }
            }
         }
      }
   }
}


void
dmz::JsExtV8Archive::post_process_archive (
      const Handle ArchiveHandle,
      const Int32 Version) {

   CallbackTable *ct = _postProcessTable.lookup (ArchiveHandle);

   if (ct) { _do_pre_post_callback (ArchiveHandle, Version, *ct); }
}


// JsExtV8Archive Interface
dmz::V8Value
dmz::JsExtV8Archive::_archive_observe (
      const v8::Arguments &Args,
      JsExtV8Archive &self,
      HashTableHandleTemplate<CallbackTable> &preTable,
      HashTableHandleTemplate<CallbackTable> &table,
      HashTableHandleTemplate<CallbackTable> &postTable) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (self._core) {

      const int Length = Args.Length ();

      V8Object obj = v8_to_object (Args[0]);
      const Handle Instance (self._core->get_instance_handle (obj));
      const String Name (self._core->get_instance_name (obj));
      Handle archive (self._defaultArchive);
      String type;
      V8Function func;

      if (Length == 2) { func = v8_to_function (Args[1]); }
      else if (Length == 3) {

         archive = self._to_handle (Args[1]);
         func = v8_to_function (Args[2]);
      }
      else if (Length >= 4) {

         archive = self._to_handle (Args[1]);
         type = v8_to_string (Args[2]);
         func = v8_to_function (Args[3]);
      }

      if (self._register_callback (
            Instance,
            Name,
            archive,
            obj,
            func,
            (type == LocalPre ? preTable : (type == LocalPost ? postTable : table)))) {

         result = func;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_release (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self && self->_core) {

      const Handle Instance = self->_core->get_instance_handle (Args[0]);

      if (Args.Length () > 1) {

         V8Value value = Args[1];

         if (value->IsFunction ()) {

            V8Function func = v8_to_function (value);

            InstanceStruct *is = self->_instanceTable.lookup (Instance);

            if (is) {

               CallbackStruct *current = is->list;

               while (current) {

                  if (current->func == func) {

                     delete current; current = 0;
                  }
                  else { current = current->next; }
               }
            }
         }
         else {

            if (value->IsString ()) {

            }
         }
      }
      else {

         InstanceStruct *is = self->_instanceTable.remove (Instance);

         if (is) { delete is; is = 0; }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_create (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self && self->_archive && self->_runtime) {

      Config archive = self->_archive->create_archive (self->_to_handle (Args[0]));

      if (archive) { result = self->_runtime->create_v8_config (&archive); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_create_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self) {

      result = _archive_observe (
         Args,
         *self,
         self->_preCreateTable,
         self->_createTable,
         self->_postCreateTable);
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

         self->_archive->process_archive (self->_to_handle (Args[0]), archive);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Archive::_archive_process_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Archive *self = _to_self (Args);

   if (self) {

      result = _archive_observe (
         Args,
         *self,
         self->_preProcessTable,
         self->_processTable,
         self->_postProcessTable);
   }

   return scope.Close (result);
}


dmz::Handle
dmz::JsExtV8Archive::_to_handle (V8Value value) {

   Handle result (_defaultArchive);

   if (value.IsEmpty () == false) {

      if (value->IsNumber ()) { result = v8_to_handle (value); }
      else if (value->IsString ()) {

         result = _defs.create_named_handle (v8_to_string (value));
      }
   }

   return result;
}


dmz::Boolean
dmz::JsExtV8Archive::_register_callback (
      const Handle Instance,
      const String &Name,
      const Handle Archive,
      V8Object self,
      V8Function func,
      HashTableHandleTemplate<CallbackTable> &table) {

   Boolean result (False);

   if (Instance && Archive && (func.IsEmpty () == false)) {

      InstanceStruct *is = _instanceTable.lookup (Instance);

      if (!is) {

         is = new InstanceStruct (Instance, Name);

         if (!_instanceTable.store (Instance, is)) { delete is; is = 0; }
      }

      if (is) {

         CallbackTable *ct = table.lookup (Archive);

         if (!ct) {

            ct = new CallbackTable (Archive);

            if (!table.store (Archive, ct)) { delete ct; ct = 0; }
         }

         if (ct) {

            CallbackStruct *cs = ct->table.lookup (Instance);

            if (!cs) {

               cs = new CallbackStruct (*is, *ct);

               if (ct->table.store (Instance, cs)) {

                  cs->next = is->list;
                  is->list = cs;
               }
               else if (cs) { delete cs; cs = 0; }
            }

            if (cs) {

               result = True;

               cs->self.Dispose (); cs->self.Clear ();
               cs->func.Dispose (); cs->func.Clear ();

               cs->self = V8ObjectPersist::New (self);
               cs->func = V8FunctionPersist::New (func);

               if (!is_active_archive_handle (Archive)) {

                  activate_archive (Archive);
               }
            }
         }
      }
   }

   return result;
}


void
dmz::JsExtV8Archive::_do_pre_post_callback (
      const Handle ArchiveHandle,
      const Int32 Version,
      CallbackTable &ct) {

   if (_v8Context.IsEmpty () == false) {

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      const int Argc = 3;
      V8Value argv[Argc];
      argv[0] = v8::Integer::NewFromUnsigned (ArchiveHandle);
      argv[1] = v8::Integer::New (Version);

      HashTableHandleIterator it;
      CallbackStruct *cb (0);

      while (ct.table.get_next (it, cb)) {

         if ((cb->self.IsEmpty () == false) && (cb->func.IsEmpty () == false)) {

            // Copy self and func onto stack because CallbackStruct  may not be valid
            // after the callback is made.
            V8Object localSelf = v8::Local<v8::Object>::New (cb->self);
            V8Function localFunc = v8::Local<v8::Function>::New (cb->func);
            const Handle Instance = cb->is.Instance;

            v8::TryCatch tc;

            argv[Argc - 1] = localSelf;
            // CallbackStruct cs may not be valid after this call and should not be
            // referenced after this point.
            localFunc->Call (localSelf, Argc, argv);

            if (tc.HasCaught ()) {

               if (_core) { _core->handle_v8_exception (it.get_hash_key (), tc); }

               cb = ct.table.lookup (Instance);

               if (cb) { delete cb; cb = 0; }
            }
         }
      }
   }
}


void
dmz::JsExtV8Archive::_init (Config &local) {

   _defaultArchive = _defs.create_named_handle (
      config_to_string ("default-archive.name", local, ArchiveDefaultName));

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _archiveApi.add_constant ("PreCallback", LocalPre);
   _archiveApi.add_constant ("PostCallback", LocalPost);
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
