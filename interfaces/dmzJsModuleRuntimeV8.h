#ifndef DMZ_JS_MODULE_RUNTIME_V8_DOT_H
#define DMZ_JS_MODULE_RUNTIME_V8_DOT_H

#include <dmzRuntimePlugin.h>
#include <dmzRuntimeRTTI.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

#include <v8.h>

namespace dmz {

   class Config;
   class Data;
   class EventType;
   class ObjectType;

   class JsModuleRuntimeV8 {

      public:
         static JsModuleRuntimeV8 *cast (
            const Plugin *PluginPtr,
            const String &PluginName = "");

         String get_js_module_runtime_v8_name () const;
         Handle get_js_module_runtime_v8_handle () const;

         // JsModuleRuntimeV8 Interface
         virtual v8::Handle<v8::Value> create_v8_config (const Config *Value) = 0;
         virtual v8::Handle<v8::Value> create_v8_data (const Data *Value) = 0;
         virtual Boolean to_dmz_data (v8::Handle<v8::Value> value, Data &out) = 0;
         virtual v8::Handle<v8::Value> create_v8_event_type (const EventType *Value) = 0;
         virtual Boolean to_dmz_event_type (
            v8::Handle<v8::Value> value,
            EventType &out) = 0;
         virtual v8::Handle<v8::Value> create_v8_log (const String &Name) = 0;
         virtual v8::Handle<v8::Value> create_v8_message (const String &Name) = 0;
         virtual v8::Handle<v8::Value> create_v8_object_type (
            const ObjectType *Value) = 0;
         virtual Boolean to_dmz_object_type (
            v8::Handle<v8::Value> value,
            ObjectType &out) = 0;

      protected:
         JsModuleRuntimeV8 (const PluginInfo &Info);
         ~JsModuleRuntimeV8 ();

      private:
         JsModuleRuntimeV8 ();
         JsModuleRuntimeV8 (const JsModuleRuntimeV8 &);
         JsModuleRuntimeV8 &operator= (const JsModuleRuntimeV8 &);

         const PluginInfo &__Info;
   };

   //! \cond
   const char JsModuleRuntimeV8InterfaceName[] = "JsModuleRuntimeV8Interface";
   //! \endcond
};


inline dmz::JsModuleRuntimeV8 *
dmz::JsModuleRuntimeV8::cast (const Plugin *PluginPtr, const String &PluginName) {

   return (JsModuleRuntimeV8 *)lookup_rtti_interface (
      JsModuleRuntimeV8InterfaceName,
      PluginName,
      PluginPtr);
}


inline
dmz::JsModuleRuntimeV8::JsModuleRuntimeV8 (const PluginInfo &Info) :
      __Info (Info) {

   store_rtti_interface (JsModuleRuntimeV8InterfaceName, __Info, (void *)this);
}


inline
dmz::JsModuleRuntimeV8::~JsModuleRuntimeV8 () {

   remove_rtti_interface (JsModuleRuntimeV8InterfaceName, __Info);
}


inline dmz::String
dmz::JsModuleRuntimeV8::get_js_module_runtime_v8_name () const {

   return __Info.get_name ();
}


inline dmz::Handle
dmz::JsModuleRuntimeV8::get_js_module_runtime_v8_handle () const {

   return __Info.get_handle ();
}

#endif // DMZ_JS_MODULE_RUNTIME_V8_DOT_H
