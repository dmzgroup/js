#ifndef DMZ_JS_EXT_V8_EVENT_DOT_H
#define DMZ_JS_EXT_V8_EVENT_DOT_H

#include <dmzEventObserverUtil.h>
#include <dmzJsExtV8.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeEventType.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHandleContainer.h>
#include <dmzTypesHashTableHandleTemplate.h>

#include <v8.h>

namespace dmz {

   class EventModule;
   class JsModuleV8;
   class JsModuleRuntimeV8;
   class JsModuleTypesV8;

   class JsExtV8Event :
         public Plugin,
         public EventObserverUtil,
         public JsExtV8 {

      public:
         JsExtV8Event (const PluginInfo &Info, Config &local);
         ~JsExtV8Event ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // Event Observer Interface
         virtual void create_event (
            const Handle EventHandle,
            const EventType &Type,
            const EventLocalityEnum Locality);

         virtual void close_event (
            const Handle EventHandle,
            const EventType &Type,
            const EventLocalityEnum Locality);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

      protected:
         struct CallbackStruct;

         struct CallbackTable {

            const EventType Type;
            HashTableHandleTemplate<CallbackStruct> table;

            CallbackTable (const EventType &TheType) : Type (TheType) {;}
            ~CallbackTable () { table.empty (); }
         };

         struct CallbackStruct {

            const Handle Observer;
            V8FunctionPersist func;
            V8ObjectPersist self;

            CallbackStruct (const Handle TheObserver) : Observer (TheObserver) {;}

            ~CallbackStruct () {

               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };

         struct ObsStruct {

            HashTableHandleTemplate<CallbackTable> create;
            HashTableHandleTemplate<CallbackTable> close;

            ~ObsStruct () { create.clear (); close.clear (); }
         };

         static JsExtV8Event *_to_self (const v8::Arguments &Args);

         static Boolean _to_event (
            const v8::Arguments &Args,
            JsExtV8Event *&self,
            Handle &event,
            Handle &attr);

         static V8Function _register_observer_static (
            const v8::Arguments &Args,
            const Mask &AttrMask);

         // Event API Bindings
         static V8Value _event_release (const v8::Arguments &Args);
         static V8Value _event_create (const v8::Arguments &Args);
         static V8Value _event_create_observe (const v8::Arguments &Args);
         static V8Value _event_close (const v8::Arguments &Args);
         static V8Value _event_close_observe (const v8::Arguments &Args);
         static V8Value _event_type (const v8::Arguments &Args);
         static V8Value _event_locality (const v8::Arguments &Args);
         static V8Value _event_handle (const v8::Arguments &Args);
         static V8Value _event_object_handle (const v8::Arguments &Args);
         static V8Value _event_object_type (const v8::Arguments &Args);
         static V8Value _event_state (const v8::Arguments &Args);
         static V8Value _event_time_stamp (const v8::Arguments &Args);
         static V8Value _event_position (const v8::Arguments &Args);
         static V8Value _event_orientation (const v8::Arguments &Args);
         static V8Value _event_velocity (const v8::Arguments &Args);
         static V8Value _event_acceleration (const v8::Arguments &Args);
         static V8Value _event_scale (const v8::Arguments &Args);
         static V8Value _event_vector (const v8::Arguments &Args);
         static V8Value _event_scalar (const v8::Arguments &Args);
         static V8Value _event_counter (const v8::Arguments &Args);
         static V8Value _event_text (const v8::Arguments &Args);
         static V8Value _event_data (const v8::Arguments &Args);

         // Event Observer Util Interface
         virtual void _store_event_module (EventModule &module);
         virtual void _remove_event_module (EventModule &module);

         V8Function _register_observer (const v8::Arguments &Args, const Mask &AttrMask);

         void _do_callback (
            const Handle EventHandle,
            const EventLocalityEnum Locality,
            CallbackTable *table);

         void _release_callback (
            const Mask &CBMask,
            const Handle Obs,
            HashTableHandleTemplate<CallbackTable> &table,
            V8Function func);

         void _init (Config &local);

         Log _log;
         Definitions _defs;

         V8InterfaceHelper _eventApi;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         EventModule *_event;
         JsModuleV8 *_core;
         JsModuleRuntimeV8 *_runtime;
         JsModuleTypesV8 *_types;

         Handle _defaultAttr;

         HashTableHandleTemplate<CallbackTable> _createTable;
         HashTableHandleTemplate<CallbackTable> _closeTable;
         HashTableHandleTemplate<ObsStruct> _obsTable;

      private:
         JsExtV8Event ();
         JsExtV8Event (const JsExtV8Event &);
         JsExtV8Event &operator= (const JsExtV8Event &);

   };
};


inline dmz::JsExtV8Event *
dmz::JsExtV8Event::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Event *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_EVENT_DOT_H
