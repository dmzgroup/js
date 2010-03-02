#ifndef DMZ_JS_EXT_V8_DOT_H
#define DMZ_JS_EXT_V8_DOT_H

#include <dmzRuntimePlugin.h>
#include <dmzRuntimeRTTI.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

#include <v8.h>

namespace dmz {

   class JsModuleV8;

   class JsExtV8 {

      public:
         enum StateEnum {
            Register,
            Init,
            Shutdown,
         };

         enum ModeEnum {
            Store,
            Remove,
         };

         static JsExtV8 *cast (
            const Plugin *PluginPtr,
            const String &PluginName = "");

         String get_js_ext_v8_name () const;
         Handle get_js_ext_v8_handle () const;

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) = 0;
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context) = 0;
         virtual void update_js_ext_v8_state (const StateEnum State) = 0;

      protected:
         JsExtV8 (const PluginInfo &Info);
         ~JsExtV8 ();

      private:
         JsExtV8 ();
         JsExtV8 (const JsExtV8 &);
         JsExtV8 &operator= (const JsExtV8 &);

         const PluginInfo &__Info;
   };

   //! \cond
   const char JsExtV8InterfaceName[] = "JsExtV8Interface";
   //! \endcond
};


inline dmz::JsExtV8 *
dmz::JsExtV8::cast (const Plugin *PluginPtr, const String &PluginName) {

   return (JsExtV8 *)lookup_rtti_interface (
      JsExtV8InterfaceName,
      PluginName,
      PluginPtr);
}


inline
dmz::JsExtV8::JsExtV8 (const PluginInfo &Info) :
      __Info (Info) {

   store_rtti_interface (JsExtV8InterfaceName, __Info, (void *)this);
}


inline
dmz::JsExtV8::~JsExtV8 () {

   remove_rtti_interface (JsExtV8InterfaceName, __Info);
}


inline dmz::String
dmz::JsExtV8::get_js_ext_v8_name () const { return __Info.get_name (); }


inline dmz::Handle
dmz::JsExtV8::get_js_ext_v8_handle () const { return __Info.get_handle (); }

#endif // DMZ_JS_EXT_V8_DOT_H
