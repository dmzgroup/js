#ifndef DMZ_JS_EXT_V8_INPUT_DOT_H
#define DMZ_JS_EXT_V8_INPUT_DOT_H

#include <dmzInputObserverUtil.h>
#include <dmzJsExtV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesDeleteListTemplate.h>
#include <dmzTypesHandleContainer.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesMask.h>

#include <v8.h>

namespace dmz {

   class JsModuleRuntimeV8;
   class dmzJsModuleV8;

   class JsExtV8Input :
         public Plugin,
         public JsExtV8,
         public InputObserverUtil {

      public:
         JsExtV8Input (const PluginInfo &Info, Config &local);
         ~JsExtV8Input ();

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

         // Input Observer Interface
         virtual void update_channel_state (const Handle Channel, const Boolean State);

         virtual void receive_axis_event (
            const Handle Channel,
            const InputEventAxis &Value);

         virtual void receive_button_event (
            const Handle Channel,
            const InputEventButton &Value);

         virtual void receive_switch_event (
            const Handle Channel,
            const InputEventSwitch &Value);

         virtual void receive_key_event (
            const Handle Channel,
            const InputEventKey &Value);

         virtual void receive_mouse_event (
            const Handle Channel,
            const InputEventMouse &Value);

         virtual void receive_data_event (
            const Handle Channel,
            const Handle Source,
            const Data &Value);

      protected:
         struct CallbackTable;

         struct CallbackStruct {

            CallbackStruct *next;
            CallbackTable &table;
            V8FunctionPersist func;
            V8ObjectPersist self;

            CallbackStruct (CallbackTable &theTable) : next (0), table (theTable) {;}

            ~CallbackStruct () {

               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };

         struct CallbackTable {

            const Handle Channel;
            const Mask Type;

            HashTableHandleTemplate<CallbackStruct> table;

            CallbackTable (
                  const Handle TheChannel,
                  const Mask &TheType) : Channel (TheChannel), Type (TheType) {;}

            ~CallbackTable () { table.empty (); }
         };

         struct ObsStruct {

            CallbackStruct *list;

            ObsStruct () : list (0) {;}

            ~ObsStruct () { delete_list (list); }
         };

         static JsExtV8Input *_to_self (const v8::Arguments &Args);

         static V8Value _register_callback_static (
            const v8::Arguments &Args,
            const Mask &Type);

         // API Bindings
         static V8Value _input_release (const v8::Arguments &Args);
         static V8Value _input_channel (const v8::Arguments &Args);
         static V8Value _input_channel_create (const v8::Arguments &Args);
         static V8Value _input_channel_observe (const v8::Arguments &Args);
         static V8Value _input_axis (const v8::Arguments &Args);
         static V8Value _input_axis_observe (const v8::Arguments &Args);
         static V8Value _input_button (const v8::Arguments &Args);
         static V8Value _input_button_observe (const v8::Arguments &Args);
         static V8Value _input_lever (const v8::Arguments &Args);
         static V8Value _input_lever_observe (const v8::Arguments &Args);
         static V8Value _input_key (const v8::Arguments &Args);
         static V8Value _input_key_to_value (const v8::Arguments &Args);
         static V8Value _input_key_to_string (const v8::Arguments &Args);
         static V8Value _input_key_observe (const v8::Arguments &Args);
         static V8Value _input_mouse (const v8::Arguments &Args);
         static V8Value _input_mouse_observe (const v8::Arguments &Args);
         static V8Value _input_data (const v8::Arguments &Args);
         static V8Value _input_data_observe (const v8::Arguments &Args);

         V8Value _register_callback (const v8::Arguments &Args, const Mask &Type);
         Boolean _release_callback (V8Object src, V8Function func);
         void _delete_callback (const Handle Obs, CallbackTable &ct);
         Handle _to_handle (V8Value value);

         void _do_callback (
            const int Argc,
            V8Value argv[],
            CallbackTable &ct,
            HandleContainer &called);

         void _do_all_callbacks (
            const int Argc,
            V8Value argv[],
            HashTableHandleTemplate<CallbackTable> &table);

         void _init (Config &local);

         Log _log;
         Definitions _defs;

         Handle _defaultChannel;

         HandleContainer _active;

         HashTableHandleTemplate<ObsStruct> _obsTable;

         HashTableHandleTemplate<CallbackTable> _stateTable;
         HashTableHandleTemplate<CallbackTable> _axisTable;
         HashTableHandleTemplate<CallbackTable> _buttonTable;
         HashTableHandleTemplate<CallbackTable> _leverTable;
         HashTableHandleTemplate<CallbackTable> _keyTable;
         HashTableHandleTemplate<CallbackTable> _mouseTable;
         HashTableHandleTemplate<CallbackTable> _dataTable;

         V8InterfaceHelper _inputApi;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         V8StringPersist _sourceStr;
         V8StringPersist _idStr;
         V8StringPersist _valueStr;
         V8StringPersist _previousStr;
         V8StringPersist _deltaStr;
         V8StringPersist _keyStr;
         V8StringPersist _stateStr;

         JsModuleV8 *_core;
         JsModuleRuntimeV8 *_runtime;

      private:
         JsExtV8Input ();
         JsExtV8Input (const JsExtV8Input &);
         JsExtV8Input &operator= (const JsExtV8Input &);

   };
};


inline dmz::JsExtV8Input *
dmz::JsExtV8Input::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Input *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_INPUT_DOT_H
