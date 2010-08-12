#ifndef DMZ_JS_EXT_V8_QT_DOT_H
#define DMZ_JS_EXT_V8_QT_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableUInt32Template.h>

#include <v8.h>

class QWidget;


namespace dmz {

   class JsModuleTypesV8;
   class JsModuleV8;

   class JsExtV8Qt :
         public Plugin,
         public JsExtV8 {

      public:
         JsExtV8Qt (const PluginInfo &Info, Config &local);
         ~JsExtV8Qt ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsExtV8 Interfacfe
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

      protected:
         // Static Functions
         static JsExtV8Qt *_to_self (const v8::Arguments &Args);
         static V8Value _qt_load (const v8::Arguments &Args);
         
         // Widget API
         static V8Value _uiloader_load (const v8::Arguments &Args);
         static V8Value _widget_show (const v8::Arguments &Args);

         V8Value _create_v8_widget (QWidget *widget);

         V8Object _to_widget (V8Value value);
         QWidget *_to_widget_ptr (V8Value value);

         void _init (Config &local);

         Log _log;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         JsModuleTypesV8 *_types;
         JsModuleV8 *_core;

         V8InterfaceHelper _qtApi;

         V8FunctionTemplatePersist _widgetTemp;
         V8FunctionPersist _widgetCtor;

      private:
         JsExtV8Qt ();
         JsExtV8Qt (const JsExtV8Qt &);
         JsExtV8Qt &operator= (const JsExtV8Qt &);
   };
};


inline dmz::JsExtV8Qt *
dmz::JsExtV8Qt::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Qt *)v8::External::Unwrap (Args.Data ());
}


#endif // DMZ_JS_EXT_V8_QT_DOT_H
