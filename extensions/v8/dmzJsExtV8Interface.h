#ifndef DMZ_JS_EXT_V8_FILE_DOT_H
#define DMZ_JS_EXT_V8_FILE_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesDeleteListTemplate.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>

namespace dmz {

   class JsExtV8Interface :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8Interface (const PluginInfo &Info, Config &local);
         ~JsExtV8Interface ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

      protected:
         // JsExtV8Interface Interface
         struct InterfaceStruct;

         struct InstanceStruct {

            const String Name;
            const String Scope;
            InstanceStruct *next;
            InterfaceStruct &is;
            V8ObjectPersist self;
            V8ObjectPersist interface;

            InstanceStruct (
                  const String &TheName,
                  const String &TheScope,
                  InterfaceStruct &theIs) :
                  Name (TheName),
                  Scope (TheScope),
                  next (0),
                  is (theIs) {;}

            ~InstanceStruct () {

               is.table.remove (Scope);
               self.Dispose (); self.Clear ();
               interface.Dispose (); interface.Clear ();
            }
         };

         struct SubscribeStruct {

            const Handle Object;
            const String Scope;
            SubscribeStruct *next;
            InterfaceStruct &is;
            V8ObjectPersist self;
            V8FunctionPersist func;

            SubscribeStruct (
                  const Handle TheObject,
                  const String &TheScope,
                  InterfaceStruct &theIs):
                  Object (TheObject),
                  Scope (TheScope),
                  next (0),
                  is (theIs) {;}

            ~SubscribeStruct () {

               is.subscribeTable.remove (Object);
               self.Dispose (); self.Clear ();
               func.Dispose (); func.Clear ();
            }
         };

         struct InterfaceStruct {

            const String Name;
            HashTableHandleTemplate<SubscribeStruct> subscribeTable;
            HashTableStringTemplate<InstanceStruct> table;

            InterfaceStruct (const String &TheName) : Name (TheName) {;}
         };

         struct SelfStruct {

            InstanceStruct *instanceList;
            SubscribeStruct *subscribeList;

            SelfStruct () : instanceList (0), subscribeList (0) {;}

            ~SelfStruct () { delete_list (instanceList); delete_list (subscribeList); }

            Boolean remove (SubscribeStruct *sub) {

               Boolean result (False);

               SubscribeStruct *prev (0);
               SubscribeStruct *current (subscribeList);

               while (current) {

                  if (current == sub) {

                     if (prev) { prev->next = current->next; }
                     else { subscribeList = current->next; }

                     delete current; current = 0;
                     result = True;
                  }
                  else { prev = current; current = current->next; }
               }

               return result;
            }
         };

         static JsExtV8Interface *_to_self (const v8::Arguments &Args);
         static V8Value _interface_publish (const v8::Arguments &Args);
         static V8Value _interface_remove (const v8::Arguments &Args);
         static V8Value _interface_subscribe (const v8::Arguments &Args);

         void _subscribe_interface (
            const Boolean Activate,
            InterfaceStruct &is,
            SubscribeStruct &ds);

         Boolean _register_instance (InterfaceStruct &is, InstanceStruct &ds);
         Boolean _register_subscribe (InterfaceStruct &is, SubscribeStruct &ds);
         InterfaceStruct *_lookup_interface (const String &Name);
         SelfStruct *_lookup_self_struct (const Handle Object);
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;

         V8InterfaceHelper _interfaceApi;

         v8::Handle<v8::Context> _v8Context;

         V8ValuePersist _self;

         V8StringPersist _activateStr;
         V8StringPersist _deactivateStr;

         HashTableStringTemplate<InterfaceStruct> _interfaceTable;
         HashTableHandleTemplate<SelfStruct> _selfTable;

      private:
         JsExtV8Interface ();
         JsExtV8Interface (const JsExtV8Interface &);
         JsExtV8Interface &operator= (const JsExtV8Interface &);

   };
};


inline dmz::JsExtV8Interface *
dmz::JsExtV8Interface::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Interface *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_FILE_DOT_H
