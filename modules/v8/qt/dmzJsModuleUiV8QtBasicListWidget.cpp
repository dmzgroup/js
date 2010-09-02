#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QAbstractButton>

namespace {

void
local_list_widget_item_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      dmz::V8QtListWidgetItem *ptr = (dmz::V8QtListWidgetItem *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};


V8Value
dmz::JsModuleRuntimeV8Basic::_create_list_widget_item (const v8::Arguments &Args) {

   v8::HandleScopesscope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      if (Args.Length () == 0) {

         return v8::ThrowException (
            v8::Exception::Error (v8::String::New ("Invalid parameters.")));
      }
      else if (Args[0]->IsString ()) {

          Config cd (*(v8::String::AsciiValue (Args[0])));
          result = self->create_v8_config (&cd);
      }
      else {

         Config *ptr = self->_to_config_ptr (Args[0]);

         result = self->create_v8_config (ptr);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_item_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      String param = v8_to_string (Args[0]);

      if (widget && param) {

         QListWidgetItem *item = static_cast<QListWidgetItem *>(widget);
         item->setText (param.get_buffer ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_list_widget_item () {

   v8::HandleScope scope;

   _listWidgetItemApi.add_function ("create", _create_list_widget_item, _self);

   _listWidgetItemTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   
   V8ObjectTemplate instance = _listWidgetTemp->InstanceTemplate ();
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
   proto->Set ("item", v8::FunctionTemplate::New (_list_widget_item, _self));
   proto->Set ("takeItem", v8::FunctionTemplate::New (_list_widget_take_item, _self));
}

