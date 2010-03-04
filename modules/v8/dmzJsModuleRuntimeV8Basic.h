#ifndef DMZ_JS_MODULE_RUNTIME_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_RUNTIME_V8_BASIC_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsModuleRuntimeV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeTime.h>
#include <dmzTypesHashTableStringTemplate.h>

#include <v8.h>

namespace dmz {

   class JsModuleTypesV8;
   class Data;

   class JsModuleRuntimeV8Basic :
         public Plugin,
         public JsModuleRuntimeV8,
         public JsExtV8 {

      public:
         static JsModuleRuntimeV8Basic *to_self (const v8::Arguments &Args);
         JsModuleRuntimeV8Basic (const PluginInfo &Info, Config &local);
         ~JsModuleRuntimeV8Basic ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsModuleRuntimeV8 Interface
         virtual v8::Handle<v8::Value> create_v8_log (const String &Name);
         virtual v8::Handle<v8::Value> create_v8_data (const Data *Value);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         // JsModuleRuntimeV8Basic Interface
         void handle_v8_exception (v8::TryCatch &tc);
         // implemented in dmzJsModuleRuntimeV8BasicTimer.cpp
         Boolean delete_timer (V8Object self, V8Function callback);

      protected:
         struct TimerStruct;
         // Static Functions
         // Data bindings implemented in dmzJsModuleRuntimeV8BasicData.cpp
         static V8Value _create_data (const v8::Arguments &Args);
         static V8Value _data_is_type_of (const v8::Arguments &Args);
         static V8Value _data_to_string (const v8::Arguments &Args);
         static V8Value _data_number (const v8::Arguments &Args);

         // Definitions bindings implemented in dmzJsModuleRuntimeV8BasicDefinitions.cpp
         static V8Value _create_named_handle (const v8::Arguments &Args);
         static V8Value _lookup_named_handle (const v8::Arguments &Args);
         static V8Value _lookup_named_handle_name (const v8::Arguments &Args);
         static V8Value _lookup_state (const v8::Arguments &Args);
         static V8Value _lookup_state_name (const v8::Arguments &Args);

         // Timer bindings implemented in dmzJsModuleRuntimeV8BasicTimer.cpp
         static V8Value _set_timer (const v8::Arguments &Args);
         static V8Value _set_repeating_timer (const v8::Arguments &Args);
         static V8Value _set_base_timer (
            const v8::Arguments &Args,
            const Boolean Repeating);
         static V8Value _cancle_timer (const v8::Arguments &Args);
         static V8Value _get_frame_delta (const v8::Arguments &Args);
         static V8Value _get_frame_time (const v8::Arguments &Args);
         static V8Value _get_system_time (const v8::Arguments &Args);

         // JsModuleRuntimeV8Basic Interface
         void _clear ();
         void _reset ();
         Handle _to_handle (V8Value value);
         // implemented in dmzJsModuleRuntimeV8BasicData.cpp
         void _init_data ();
         V8Object _to_data (V8Value value);
         Data *_to_data_ptr (V8Value value);
         // implemented in dmzJsModuleRuntimeV8BasicDefinitions.cpp
         void _init_definitions ();
         // implemented in dmzJsModuleRuntimeV8BasicLog.cpp
         void _init_log ();
         // implemented in dmzJsModuleRuntimeV8BasicTimer.cpp
         void _init_time ();
         // implemented in dmzJsModuleRuntimeV8Basic.cpp
         void _init (Config &local);

         Log _log;
         Time _time;
         Definitions _defs;

         JsModuleV8 *_core;
         JsModuleTypesV8 *_types;

         v8::Handle<v8::Context> _v8Context;

         V8ValuePersist _self;

         V8InterfaceHelper _dataApi; 
         V8InterfaceHelper _defsApi; 
         V8InterfaceHelper _timeApi; 

         HashTableStringTemplate<TimerStruct> _timerTable;

         v8::Persistent<v8::FunctionTemplate> _logFuncTemplate;
         v8::Persistent<v8::Function> _logFunc;

         v8::Persistent<v8::FunctionTemplate> _dataFuncTemplate;
         v8::Persistent<v8::Function> _dataFunc;

      private:
         JsModuleRuntimeV8Basic ();
         JsModuleRuntimeV8Basic (const JsModuleRuntimeV8Basic &);
         JsModuleRuntimeV8Basic &operator= (const JsModuleRuntimeV8Basic &);

   };
};

inline dmz::JsModuleRuntimeV8Basic *
dmz::JsModuleRuntimeV8Basic::to_self (const v8::Arguments &Args) {

   return (dmz::JsModuleRuntimeV8Basic *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_MODULE_RUNTIME_V8_BASIC_DOT_H
