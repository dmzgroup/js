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

      HashTableHandleIterator it;
      SelfStruct *ss (0);

      while (_selfTable.get_next (it, ss)) {

         SubscribeStruct *current (ss->subscribeList);

         while (current) {

            _subscribe_interface (False, current->is, *current);
            current = current->next;
         }
      }

      _selfTable.empty ();
      _interfaceTable.empty ();
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
      v8::Handle<v8::Object> &instance) {

   SelfStruct *ss (_selfTable.lookup (InstanceHandle));

   if (ss) {

      SubscribeStruct *current (ss->subscribeList);

      while (current) {

         _subscribe_interface (False, current->is, *current);
         current = current->next;
      }     

      delete_list (ss->subscribeList);

      InstanceStruct *instance (ss->instanceList);

      while (instance) {

         HashTableHandleIterator it;
         current = 0;

         while (instance->is.subscribeTable.get_next (it, current)) {

            _do_callback (False, *instance, *current);
         }

         instance = instance->next;
      }

      ss = _selfTable.remove (InstanceHandle);

      if (ss) { delete ss; ss = 0; }
   }
}


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
      V8Object exportInterface = v8_to_object (Args[1]);
      const String Name = Length > 2 ? v8_to_string (Args[2]) : ObjName;
      const String Scope = Length > 3 ? v8_to_string (Args[3]) : ObjName;

      if (ObjHandle && v8_is_valid (obj) && v8_is_valid (exportInterface)) {

         SelfStruct *ss = self->_lookup_self_struct (ObjHandle);
         InterfaceStruct *interface = self->_lookup_interface (Name);

         if (ss && interface) {

            InstanceStruct *instance = new InstanceStruct (Name, Scope, *interface);

            if (instance && self->_register_instance (*interface, *instance)) {

               instance->interface = V8ObjectPersist::New (exportInterface);

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

   if (self && self->_core) {

      V8Value obj = Args[0];

      const Handle Object = self->_core->get_instance_handle (obj);

      if (Object) {

         SelfStruct *ss = self->_selfTable.lookup (Object);

         if (ss) {

            V8Value value = Args[1];

            if (v8_is_valid (value)) {

               if (value->IsFunction ()) {

                  V8Function func = v8_to_function (value);

                  SubscribeStruct *current (ss->subscribeList);

                  while (current) {

                     if (current->func == func) {

                        ss->remove (current); current = 0;
                        result = v8::Boolean::New (true);
                     }
                     else { current = current->next; }
                  }
               }
               else if (value->IsObject ()) {

                  V8Object instance = v8_to_object (value);

                  InstanceStruct *prev (0);
                  InstanceStruct *current (ss->instanceList);

                  while (current) {

                     if (current->interface == instance) {

                        HashTableHandleIterator it;
                        SubscribeStruct *sub (0);

                        while (current->is.subscribeTable.get_next (it, sub)) {

                           self->_do_callback (False, *current, *sub);
                        }

                        if (prev) { prev->next = current->next; }
                        else { ss->instanceList = current->next; }

                        delete current; current = 0;

                        result = v8::Boolean::New (true);
                     }
                     else { prev = current; current = current->next; }
                  }
               }
            }
            else if (v8_is_valid (obj) && obj->IsObject ()) {

               const String Name;
               V8Object handle = v8_to_object (obj);
               self->release_js_instance_v8 (Object, Name, handle);

               result = v8::Boolean::New (true);
            }
         }
      }
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

               InterfaceStruct *interface = self->_lookup_interface (Name);

               if (interface) {

                  SubscribeStruct *sub = new SubscribeStruct (Object, Scope, *interface);

                  if (sub) {

                     sub->self = V8ObjectPersist::New (obj);
                     sub->func = V8FunctionPersist::New (func);

                     if (self->_register_subscribe (*interface, *sub)) {

                        sub->next = ss->subscribeList;
                        ss->subscribeList = sub;
                        result = func;
                     }
                     else { delete sub; sub = 0; }
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
      InterfaceStruct &interface,
      SubscribeStruct &sub) {

   HashTableStringIterator it;
   InstanceStruct *instance (0);

   while (interface.interfaceTable.get_next (it, instance)) {

      _do_callback (Activate, *instance, sub);
   }
}


dmz::Boolean
dmz::JsExtV8Interface::_register_instance (
      InterfaceStruct &interface,
      InstanceStruct &instance) {

   Boolean result (False);

   if (interface.interfaceTable.store (instance.Scope, &instance)) {

      result = True;

      HashTableHandleIterator it;
      SubscribeStruct *sub (0);

      while (interface.subscribeTable.get_next (it, sub)) {

         _do_callback (True, instance, *sub);
      }
   }

   return result;
}


dmz::Boolean
dmz::JsExtV8Interface::_register_subscribe (
      InterfaceStruct &interface,
      SubscribeStruct &sub) {

   Boolean result (False);

   if (interface.subscribeTable.store (sub.Object, &sub)) {

      _subscribe_interface (True, interface, sub);

      result = True;
   }

   return result;
}


dmz::Boolean
dmz::JsExtV8Interface::_do_callback (
      const Boolean Activate,
      InstanceStruct &instance,
      SubscribeStruct &sub) {

   Boolean result (False);

   if ((!sub.Scope || (sub.Scope == instance.Scope)) &&
         (_v8Context.IsEmpty () == false)) {

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      if (v8_is_valid (sub.self) && v8_is_valid (sub.func)) {

         const int Argc (5);
         V8Value argv[Argc];
         argv[0] = Activate ? _activateStr : _deactivateStr;
         argv[1] = instance.interface;
         argv[2] = v8::String::New (instance.Scope.get_buffer ());
         argv[3] = v8::String::New (instance.Name ? instance.Name.get_buffer () : "");

         const Handle Object = sub.Object;
         V8Object self = v8::Local<v8::Object>::New (sub.self);
         V8Function func = v8::Local<v8::Function>::New (sub.func);

         argv[4] = self;

         v8::TryCatch tc;

         func->Call (self, Argc, argv);

         if (tc.HasCaught ()) {

            if (_core) { _core->handle_v8_exception (Object, tc); }

            SelfStruct *ss = _selfTable.lookup (Object);

            if (ss) { ss->remove (&sub); }
         }
         else { result = True; }
      }
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
