#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtListWidget.h"
#include <QtGui/QAbstractButton>
#include <QtGui/QListWidgetItem>

using namespace dmz;

namespace {


void
local_list_widget_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      V8QtListWidget *ptr = (V8QtListWidget *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_item_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QListWidgetItem *item = self->_to_qt_list_widget_item (Args.This ());

      if (item) {

         if (Args.Length ()) {

            item->setText (*(v8::String::AsciiValue (Args[0])));
         }

         result = v8::String::New (qPrintable (item->text ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_add_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      String param = v8_to_string (Args[0]);

      if (widget) {

         QListWidget *lw = qobject_cast<QListWidget *>(widget);

         if (lw) {

            lw->addItem (param.get_buffer ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_current_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         QListWidget *lw = qobject_cast<QListWidget *>(widget);
         if (lw) {

            QListWidgetItem *item = lw->currentItem ();
            if (item) {

               result = self->create_v8_list_widget_item (item);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_list_widget_item (QListWidgetItem *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_listWidgetItemCtor.IsEmpty ()) { obj = _listWidgetItemCtor->NewInstance (); }

      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_list_widget_item () {

   v8::HandleScope scope;

   _listWidgetItemTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _listWidgetItemTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _listWidgetItemTemp->PrototypeTemplate ();
   proto->Set ("text", v8::FunctionTemplate::New (_list_widget_item_text, _self));
}


void
dmz::JsModuleUiV8QtBasic::_init_list_widget () {

   v8::HandleScope scope;

   _listWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _listWidgetTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _listWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _listWidgetTemp->PrototypeTemplate ();
   proto->Set ("addItem", v8::FunctionTemplate::New (_list_widget_add_item, _self));
   proto->Set ("currentItem", v8::FunctionTemplate::New (_list_widget_current_item, _self));
   // proto->Set ("item", v8::FunctionTemplate::New (_list_widget_item, _self));
   // proto->Set ("takeItem", v8::FunctionTemplate::New (_list_widget_take_item, _self));
}

