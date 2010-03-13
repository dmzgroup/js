#ifndef DMZ_JS_EXT_V8_OBJECT_DOT_H
#define DMZ_JS_EXT_V8_OBJECT_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzObjectObserverUtil.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeTimeSlice.h>
#include <dmzTypesHandleContainer.h>
#include <dmzTypesDeleteListTemplate.h>
#include <dmzTypesHashTableHandleTemplate.h>

#include <v8.h>

namespace dmz {

   class JsModuleRuntimeV8;
   class JsModuleTypesV8;
   class JsModuleV8;

   class JsExtV8Object :
         public Plugin,
         public JsExtV8,
         public TimeSlice,
         public ObjectObserverUtil {

      public:
         static JsExtV8Object *to_self (const v8::Arguments &Args);

         JsExtV8Object (const PluginInfo &Info, Config &local);
         ~JsExtV8Object ();

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

         // TimeSlice Interface
         virtual void update_time_slice (const Float64 TimeDelta);

         // Object Observer Interface
         virtual void create_object (
            const UUID &Identity,
            const Handle ObjectHandle,
            const ObjectType &Type,
            const ObjectLocalityEnum Locality);

         virtual void destroy_object (const UUID &Identity, const Handle ObjectHandle);

         virtual void update_object_uuid (
            const Handle ObjectHandle,
            const UUID &Identity,
            const UUID &PrevIdentity);

         virtual void remove_object_attribute (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Mask &AttrMask);

         virtual void update_object_locality (
            const UUID &Identity,
            const Handle ObjectHandle,
            const ObjectLocalityEnum Locality,
            const ObjectLocalityEnum PrevLocality);

         virtual void link_objects (
            const Handle LinkHandle,
            const Handle AttributeHandle,
            const UUID &SuperIdentity,
            const Handle SuperHandle,
            const UUID &SubIdentity,
            const Handle SubHandle);

         virtual void unlink_objects (
            const Handle LinkHandle,
            const Handle AttributeHandle,
            const UUID &SuperIdentity,
            const Handle SuperHandle,
            const UUID &SubIdentity,
            const Handle SubHandle);

         virtual void update_link_attribute_object (
            const Handle LinkHandle,
            const Handle AttributeHandle,
            const UUID &SuperIdentity,
            const Handle SuperHandle,
            const UUID &SubIdentity,
            const Handle SubHandle,
            const UUID &AttributeIdentity,
            const Handle AttributeObjectHandle,
            const UUID &PrevAttributeIdentity,
            const Handle PrevAttributeObjectHandle);

         virtual void update_object_counter (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Int64 Value,
            const Int64 *PreviousValue);

         virtual void update_object_counter_minimum (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Int64 Value,
            const Int64 *PreviousValue);

         virtual void update_object_counter_maximum (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Int64 Value,
            const Int64 *PreviousValue);

         virtual void update_object_alternate_type (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const ObjectType &Value,
            const ObjectType *PreviousValue);

         virtual void update_object_state (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Mask &Value,
            const Mask *PreviousValue);

         virtual void update_object_flag (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Boolean Value,
            const Boolean *PreviousValue);

         virtual void update_object_time_stamp (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Float64 Value,
            const Float64 *PreviousValue);

         virtual void update_object_position (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Vector &Value,
            const Vector *PreviousValue);

         virtual void update_object_orientation (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Matrix &Value,
            const Matrix *PreviousValue);

         virtual void update_object_velocity (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Vector &Value,
            const Vector *PreviousValue);

         virtual void update_object_acceleration (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Vector &Value,
            const Vector *PreviousValue);

         virtual void update_object_scale (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Vector &Value,
            const Vector *PreviousValue);

         virtual void update_object_vector (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Vector &Value,
            const Vector *PreviousValue);

         virtual void update_object_scalar (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Float64 Value,
            const Float64 *PreviousValue);

         virtual void update_object_text (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const String &Value,
            const String *PreviousValue);

         virtual void update_object_data (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Data &Value,
            const Data *PreviousValue);

      protected:
         struct CallbackStruct;

         struct CallbackTable {

            const Handle Attr;
            const Mask AttrMask;

            HashTableHandleTemplate<CallbackStruct> table;

            CallbackTable (const Handle TheAttr, const Mask &TheMask) :
                  Attr (TheAttr),
                  AttrMask (TheMask) {;}
         };

         struct CallbackStruct {

            const Handle ObsHandle;
            CallbackTable *table;
            CallbackStruct *next;
            V8FunctionPersist func;
            V8ObjectPersist self;

            CallbackStruct (
                  const Handle TheObsHandle,
                  CallbackTable *theTable) :
                  ObsHandle (TheObsHandle),
                  table (theTable),
                  next (0) {

               if (table && !table->table.store (ObsHandle, this)) { table = 0; }
            }

            ~CallbackStruct () {

               if (table) { table->table.remove (ObsHandle); }
               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };

         struct ObsStruct {

            CallbackStruct *list;
            ObsStruct () : list (0) {}
            ~ObsStruct () { delete_list (list); }
            Boolean delete_callback (CallbackStruct *cb) {

               Boolean result (False);

               if (cb) {

                  CallbackStruct *prev (0);
                  CallbackStruct *current (list);
                  CallbackStruct *found (0);

                  while (current && !found) {

                     if ((current->func == cb->func) &&
                           (current->self == cb->self)) {

                        found = current;
                     }
                     else { prev = current; current = current->next; }
                  }

                  if (found) {

                     result = true;

                     if (prev) { prev->next = found->next; }
                     else { list = found->next; }

                     delete found; found = 0;
                  }
               }

               return result;
            }
         };

         static Handle _to_attr (JsExtV8Object *self, V8Value value);
         static Handle _to_object (JsExtV8Object *self, V8Value value);

         static Boolean _get_params (
            const v8::Arguments &Args,
            ObjectModule *&objMod,
            JsModuleTypesV8 *&types,
            Handle &obj,
            Handle &attr);

         static V8Function _register_observer_static (
            const v8::Arguments &Args,
            const Mask &AttrMask);

         static V8Value _object_is_object (const v8::Arguments &Args);
         static V8Value _object_is_activated (const v8::Arguments &Args);
         static V8Value _object_is_link (const v8::Arguments &Args);
         static V8Value _object_get_objects (const v8::Arguments &Args);
         static V8Value _object_create (const v8::Arguments &Args);
         static V8Value _object_create_observe (const v8::Arguments &Args);
         static V8Value _object_activate (const v8::Arguments &Args);
         static V8Value _object_destroy (const v8::Arguments &Args);
         static V8Value _object_destroy_observe (const v8::Arguments &Args);
         static V8Value _object_clone (const v8::Arguments &Args);
         static V8Value _object_make_persistent (const v8::Arguments &Args);
         static V8Value _object_lookup_type (const v8::Arguments &Args);
         static V8Value _object_locality (const v8::Arguments &Args);
         static V8Value _object_locality_observe (const v8::Arguments &Args);
         static V8Value _object_uuid (const v8::Arguments &Args);
         static V8Value _object_uuid_observe (const v8::Arguments &Args);
         static V8Value _object_link (const v8::Arguments &Args);
         static V8Value _object_link_observe (const v8::Arguments &Args);
         static V8Value _object_lookup_link_handle (const v8::Arguments &Args);
         static V8Value _object_lookup_linked_objects (const v8::Arguments &Args);
         static V8Value _object_unlink (const v8::Arguments &Args);
         static V8Value _object_unlink_observe (const v8::Arguments &Args);
         static V8Value _object_unlink_super_objects (const v8::Arguments &Args);
         static V8Value _object_unlink_sub_objects (const v8::Arguments &Args);
         static V8Value _object_lookup_super_links (const v8::Arguments &Args);
         static V8Value _object_lookup_sub_links (const v8::Arguments &Args);
         static V8Value _object_link_attribute_object (const v8::Arguments &Args);
         static V8Value _object_link_attribute_object_observe (const v8::Arguments &Args);
         static V8Value _object_lookup_attribute_object_links (const v8::Arguments &Args);
         static V8Value _object_counter (const v8::Arguments &Args);
         static V8Value _object_counter_observe (const v8::Arguments &Args);
         static V8Value _object_counter_min (const v8::Arguments &Args);
         static V8Value _object_counter_min_observe (const v8::Arguments &Args);
         static V8Value _object_counter_max (const v8::Arguments &Args);
         static V8Value _object_counter_max_observe (const v8::Arguments &Args);
         static V8Value _object_counter_rollover (const v8::Arguments &Args);
         static V8Value _object_add_to_counter (const v8::Arguments &Args);
         static V8Value _object_alt_type (const v8::Arguments &Args);
         static V8Value _object_alt_type_observe (const v8::Arguments &Args);
         static V8Value _object_state (const v8::Arguments &Args);
         static V8Value _object_state_observe (const v8::Arguments &Args);
         static V8Value _object_flag (const v8::Arguments &Args);
         static V8Value _object_flag_observe (const v8::Arguments &Args);
         static V8Value _object_time_stamp (const v8::Arguments &Args);
         static V8Value _object_time_stamp_observe (const v8::Arguments &Args);
         static V8Value _object_position (const v8::Arguments &Args);
         static V8Value _object_position_observe (const v8::Arguments &Args);
         static V8Value _object_orientation (const v8::Arguments &Args);
         static V8Value _object_orientation_observe (const v8::Arguments &Args);
         static V8Value _object_velocity (const v8::Arguments &Args);
         static V8Value _object_velocity_observe (const v8::Arguments &Args);
         static V8Value _object_acceleration (const v8::Arguments &Args);
         static V8Value _object_acceleration_observe (const v8::Arguments &Args);
         static V8Value _object_scale (const v8::Arguments &Args);
         static V8Value _object_scale_observe (const v8::Arguments &Args);
         static V8Value _object_vector (const v8::Arguments &Args);
         static V8Value _object_vector_observe (const v8::Arguments &Args);
         static V8Value _object_scalar (const v8::Arguments &Args);
         static V8Value _object_scalar_observe (const v8::Arguments &Args);
         static V8Value _object_text (const v8::Arguments &Args);
         static V8Value _object_text_observe (const v8::Arguments &Args);
         static V8Value _object_data (const v8::Arguments &Args);
         static V8Value _object_data_observe (const v8::Arguments &Args);

         // JsExtV8Object Interface
         Handle _to_handle (V8Value value);
         Handle _to_object_handle (V8Value value);
         V8Function _register_observer (
            const v8::Arguments &Args,
            const Mask &AttrMask);

         CallbackStruct *_create_callback (
            const Handle ObsHandle,
            const Handle Attr,
            const Mask &AttrMask,
            HashTableHandleTemplate<CallbackTable> &table,
            Boolean &doDump);

         void _process_callback (
            CallbackTable *table,
            int argc,
            v8::Handle<v8::Value> argv[]);

         void _do_callback (CallbackStruct *cb, int argc, v8::Handle<v8::Value> argv[]);
         void _init (Config &local);

         Log _log;
         Definitions _defs;

         HandleContainer _localObjects;

         V8InterfaceHelper _objectApi;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         Handle _defaultAttr;

         JsModuleRuntimeV8 *_runtime;
         JsModuleTypesV8 *_types;
         JsModuleV8 *_core;

         CallbackStruct *_newCallback;

         HashTableHandleTemplate<ObsStruct> _obsTable;

         CallbackTable _createTable;
         CallbackTable _destroyTable;
         CallbackTable _uuidTable;
         CallbackTable _localityTable;
         HashTableHandleTemplate<CallbackTable> _linkTable;
         HashTableHandleTemplate<CallbackTable> _unlinkTable;
         HashTableHandleTemplate<CallbackTable> _linkAttrTable;
         HashTableHandleTemplate<CallbackTable> _counterTable;
         HashTableHandleTemplate<CallbackTable> _minCounterTable;
         HashTableHandleTemplate<CallbackTable> _maxCounterTable;
         HashTableHandleTemplate<CallbackTable> _altTypeTable;
         HashTableHandleTemplate<CallbackTable> _stateTable;
         HashTableHandleTemplate<CallbackTable> _flagTable;
         HashTableHandleTemplate<CallbackTable> _timeStampTable;
         HashTableHandleTemplate<CallbackTable> _positionTable;
         HashTableHandleTemplate<CallbackTable> _orientationTable;
         HashTableHandleTemplate<CallbackTable> _velocityTable;
         HashTableHandleTemplate<CallbackTable> _accelerationTable;
         HashTableHandleTemplate<CallbackTable> _scaleTable;
         HashTableHandleTemplate<CallbackTable> _vectorTable;
         HashTableHandleTemplate<CallbackTable> _scalarTable;
         HashTableHandleTemplate<CallbackTable> _textTable;
         HashTableHandleTemplate<CallbackTable> _dataTable;

      private:
         JsExtV8Object ();
         JsExtV8Object (const JsExtV8Object &);
         JsExtV8Object &operator= (const JsExtV8Object &);

   };
};


inline dmz::JsExtV8Object *
dmz::JsExtV8Object::to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Object *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_OBJECT_DOT_H
