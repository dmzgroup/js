#include "dmzJsModuleUiV8QtBasic.h"
#include "dmzJsModuleUiV8QtBasicWidgets.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesStringTokenizer.h>
#include <QtGui/QtGui>
#include <QtUiTools/QUiLoader>

#include <QtCore/QDebug>

namespace {

static const char ScopeChar = '.';

static void
local_get_scope (const dmz::String &Path, dmz::String &scope, dmz::String &name) {

   const dmz::Int32 Length (Path.get_length ());
   dmz::Boolean done (dmz::False);
   dmz::Int32 place = Length - 1;

   while (!done) {

      if (place < 0) { done = dmz::True; }
      else if (Path.get_char (place) == ScopeChar) {

         done = dmz::True;
      }
      else { place--; }
   }

   if (place >= 0) {

      scope = Path.get_sub (0, place - 1);
      name = Path.get_sub (place + 1);
   }
   else { name = Path; scope.flush (); }
}

   
static void
local_qwidget_delete (dmz::V8ValuePersist object, void *param) {

   if (param) {

      QWidget *widget = (QWidget *)param;

      if (!widget->parentWidget ()) {

         delete widget; widget = 0;
      }
   }

   object.Dispose (); object.Clear ();
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


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_lookup (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_widget_ptr (Args.This ());
      String param = v8_to_string (Args[0]);

      if (widget && param) {

         QWidget *child = widget->findChild<QWidget *>(param.get_buffer ());
         
         if (child) {
            
            result = self->create_v8_widget (child);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_show (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_widget_ptr (Args.This ());

      if (widget) {

         widget->show ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self && Args[0]->IsObject () && Args[2]->IsFunction ()) {

      QWidget *widget = self->_to_widget_ptr (Args.This ());
      
      V8Object src = V8Object::Cast (Args[0]);
      const String Method = v8_to_string (Args[1]).to_lower ();
      V8Function func = v8_to_function (Args[2]);
      
      if (widget) {

         V8QtObserver *obs = new V8QtObserver (src, func, widget, self->_root);
         
         if (Method == "clicked") {

            obs->connect (widget, SIGNAL (clicked ()), SLOT (observe ()));
         }
         
         self->_obsList.append (obs);
         
         result = func;
      }
   }

   return scope.Close (result);
}


// dmz::V8Value
// dmz::JsModuleUiV8QtBasic::_button_clicked (const v8::Arguments &Args) {
// 
//    v8::HandleScope scope;
//    V8Value result = v8::Undefined ();
// 
//    JsModuleUiV8QtBasic *self = _to_self (Args);
// 
//    if (self) {
// 
//       V8Object obj = v8_to_object (Args[0]);
// 
//       const String Name = self->_core->get_instance_name (obj);;
//       self->_log.warn << "_button_clicked: " << Name << endl;
// 
//       QWidget *widget = self->_to_widget_ptr (Args.This ());
// 
//       if (widget) {
// 
//          self->_log.warn << "CLICKED!!!!" << endl;
//       }
//    }
// 
//    return scope.Close (result);
// }
// 
// 
// dmz::V8Value
// dmz::JsModuleUiV8QtBasic::_button_clicked_observe (const v8::Arguments &Args) {
// 
//    v8::HandleScope scope;
//    V8Value result = v8::Undefined ();
// 
//    JsModuleUiV8QtBasic *self = _to_self (Args);
// 
//    if (self) {
// 
//       V8Object obj = v8_to_object (Args[0]);
// 
//       const String Name = self->_core->get_instance_name (obj);;
//       self->_log.warn << "_button_clicked_observe: " << Name << endl;
//       
//       QWidget *widget = self->_to_widget_ptr (Args.This ());
// 
//       if (widget) {
// 
//       }
//    }
// 
//    return scope.Close (result);
// }


dmz::JsModuleUiV8QtBasic::InterfaceStruct::InterfaceStruct (
      const String &TheName) :
      Name (TheName) {;}
      

dmz::JsModuleUiV8QtBasic::InterfaceStruct::~InterfaceStruct () {

   funcTemplate.Dispose ();
   funcTemplate.Clear ();
   table.empty ();
};


#if 0
dmz::JsModuleUiV8QtBasic::WidgetInterfaceStruct::WidgetInterfaceStruct () {
      
   // v8::HandleScope scope;
   
   // funcTemplate = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   // funcCtor = V8FunctionPersist::New (funcTemplate->GetFunction ());
      
   // V8ObjectTemplate funcInstance = funcTemplate->InstanceTemplate ();
   // funcInstance->SetInternalFieldCount (1);
   
}

   
dmz::JsModuleUiV8QtBasic::WidgetInterfaceStruct::~WidgetInterfaceStruct () {

   clear ();
   funcTemplate.Dispose ();
   funcTemplate.Clear ();
   table.empty ();
};


void
dmz::JsModuleUiV8QtBasic::WidgetInterfaceStruct::clear () {

   funcCtor.Dispose ();
   funcCtor.Clear ();
}


dmz::Boolean
dmz::JsModuleUiV8QtBasic::WidgetInterfaceStruct::add_prototype_function (
      const String &Name,
      v8::InvocationCallback cb,
      V8Value data) {

   v8::HandleScope scope;
   V8ObjectTemplate funcProto = funcTemplate->PrototypeTemplate ();
   funcProto->Set (Name.get_buffer (), v8::FunctionTemplate::New (cb, data));
   return True;
}


dmz::Boolean
dmz::JsModuleUiV8QtBasic::WidgetInterfaceStruct::add_function (
      const String &Path,
      v8::InvocationCallback cb,
      V8Value data) {

   v8::HandleScope v8Scope;
   Boolean result (False);

   String scope, name;

   local_get_scope (Path, scope, name);

   HashTableStringTemplate<InterfaceStruct> *cTable (&table);

   InterfaceStruct *foundIs (0);

   if (scope) {

      StringTokenizer stz (scope, ScopeChar);

      String token;

      while (cTable && stz.get_next (token)) {

         foundIs = cTable->lookup (token);
         if (foundIs) { cTable = &(foundIs->table); }
      }
   }

   InterfaceStruct *is = new InterfaceStruct (name);

   if (name && cTable && cTable->store (name, is)) {

      is->funcTemplate = V8FunctionTemplatePersist::New (
         v8::FunctionTemplate::New (cb, data));

      if (foundIs) {

         if (!(foundIs->funcTemplate.IsEmpty ())) {

            foundIs->funcTemplate->Set (name.get_buffer (), is->funcTemplate);
            result = True;
         }
      }
      else {

         if (!funcTemplate.IsEmpty ()) {

            funcTemplate->Set (name.get_buffer (), is->funcTemplate);
            result = True;
         }
      }
   }
   else if (is) { delete is; is = 0; }

   return result;
}


dmz::V8Object
dmz::JsModuleUiV8QtBasic::WidgetInterfaceStruct::get_new_instance () {

   v8::HandleScope scope;
   V8Object result;
   if (!funcCtor.IsEmpty ()) { result = funcCtor->NewInstance (); }
   return result;
}
#endif


dmz::JsModuleUiV8QtBasic::JsModuleUiV8QtBasic (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsModuleUiV8Qt (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _root (0),
      _qtApi () {

   _root = new QObject (0);
   _root->setObjectName (get_plugin_name ().get_buffer ());
   
   _init (local);
}


dmz::JsModuleUiV8QtBasic::~JsModuleUiV8QtBasic () {

   delete _root;
   _root = 0;
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

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {
      
      V8Object obj;

      // if (value->inherits ("QAbstractButton")) {
      // 
      //    if (!_buttonCtor.IsEmpty ()) { obj = _buttonCtor->NewInstance (); }
      // }
      // if (value->inherits ("QWidget")) {

         if (!_widgetCtor.IsEmpty ()) { obj = _widgetCtor->NewInstance (); }
      // }
      
      if (!obj.IsEmpty ()) {
         
         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         
         V8ObjectPersist persist = V8ObjectPersist::New (obj);
         persist.MakeWeak ((void *)value, local_qwidget_delete);
         
         result = obj;
      }
   }

   return scope.Close (result);
}


// JsExtV8 Interface
void
dmz::JsModuleUiV8QtBasic::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) {
      
      if (!_core) { _core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {
      
      if (_core == &module) { _core = 0; }
   }
}


void
dmz::JsModuleUiV8QtBasic::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsModuleUiV8QtBasic::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {
      
      if (_core) {

         _core->register_interface ("dmz/components/ui", _qtApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _widgetCtor = V8FunctionPersist::New (_widgetTemp->GetFunction ());
      // _buttonCtor = V8FunctionPersist::New (_buttonTemp->GetFunction ());
   }
   else if (State == JsExtV8::Shutdown) {

      _widgetCtor.Dispose (); _widgetCtor.Clear ();
      // _buttonCtor.Dispose (); _buttonCtor.Clear ();

      _qtApi.clear ();
      _v8Context.Clear ();
   }
}


dmz::V8Object
dmz::JsModuleUiV8QtBasic::_to_widget (V8Value value) {

   v8::HandleScope scope;

   V8Object result;

   V8Object obj = v8_to_object (value);

   if (!obj.IsEmpty ()) {

      if (_widgetTemp->HasInstance (obj)) {
         
         result = obj;
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


QWidget *
dmz::JsModuleUiV8QtBasic::_to_widget_ptr (V8Value value) {

   v8::HandleScope scope;

   QWidget *result (0);

   V8Object obj = v8_to_object (value);

   if (!obj.IsEmpty ()) {

      if (_widgetTemp->HasInstance (obj)) {

         result = (QWidget *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


#if 0
dmz::Boolean
dmz::JsModuleUiV8QtBasic::_add_function (
      V8FunctionTemplatePersist funcTemp;
      const String &Path,
      v8::InvocationCallback cb,
      V8Value data) {

   v8::HandleScope v8Scope;
   Boolean result (False);

   String scope, name;

   local_get_scope (Path, scope, name);

   HashTableStringTemplate<InterfaceStruct> *cTable (&table);

   InterfaceStruct *foundIs (0);

   if (scope) {

      StringTokenizer stz (scope, ScopeChar);

      String token;

      while (cTable && stz.get_next (token)) {

         foundIs = cTable->lookup (token);
         if (foundIs) { cTable = &(foundIs->table); }
      }
   }

   InterfaceStruct *is = new InterfaceStruct (name);

   if (name && cTable && cTable->store (name, is)) {

      is->funcTemplate = V8FunctionTemplatePersist::New (
         v8::FunctionTemplate::New (cb, data));

      if (foundIs) {

         if (!(foundIs->funcTemplate.IsEmpty ())) {

            foundIs->funcTemplate->Set (name.get_buffer (), is->funcTemplate);
            result = True;
         }
      }
      else {

         if (!funcTemplate.IsEmpty ()) {

            funcTemplate->Set (name.get_buffer (), is->funcTemplate);
            result = True;
         }
      }
   }
   else if (is) { delete is; is = 0; }

   return result;
}
#endif


// JsModuleUiV8QtBasic Interface
void
dmz::JsModuleUiV8QtBasic::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // API
   _qtApi.add_function ("load", _uiloader_load, _self);

   _widgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate widgetInstance = _widgetTemp->InstanceTemplate ();
   widgetInstance->SetInternalFieldCount (1);

   V8ObjectTemplate widgetProto = _widgetTemp->PrototypeTemplate ();
   widgetProto->Set ("lookup", v8::FunctionTemplate::New (_widget_lookup, _self));
   widgetProto->Set ("show", v8::FunctionTemplate::New (_widget_show, _self));
   widgetProto->Set ("observe", v8::FunctionTemplate::New (_widget_observe, _self));

   // _buttonTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   // 
   // V8ObjectTemplate buttonInstance = _buttonTemp->InstanceTemplate ();
   // buttonInstance->SetInternalFieldCount (1);

   // V8ObjectTemplate buttonProto = _buttonTemp->PrototypeTemplate ();
   // buttonProto->Set (
   //    "clicked",
   //    v8::FunctionTemplate::New (_button_clicked, _self));

   // _add_function (_buttonTemp, "clicked", _button_clicked_observe, _self);
   // _buttonApi.add_function ("clicked", _button_clicked, _self);
   // _buttonApi.add_function ("clicked.observer", _button_clicked_observe, _self);
   
   // String name ("clicked");
   // 
   // InterfaceStruct *is = new InterfaceStruct (name);
   // 
   // if (_buttonTable.store (name, is)) {
   // 
   //    is->funcTemplate = V8FunctionTemplatePersist::New (
   //       v8::FunctionTemplate::New (_button_clicked, _self));
   // 
   //    buttonProto->Set (name.get_buffer (), is->funcTemplate);
   //    
   //    name = "observer";
   //    InterfaceStruct *is2 = new InterfaceStruct (name);
   //    is2->funcTemplate = V8FunctionTemplatePersist::New (
   //       v8::FunctionTemplate::New (_button_clicked_observe, _self));
   //       
   //    if (is->table.store (name, is2)) {
   //       
   //       if (!is->funcTemplate.IsEmpty ()) {
   //       
   //          V8ObjectTemplate isProto = is->funcTemplate->PrototypeTemplate ();
   //       
   //          isProto->Set (name.get_buffer (), is2->funcTemplate);
   //       }
   //    }
   // }
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
