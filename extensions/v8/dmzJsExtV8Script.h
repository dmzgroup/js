#ifndef DMZ_JS_EXT_V8_SCRIPT_DOT_H
#define DMZ_JS_EXT_V8_SCRIPT_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsModule.h>
#include <dmzJsModuleV8.h>
#include <dmzJsObserver.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHandleContainer.h>
#include <dmzTypesHashTableHandleTemplate.h>

namespace dmz {

   class JsExtV8Script :
         public Plugin,
         public JsExtV8,
         public JsObserver {

      public:
         JsExtV8Script (const PluginInfo &Info, Config &local);
         ~JsExtV8Script ();

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

         // JsObserver Interface
         virtual void update_js_script (
            const JsObserverModeEnum Mode,
            const Handle Module,
            const Handle Script);

         virtual void update_js_instance (
            const JsObserverModeEnum Mode,
            const Handle Module,
            const Handle Instance);

         virtual void update_js_error (
            const Handle Module,
            const Handle Script,
            const Handle Instance,
            const String &ErrorMessage,
            const String &StackTrace);

      protected:
         // JsExtV8Script Interface
         struct ErrorStruct {

            Handle script;
            Handle instance;
            String error;
            String stack;

            ErrorStruct () : script (0), instance (0) {;}

            void clear () {

               script = 0;
               instance = 0;
               error.flush ();
               stack.flush ();
            }
         };

         struct CallbackStruct {

            V8FunctionPersist func;
            V8ObjectPersist self;

            void clear () {

               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }

            CallbackStruct () {;}

            ~CallbackStruct () { clear (); }
         };

         static JsExtV8Script *_to_self (const v8::Arguments &Args);
         // API Bindings
         static V8Value _script_observe (const v8::Arguments &Args);
         static V8Value _script_release (const v8::Arguments &Args);
         static V8Value _script_error (const v8::Arguments &Args);
         static V8Value _script_load (const v8::Arguments &Args);
         static V8Value _script_reload (const v8::Arguments &Args);
         static V8Value _script_compile (const v8::Arguments &Args);
         static V8Value _script_instance (const v8::Arguments &Args);
         static V8Value _script_destroy (const v8::Arguments &Args);

         void _init (Config &local);

         Log _log;

         HandleContainer _scripts;

         V8InterfaceHelper _scriptApi;

         HashTableHandleTemplate<CallbackStruct> _releaseTable;

         v8::Handle<v8::Context> _v8Context;

         V8ValuePersist _self;

         V8StringPersist _scriptStr;
         V8StringPersist _instanceStr;
         V8StringPersist _errorStr;
         V8StringPersist _stackStr;

         JsModule *_js;
         JsModuleV8 *_core;

         ErrorStruct _lastError;

      private:
         JsExtV8Script ();
         JsExtV8Script (const JsExtV8Script &);
         JsExtV8Script &operator= (const JsExtV8Script &);

   };
};


inline dmz::JsExtV8Script *
dmz::JsExtV8Script::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Script *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_SCRIPT_DOT_H
