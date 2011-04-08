#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QScrollArea>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_scroll_area_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QScrollArea *scrollArea = self->v8_to_qobject<QScrollArea>(Args.This ());
      if (scrollArea) {

         if (Args.Length ()) {

            QWidget *widget = self->_to_qwidget (Args[0]);
            if (widget) { scrollArea->setWidget(widget); }
         }

         result = self->create_v8_qwidget (scrollArea->widget());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_scroll_area_ensure_visible (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QScrollArea *scrollArea = self->v8_to_qobject<QScrollArea>(Args.This ());
      if (scrollArea) {

         if (Args[0]->IsInt32 ()) {

            Int32 x = v8_to_int32 (Args[0]);
            Int32 y = v8_to_int32 (Args[1]);

            scrollArea->ensureVisible (x, y);
         }
         else {

            QWidget *widget = self->_to_qwidget (Args[0]);
            if (widget) { scrollArea->ensureWidgetVisible (widget); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_scroll_area_widget_resizable (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QScrollArea *scrollArea = self->v8_to_qobject<QScrollArea>(Args.This ());
      if (scrollArea) {

         if (Args.Length () > 0) {

            scrollArea->setWidgetResizable (v8_to_boolean(Args[0]));
         }

         result = v8::Boolean::New (scrollArea->widgetResizable ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_scroll_area (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QScrollArea *widget = new QScrollArea (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_scroll_area () {

   v8::HandleScope scope;

   _scrollAreaTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _scrollAreaTemp->Inherit (_frameTemp);

   V8ObjectTemplate instance = _scrollAreaTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _scrollAreaTemp->PrototypeTemplate ();
   proto->Set ("widget", v8::FunctionTemplate::New (_scroll_area_widget, _self));
   proto->Set ("ensureVisible", v8::FunctionTemplate::New (_scroll_area_ensure_visible, _self));
   proto->Set ("widgetResizable", v8::FunctionTemplate::New (_scroll_area_widget_resizable, _self));

   _scrollAreaApi.add_function ("create", _create_scroll_area, _self);
}

