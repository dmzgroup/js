#ifndef DMZ_JS_MODULE_UI_V8_QT_DOT_H
#define DMZ_JS_MODULE_UI_V8_QT_DOT_H

#include <dmzRuntimePlugin.h>
#include <dmzRuntimeRTTI.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

#include <v8.h>

class QListWidgetItem;
class QWidget;


namespace dmz {

   class JsModuleUiV8Qt {

      public:
         static JsModuleUiV8Qt *cast (
            const Plugin *PluginPtr,
            const String &PluginName = "");

         String get_js_module_ui_v8_qt_name () const;
         Handle get_js_module_ui_v8_qt_handle () const;

         // JsModuleUiV8Qt Interface
         virtual v8::Handle<v8::Value> create_v8_widget (QWidget *value) = 0;
         virtual v8::Handle<v8::Value> create_v8_list_widget_item (QListWidgetItem *value) = 0;

      protected:
         JsModuleUiV8Qt (const PluginInfo &Info);
         ~JsModuleUiV8Qt ();

      private:
         JsModuleUiV8Qt ();
         JsModuleUiV8Qt (const JsModuleUiV8Qt &);
         JsModuleUiV8Qt &operator= (const JsModuleUiV8Qt &);

         const PluginInfo &__Info;
   };

   //! \cond
   const char JsModuleUiV8QtInterfaceName[] = "JsModuleUiV8QtInterface";
   //! \endcond
};


inline dmz::JsModuleUiV8Qt *
dmz::JsModuleUiV8Qt::cast (const Plugin *PluginPtr, const String &PluginName) {

   return (JsModuleUiV8Qt *)lookup_rtti_interface (
      JsModuleUiV8QtInterfaceName,
      PluginName,
      PluginPtr);
}


inline
dmz::JsModuleUiV8Qt::JsModuleUiV8Qt (const PluginInfo &Info) :
      __Info (Info) {

   store_rtti_interface (JsModuleUiV8QtInterfaceName, __Info, (void *)this);
}


inline
dmz::JsModuleUiV8Qt::~JsModuleUiV8Qt () {

   remove_rtti_interface (JsModuleUiV8QtInterfaceName, __Info);
}


inline dmz::String
dmz::JsModuleUiV8Qt::get_js_module_ui_v8_qt_name () const { return __Info.get_name (); }


inline dmz::Handle
dmz::JsModuleUiV8Qt::get_js_module_ui_v8_qt_handle () const { return __Info.get_handle (); }


#endif // DMZ_JS_MODULE_UI_V8_QT_DOT_H
