#ifndef DMZ_JS_MODULE_TYPES_V8_DOT_H
#define DMZ_JS_MODULE_TYPES_V8_DOT_H

#include <dmzRuntimePlugin.h>
#include <dmzRuntimeRTTI.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>
#include <dmzTypesMask.h>
#include <dmzTypesMatrix.h>
#include <dmzTypesVector.h>

#include <v8.h>

namespace dmz {

   class JsModuleTypesV8 {

      public:
         static JsModuleTypesV8 *cast (
            const Plugin *PluginPtr,
            const String &PluginName = "");

         String get_js_module_types_v8_name () const;
         Handle get_js_module_types_v8_handle () const;

         // JsModuleTypesV8 Interface
         virtual v8::Handle<v8::Object> to_v8_vector (const Vector &Value) = 0;
         virtual Vector to_dmz_vector (const v8::Handle<v8::Value> Value) = 0;

         virtual Boolean to_dmz_vector (
            const v8::Handle<v8::Value> Value,
            Vector &out) = 0;

         virtual v8::Handle<v8::Object> to_v8_matrix (const Matrix &Value) = 0;
         virtual Matrix to_dmz_matrix (const v8::Handle<v8::Value> Value) = 0;

         virtual Boolean to_dmz_matrix (
            const v8::Handle<v8::Value> Value,
            Matrix &out) = 0;

         virtual v8::Handle<v8::Object> to_v8_mask (const Mask &Value) = 0;
         virtual Mask to_dmz_mask (const v8::Handle<v8::Value> Value) = 0;

         virtual Boolean to_dmz_mask (
            const v8::Handle<v8::Value> Value,
            Mask &out) = 0;

      protected:
         JsModuleTypesV8 (const PluginInfo &Info);
         ~JsModuleTypesV8 ();

      private:
         JsModuleTypesV8 ();
         JsModuleTypesV8 (const JsModuleTypesV8 &);
         JsModuleTypesV8 &operator= (const JsModuleTypesV8 &);

         const PluginInfo &__Info;
   };

   //! \cond
   const char JsModuleTypesV8InterfaceName[] = "JsModuleTypesV8Interface";
   //! \endcond
};


inline dmz::JsModuleTypesV8 *
dmz::JsModuleTypesV8::cast (const Plugin *PluginPtr, const String &PluginName) {

   return (JsModuleTypesV8 *)lookup_rtti_interface (
      JsModuleTypesV8InterfaceName,
      PluginName,
      PluginPtr);
}


inline
dmz::JsModuleTypesV8::JsModuleTypesV8 (const PluginInfo &Info) :
      __Info (Info) {

   store_rtti_interface (JsModuleTypesV8InterfaceName, __Info, (void *)this);
}


inline
dmz::JsModuleTypesV8::~JsModuleTypesV8 () {

   remove_rtti_interface (JsModuleTypesV8InterfaceName, __Info);
}


inline dmz::String
dmz::JsModuleTypesV8::get_js_module_types_v8_name () const { return __Info.get_name (); }


inline dmz::Handle
dmz::JsModuleTypesV8::get_js_module_types_v8_handle () const { return __Info.get_handle (); }

#endif // DMZ_JS_MODULE_TYPES_V8_DOT_H
