#include "dmzJsExtV8Qt.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <QtGui/QtGui>
#include <QtUiTools/QUiLoader>

#include <QtCore/QDebug>

namespace {

void
local_qwidget_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QWidget *widget = (QWidget *)param;

      if (!widget->parentWidget ()) {

qWarning () << "=-=-=-=-=-=-=-=- local_qwidget_delete =-=-=-=-=-=-=-=- ";
         delete widget; widget = 0;
      }
   }

   object.Dispose (); object.Clear ();
}

};


dmz::V8Value
dmz::JsExtV8Qt::_uiloader_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Qt *self = _to_self (Args);
   const String Name = v8_to_string (Args[0]);

   if (self && Name) {

self->_log.warn << "_uiloader_load: " << Name << endl;

      QUiLoader loader;
      QFile file (Name.get_buffer ());
      file.open (QFile::ReadOnly);
      QWidget *widget = loader.load (&file, 0);
      file.close ();

      result = self->_create_v8_widget (widget);
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Qt::_widget_show (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Qt *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_widget_ptr (Args.This ());

      if (widget) {

self->_log.warn << "_widget_show" << endl;

         widget->show ();
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::JsExtV8Qt::JsExtV8Qt (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _types (0),
      _core (0) {

   _init (local);
}


dmz::JsExtV8Qt::~JsExtV8Qt () {

}


// Plugin Interface
void
dmz::JsExtV8Qt::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

   }
}


void
dmz::JsExtV8Qt::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_types) { _types = JsModuleTypesV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Qt::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Qt::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Qt::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface ("dmz/components/ui", _qtApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _widgetCtor = V8FunctionPersist::New (_widgetTemp->GetFunction ());
   }
   else if (State == JsExtV8::Shutdown) {

      _widgetCtor.Dispose (); _widgetCtor.Clear ();

      _qtApi.clear ();
      _v8Context.Clear ();
   }
}


dmz::V8Value
dmz::JsExtV8Qt::_create_v8_widget (QWidget *widget) {

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (widget) {
      
      V8Object obj;
      
      if (_widgetCtor.IsEmpty () == false) {
      
         obj = _widgetCtor->NewInstance ();
      }
      
      if (obj.IsEmpty () == false) {
         
         obj->SetInternalField (0, v8::External::Wrap ((void *)widget));
         
         v8::Persistent<v8::Object> persist = v8::Persistent<v8::Object>::New (obj);
         persist.MakeWeak ((void *)widget, local_qwidget_delete);
         
         result = obj;
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Object
dmz::JsExtV8Qt::_to_widget (V8Value value) {

   v8::HandleScope scope;

   V8Object result;

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_widgetTemp->HasInstance (obj)) {

         result = obj;
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


QWidget *
dmz::JsExtV8Qt::_to_widget_ptr (V8Value value) {

   v8::HandleScope scope;

   QWidget *result (0);

   V8Object obj = v8_to_object (value);

   if (obj.IsEmpty () == false) {

      if (_widgetTemp->HasInstance (obj)) {

         result = (QWidget *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


void
dmz::JsExtV8Qt::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // API
   _qtApi.add_function ("load", _uiloader_load, _self);

   _widgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate widgetProto = _widgetTemp->PrototypeTemplate ();
   widgetProto->Set ("show", v8::FunctionTemplate::New (_widget_show, _self));

   V8ObjectTemplate widgetInstance = _widgetTemp->InstanceTemplate ();
   widgetInstance->SetInternalFieldCount (1);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Qt (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Qt (Info, local);
}

};
