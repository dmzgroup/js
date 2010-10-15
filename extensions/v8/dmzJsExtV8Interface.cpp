#include "dmzJsExtV8Interface.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzSystemFile.h>

namespace {

const char LocalActivate[] = "Activate";
const char LocalDeactivate[] = "Deactivate";

};

dmz::JsExtV8Interface::JsExtV8Interface (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0) {

   _init (local);
}


dmz::JsExtV8Interface::~JsExtV8Interface () {

}


// Plugin Interface
void
dmz::JsExtV8Interface::update_plugin_state (
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
dmz::JsExtV8Interface::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Interface::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Interface::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Interface::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/runtime/interface",
            _interfaceApi.get_new_instance ());
      }

      _activateStr = V8StringPersist::New (v8::String::NewSymbol (LocalActivate));
      _deactivateStr = V8StringPersist::New (v8::String::NewSymbol (LocalDeactivate));
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      _activateStr.Dispose (); _activateStr.Clear ();
      _deactivateStr.Dispose (); _deactivateStr.Clear ();
      _interfaceApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Interface::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {;}


// JsExtV8Interface Interface
dmz::V8Value
dmz::JsExtV8Interface::_interface_publish (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Interface *self = _to_self (Args);

   if (self && self->_core) {

      const int Length = Args.Length ();

      V8Object obj = v8_to_object (Args[0]);
      const String ObjName = self->_core->get_instance_name (obj);
      const Handle ObjHandle = self->_core->get_instance_handle (obj);
      V8Object instance = v8_to_object (Args[1]);
      const String Name = Length > 2 ? v8_to_string (Args[2]) : ObjName;
      const String Scope = Length > 3 ? v8_to_string (Args[3]) : ObjName;

      if (ObjHandle && v8_is_valid (obj) && v8_is_valid (instance)) {

         SelfStruct *ss = self->_lookup_self_struct (ObjHandle);
         InterfaceStruct *is = self->_lookup_interface (Name);

         if (ss && is) {

            InstanceStruct *instance = new InstanceStruct (Name, Scope, *is);

            if (instance && self->_register_instance (*is, *instance)) {

               instance->next = ss->instanceList;
               ss->instanceList = instance;
            }
            else if (instance) { delete instance; instance = 0; }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Interface::_interface_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Interface *self = _to_self (Args);

   if (self) {

   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Interface::_interface_subscribe (const v8::Arguments &Args) {

   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   JsExtV8Interface *self = _to_self (Args);

   if (self && self->_core) {

      const int Length = Args.Length ();

      V8Object obj = v8_to_object (Args[0]);
      const String Name = v8_to_string (Args[1]);
      const String Scope = Length > 3 ? v8_to_string (Args[2]) : String ();
      V8Function func = v8_to_function (Args[Length > 3 ? 3 : 2]);

      if (v8_is_valid (obj) && v8_is_valid (func) && Name) {

         const Handle Object = self->_core->get_instance_handle (obj);

         if (Object) {

            SelfStruct *ss = self->_selfTable.lookup (Object);

            if (!ss) {

               ss = new SelfStruct;

               if (ss && !self->_selfTable.store (Object, ss)) { delete ss; ss = 0; }
            }

            if (ss) {

               InterfaceStruct *is = self->_lookup_interface (Name);

               if (is) {

                  SubscribeStruct *ds = new SubscribeStruct (Object, Scope, *is);

                  if (ds) {

                     ds->self = V8ObjectPersist::New (obj);
                     ds->func = V8FunctionPersist::New (func);

                     if (self->_register_subscribe (*is, *ds)) {

                        ds->next = ss->subscribeList;
                        ss->subscribeList = ds;
                        result = func;
                     }
                     else { delete ds; ds = 0; }
                  }
               }
            }
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsExtV8Interface::_subscribe_interface (
      const Boolean Activate,
      InterfaceStruct &is,
      SubscribeStruct &ds) {

   if (v8_is_valid (ds.func) && (is.table.get_size () > 0) &&
         (_v8Context.IsEmpty () == false)) {

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      const int Argc (5);
      V8Value argv[Argc];
      argv[0] = Activate ? _activateStr : _deactivateStr;
      argv[3] = v8::String::New (is.Name ? is.Name.get_buffer () : "");

      HashTableStringIterator it;
      InstanceStruct *instance (0);

      const Handle Object = ds.Object;
      V8Object self = v8::Local<v8::Object>::New (ds.self);
      V8Function func = v8::Local<v8::Function>::New (ds.func);
      argv[4] = self;

      while (is.table.get_next (it, instance)) {

         if (!ds.Scope || (ds.Scope == instance->Scope)) {

            argv[1] = instance->interface;
            argv[2] = v8::String::New (instance->Scope.get_buffer ());

            v8::TryCatch tc;
            func->Call (self, Argc, argv);

            if (tc.HasCaught ()) {

               if (_core) { _core->handle_v8_exception (Object, tc); }
               SelfStruct *ss = _selfTable.lookup (Object);

               if (ss) { ss->remove (&ds); }
            }
         }
      }
   }
}


dmz::Boolean
dmz::JsExtV8Interface::_register_instance (InterfaceStruct &is, InstanceStruct &ds) {

   Boolean result (False);

   if (is.table.store (ds.Scope, &ds)) {

      HashTableHandleIterator it;
      SubscribeStruct *ss (0);

      while (is.subscribeTable.get_next (it, ss)) {

      }
   }

   return result;
}


dmz::Boolean
dmz::JsExtV8Interface::_register_subscribe (InterfaceStruct &is, SubscribeStruct &ds) {

   Boolean result (False);

   if (is.subscribeTable.store (ds.Object, &ds)) {

      _subscribe_interface (True, is ,ds);

      result = True;
   }

   return result;
}


dmz::JsExtV8Interface::InterfaceStruct *
dmz::JsExtV8Interface::_lookup_interface (const String &Name) {

   InterfaceStruct *result (_interfaceTable.lookup (Name));

   if (!result) {

      result = new InterfaceStruct (Name);

      if (result && !_interfaceTable.store (Name, result)) { delete result; result = 0; }
   }

   return result;
}


dmz::JsExtV8Interface::SelfStruct *
dmz::JsExtV8Interface::_lookup_self_struct (const Handle Object) {

   SelfStruct *result (_selfTable.lookup (Object));

   if (!result) {

      result = new SelfStruct;

      if (result && !_selfTable.store (Object, result)) { delete result; result = 0; }
   }

   return result;
}


void
dmz::JsExtV8Interface::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _interfaceApi.add_constant ("Activate", LocalActivate);
   _interfaceApi.add_constant ("Deactivate", LocalDeactivate);
   _interfaceApi.add_function ("publish", _interface_publish, _self);
   _interfaceApi.add_function ("remove", _interface_remove, _self);
   _interfaceApi.add_function ("subscribe", _interface_subscribe, _self);

}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Interface (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Interface (Info, local);
}

};
