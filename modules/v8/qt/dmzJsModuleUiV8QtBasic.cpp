#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesStringTokenizer.h>
#include "dmzV8QtTypes.h"
#include <QtCore/QFile>
#include <QtGui/QWidget>
#include <QtUiTools/QUiLoader>


namespace {

static void
local_v8_qt_object_delete (dmz::V8ValuePersist object, void *param) {

   if (param) {

      dmz::V8QtObject *obj = (dmz::V8QtObject *)param;
      delete obj;
      obj = 0;
   }

   object.Dispose ();
   object.Clear ();
}

};


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_uiloader_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   const String Name = v8_to_string (Args[0]);

   if (self && Name) {

      QUiLoader loader;
      QFile file (Name.get_buffer ());
      file.open (QFile::ReadOnly);
      QWidget *widget = loader.load (&file, 0);
      file.close ();

      result = self->create_v8_widget (widget);
      
      self->_log.info << "Loaded UI: " << Name << endl;
   }

   return scope.Close (result);
}


dmz::JsModuleUiV8QtBasic::JsModuleUiV8QtBasic (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsModuleUiV8Qt (Info),
      JsExtV8 (Info),
      _log (Info),
      _qtApi () {

   _state.ui = this;
   _init (local);
}


dmz::JsModuleUiV8QtBasic::~JsModuleUiV8QtBasic () {

}


// Plugin Interface
void
dmz::JsModuleUiV8QtBasic::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

      _self.Dispose (); _self.Clear ();
   }
}


void
dmz::JsModuleUiV8QtBasic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// JsModuleUiV8Qt Interface
dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_widget (QWidget *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {
      
      V8Object vobj;
      V8QtObject *qobj;

      if (value->inherits ("QAbstractButton")) {
      
         if (!_buttonCtor.IsEmpty ()) {
            
            vobj = _buttonCtor->NewInstance ();
            qobj = new V8QtButton (value, &_state);
         }
      }
      else if (value->inherits ("QListWidget")) {
         
         if (!_listWidgetCtor.IsEmpty ()) {
            
            vobj = _listWidgetCtor->NewInstance ();
            qobj = new V8QtListWidget (value, &_state);
         }
      }
      else if (value->inherits ("QWidget")) {

         if (!_widgetCtor.IsEmpty ()) {
            
            vobj = _widgetCtor->NewInstance ();
            qobj = new V8QtWidget (value, &_state);
         }
      }

      if (!vobj.IsEmpty () && qobj) {
         
         vobj->SetInternalField (0, v8::External::Wrap ((void *)qobj));
         
         V8ObjectPersist persist = V8ObjectPersist::New (vobj);
         persist.MakeWeak ((void *)qobj, local_v8_qt_object_delete);
         
         result = vobj;
      }
   }

   return scope.Close (result);
}


// JsExtV8 Interface
void
dmz::JsModuleUiV8QtBasic::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) {
      
      if (!_state.core) { _state.core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {
      
      if (_state.core == &module) { _state.core = 0; }
   }
}


void
dmz::JsModuleUiV8QtBasic::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _state.context = context;
}


void
dmz::JsModuleUiV8QtBasic::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {
      
      if (_state.core) {

         _state.core->register_interface ("dmz/components/ui", _qtApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _widgetCtor = V8FunctionPersist::New (_widgetTemp->GetFunction ());
      _buttonCtor = V8FunctionPersist::New (_buttonTemp->GetFunction ());
      _listWidgetItemCtor = V8FunctionPersist::New (_listWidgetItemTemp->GetFunction ());
      _listWidgetCtor = V8FunctionPersist::New (_listWidgetTemp->GetFunction ());
   }
   else if (State == JsExtV8::Shutdown) {

      _widgetCtor.Dispose (); _widgetCtor.Clear ();
      _buttonCtor.Dispose (); _buttonCtor.Clear ();
      _listWidgetItemCtor.Dispose (); _listWidgetItemCtor.Clear ();
      _listWidgetCtor.Dispose (); _listWidgetCtor.Clear ();

      _qtApi.clear ();
      _state.context.Clear ();
   }
}


void
dmz::JsModuleUiV8QtBasic::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

   // ObsStruct *os = _obsTable.remove (InstanceHandle);
   // 
   // if (os) {
   // 
   //    while (os->list && _release_callback (os->list->self, os->list->func)) {;}
   // 
   //    delete os; os = 0;
   // }
}


QListWidgetItem *
dmz::JsModuleUiV8QtBasic::_to_qt_list_widget_item (V8Value value) {

   v8::HandleScope scope;
   QListWidgetItem *result (0);
   
   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {
      
      if (_listWidgetItemTemp->HasInstance (obj)) {

         result = (QListWidgetItem *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QWidget  *
dmz::JsModuleUiV8QtBasic::_to_qt_widget (V8Value value) {
   
   QWidget *result (0);
   V8QtObject *object = _to_js_qt_object (value);
   if (object) { result = object->get_qt_widget (); }
   return result;
}


dmz::V8QtObject *
dmz::JsModuleUiV8QtBasic::_to_js_qt_object (V8Value value) {

   v8::HandleScope scope;
   V8QtObject *result (0);
   
   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {
      
      if (_widgetTemp->HasInstance (obj)) {

         result = (V8QtObject *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


// JsModuleUiV8QtBasic Interface
void
dmz::JsModuleUiV8QtBasic::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // API
   _qtApi.add_function ("load", _uiloader_load, _self);

   _init_widget ();
   _init_button ();
   _init_list_widget_item ();
   _init_list_widget ();
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleUiV8QtBasic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleUiV8QtBasic (Info, local);
}

};
