#ifndef DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
#define DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsModuleUiV8Qt.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <QtCore/QList>

class QObject;


namespace dmz {

   class JsModuleV8;
   class V8QtObserver;

   class JsModuleUiV8QtBasic :
         public Plugin,
         public JsModuleUiV8Qt,
         public JsExtV8 {

      public:
         JsModuleUiV8QtBasic (const PluginInfo &Info, Config &local);
         ~JsModuleUiV8QtBasic ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsModuleUiV8Qt Interface
         virtual v8::Handle<v8::Value> create_v8_widget (QWidget *value);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

      protected:
         struct InterfaceStruct {

            const String Name;
            V8FunctionTemplatePersist funcTemplate;
            HashTableStringTemplate<InterfaceStruct> table;

            InterfaceStruct (const String &TheName);
            ~InterfaceStruct ();
         };

         // struct WidgetInterfaceStruct {
         //    
         //    HashTableStringTemplate<InterfaceStruct> table;
         //    V8FunctionTemplatePersist funcTemplate;
         //    V8FunctionPersist funcCtor;
         // 
         //    WidgetInterfaceStruct ();
         //    ~WidgetInterfaceStruct ();
         //    
         //    void clear ();
         //    
         //    Boolean add_prototype_function (
         //       const String &Name,
         //       v8::InvocationCallback cb,
         //       V8Value data);
         // 
         //    Boolean add_function (
         //       const String &Name,
         //       v8::InvocationCallback cb,
         //       V8Value data);
         //       
         //    V8Object get_new_instance ();
         // };
         
         // Static Functions
         static JsModuleUiV8QtBasic *_to_self (const v8::Arguments &Args);
         
         // Qt API
         static V8Value _uiloader_load (const v8::Arguments &Args);

         // QWidget API
         static V8Value _widget_lookup (const v8::Arguments &Args);
         static V8Value _widget_show (const v8::Arguments &Args);
         static V8Value _widget_observe (const v8::Arguments &Args);

         // QAbstraceButton API
         // static V8Value _button_clicked (const v8::Arguments &Args);
         // static V8Value _button_clicked_observe (const v8::Arguments &Args);

         V8Object _to_widget (V8Value value);
         QWidget *_to_widget_ptr (V8Value value);
         
         void _init (Config &local);

         Log _log;
         
         JsModuleV8 *_core;
         QObject *_root;
         
         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         V8InterfaceHelper _qtApi;

         V8FunctionTemplatePersist _widgetTemp;
         V8FunctionPersist _widgetCtor;
         
         QList<V8QtObserver *>_obsList;

         // V8FunctionTemplatePersist _buttonTemp;
         // V8FunctionPersist _buttonCtor;
         // HashTableStringTemplate<InterfaceStruct> _buttonTable;
         
      private:
         JsModuleUiV8QtBasic ();
         JsModuleUiV8QtBasic (const JsModuleUiV8QtBasic &);
         JsModuleUiV8QtBasic &operator= (const JsModuleUiV8QtBasic &);
   };
};


inline dmz::JsModuleUiV8QtBasic *
dmz::JsModuleUiV8QtBasic::_to_self (const v8::Arguments &Args) {

   return (dmz::JsModuleUiV8QtBasic *)v8::External::Unwrap (Args.Data ());
}


#endif // DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
