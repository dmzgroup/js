#ifndef DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
#define DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsModuleUiV8Qt.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

class QListWidgetItem;


namespace dmz {

   class JsModuleV8;
   class V8QtObject;
   class V8QtWidget;
   class V8QtButton;
   

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
         virtual v8::Handle<v8::Value> create_v8_list_widget_item (QListWidgetItem *value);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         struct State {
            
            JsModuleV8 *core;
            JsModuleUiV8QtBasic *ui;
            v8::Handle<v8::Context> context;
            
            State () : core (0), ui (0) {;}
         };

      protected:
         // Static Functions
         static JsModuleUiV8QtBasic *_to_self (const v8::Arguments &Args);
         
         // QUiLoader bindings
         static V8Value _uiloader_load (const v8::Arguments &Args);

         // QWidget bindings implemented in JsModuleUiV8QtBasicWidget.cpp
         static V8Value _widget_close (const v8::Arguments &Args);
         static V8Value _widget_enabled (const v8::Arguments &Args);
         static V8Value _widget_hide (const v8::Arguments &Args);
         static V8Value _widget_lookup (const v8::Arguments &Args);
         static V8Value _widget_observe (const v8::Arguments &Args);
         static V8Value _widget_show (const v8::Arguments &Args);

         // QAbstractButton bindings implemented in JsModuleUiV8QtBasicButton.cpp
         static V8Value _button_text (const v8::Arguments &Args);

         // QListWidgetItem bindings implemented in JsModuleUiV8QtBasicListWidget.cpp
         static V8Value _create_list_widget_item (const v8::Arguments &Args);
         static V8Value _list_widget_item_text (const v8::Arguments &Args);

         static V8Value _list_widget_add_item (const v8::Arguments &Args);
         static V8Value _list_widget_current_item (const v8::Arguments &Args);
         // static V8Value _list_widget_item (const v8::Arguments &Args);

         QListWidgetItem *_to_qt_list_widget_item (V8Value value);
         QWidget *_to_qt_widget (V8Value value);
         V8QtObject *_to_js_qt_object (V8Value value);
         
         void _init_widget ();
         void _init_button ();
         void _init_list_widget ();
         void _init_list_widget_item ();
         
         void _init (Config &local);

         Log _log;
         
         State _state;
         V8ValuePersist _self;

         V8InterfaceHelper _qtApi;

         V8FunctionTemplatePersist _widgetTemp;
         V8FunctionPersist _widgetCtor;

         V8FunctionTemplatePersist _buttonTemp;
         V8FunctionPersist _buttonCtor;

         V8FunctionTemplatePersist _listWidgetItemTemp;
         V8FunctionPersist _listWidgetItemCtor;

         V8FunctionTemplatePersist _listWidgetTemp;
         V8FunctionPersist _listWidgetCtor;
         
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


// inline dmz::JsModuleUiV8QtBasic *
// dmz::JsModuleUiV8QtBasic::_to_object (const v8::Arguments &Args, V8QtObject *object) {
// 
//    object = _to_object_ptr (Args.This ());
//    return (dmz::JsModuleUiV8QtBasic *)v8::External::Unwrap (Args.Data ());
// }


#endif // DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
