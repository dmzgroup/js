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

   class JsExtV8Module :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8Module (const PluginInfo &Info, Config &local);
         ~JsExtV8Module ();

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
         // JsExtV8Module Interface
         struct ModuleStruct;

         struct InstanceStruct {

            const String Name;
            const String Scope;
            InstanceStruct *next;
            ModuleStruct &is;
            V8ObjectPersist self;
            V8ObjectPersist module;

            InstanceStruct (
                  const String &TheName,
                  const String &TheScope,
                  ModuleStruct &theIs) :
                  Name (TheName),
                  Scope (TheScope),
                  next (0),
                  is (theIs) {;}

            ~InstanceStruct () {

               is.moduleTable.remove (Scope);
               self.Dispose (); self.Clear ();
               module.Dispose (); module.Clear ();
            }
         };

         struct SubscribeStruct {

            const Handle Object;
            const String Scope;
            SubscribeStruct *next;
            ModuleStruct &is;
            V8ObjectPersist self;
            V8FunctionPersist func;

            SubscribeStruct (
                  const Handle TheObject,
                  const String &TheScope,
                  ModuleStruct &theIs):
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

         struct ModuleStruct {

            const String Name;
            HashTableHandleTemplate<SubscribeStruct> subscribeTable;
            HashTableStringTemplate<InstanceStruct> moduleTable;

            ModuleStruct (const String &TheName) : Name (TheName) {;}
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

         static JsExtV8Module *_to_self (const v8::Arguments &Args);
         static V8Value _module_publish (const v8::Arguments &Args);
         static V8Value _module_remove (const v8::Arguments &Args);
         static V8Value _module_subscribe (const v8::Arguments &Args);

         void _subscribe_module (
            const Boolean Activate,
            ModuleStruct &module,
            SubscribeStruct &sub);

         Boolean _register_instance (
            ModuleStruct &module,
            InstanceStruct &instance);

         Boolean _register_subscribe (ModuleStruct &module, SubscribeStruct &sub);

         Boolean _do_callback (
            const Boolean Activate,
            InstanceStruct &instance,
            SubscribeStruct &sub);

         ModuleStruct *_lookup_module (const String &Name);
         SelfStruct *_lookup_self_struct (const Handle Object);
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;

         V8InterfaceHelper _moduleApi;

         v8::Handle<v8::Context> _v8Context;

         V8ValuePersist _self;

         V8StringPersist _activateStr;
         V8StringPersist _deactivateStr;

         HashTableStringTemplate<ModuleStruct> _moduleTable;
         HashTableHandleTemplate<SelfStruct> _selfTable;

      private:
         JsExtV8Module ();
         JsExtV8Module (const JsExtV8Module &);
         JsExtV8Module &operator= (const JsExtV8Module &);

   };
};


inline dmz::JsExtV8Module *
dmz::JsExtV8Module::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Module *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_FILE_DOT_H
