#ifndef DMZ_JS_MODULE_V8_DOT_H
#define DMZ_JS_MODULE_V8_DOT_H

#include <dmzRuntimePlugin.h>
#include <dmzRuntimeRTTI.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

#include <v8.h>

namespace dmz {

   class JsModuleV8 {

      public:
         static JsModuleV8 *cast (
            const Plugin *PluginPtr,
            const String &PluginName = "");

         String get_js_module_v8_name () const;
         Handle get_js_module_v8_handle () const;

         // JsModuleV8 Interface
         virtual void reset_v8 () = 0;

         virtual v8::Handle<v8::Context> get_v8_context () = 0;

         virtual Boolean register_interface (
            const String &Name,
            v8::Persistent<v8::Object> object) = 0;

         virtual v8::Handle<v8::Object> require (const String &Value) = 0;

         virtual void handle_v8_exception (v8::TryCatch &tc) = 0;

      protected:
         JsModuleV8 (const PluginInfo &Info);
         ~JsModuleV8 ();

      private:
         JsModuleV8 ();
         JsModuleV8 (const JsModuleV8 &);
         JsModuleV8 &operator= (const JsModuleV8 &);

         const PluginInfo &__Info;
   };

   //! \cond
   const char JsModuleV8InterfaceName[] = "JsModuleV8Interface";
   //! \endcond
};


inline dmz::JsModuleV8 *
dmz::JsModuleV8::cast (const Plugin *PluginPtr, const String &PluginName) {

   return (JsModuleV8 *)lookup_rtti_interface (
      JsModuleV8InterfaceName,
      PluginName,
      PluginPtr);
}


inline
dmz::JsModuleV8::JsModuleV8 (const PluginInfo &Info) :
      __Info (Info) {

   store_rtti_interface (JsModuleV8InterfaceName, __Info, (void *)this);
}


inline
dmz::JsModuleV8::~JsModuleV8 () {

   remove_rtti_interface (JsModuleV8InterfaceName, __Info);
}


inline dmz::String
dmz::JsModuleV8::get_js_module_v8_name () const { return __Info.get_name (); }


inline dmz::Handle
dmz::JsModuleV8::get_js_module_v8_handle () const { return __Info.get_handle (); }

#endif // DMZ_JS_MODULE_V8_DOT_H
