#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtStackedWidget.h"
#include <QtGui/QAbstractButton>
#include <QtGui/QStackedWidget>

using namespace dmz;


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_stacked_widget_add (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QStackedWidget *sw = self->v8_to_qobject<QStackedWidget>(Args.This ());
      QWidget *qw = self->_to_qwidget (Args[0]);

      if (sw && qw) {

         if (Args.Length () > 1) {

            result = v8::Number::New (sw->insertWidget (v8_to_int32 (Args[1]), qw));
         }
         else {

            result = v8::Number::New (sw->addWidget (qw));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_stacked_widget_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QStackedWidget *sw = self->v8_to_qobject<QStackedWidget>(Args.This ());
      QWidget *qw = self->_to_qwidget (Args[0]);

      if (sw && qw) {

         sw->removeWidget (qw);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_stacked_widget_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QStackedWidget *sw = self->v8_to_qobject<QStackedWidget>(Args.This ());
      if (sw) {

         result = v8::Number::New (sw->count ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_stacked_widget_current_index (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QStackedWidget *sw = self->v8_to_qobject<QStackedWidget>(Args.This ());
      if (sw) {

         if (Args.Length () > 0) {

            sw->setCurrentIndex (v8_to_int32 (Args[0]));
         }

         result = v8::Number::New (sw->currentIndex ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_stacked_widget_current_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QStackedWidget *sw = self->v8_to_qobject<QStackedWidget>(Args.This ());
      if (sw) {

         QWidget *qw;

         if (Args.Length () > 0) {

            qw = self->_to_qwidget (Args[0]);
            sw->setCurrentWidget (qw);
         }

         qw = sw->currentWidget ();
         if (qw) {

            result = self->create_v8_qwidget (qw);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_stacked_widget_at (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QStackedWidget *sw = self->v8_to_qobject<QStackedWidget>(Args.This ());
      if (sw) {

         QWidget *qw = sw->widget(v8_to_int32 (Args[0]));
         if (qw) {

            result = self->create_v8_qwidget (qw);
         }
      }
   }

   return scope.Close (result);
}



dmz::V8Value
dmz::JsModuleUiV8QtBasic::_stacked_widget_index_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QStackedWidget *sw = self->v8_to_qobject<QStackedWidget>(Args.This ());
      QWidget *qw = self->_to_qwidget (Args[0]);

      if (sw && qw) {

         result = v8::Number::New (sw->indexOf (qw));
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_stacked_widget () {

   v8::HandleScope scope;

   _stackedWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _stackedWidgetTemp->Inherit (_frameTemp);

   V8ObjectTemplate instance = _stackedWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _stackedWidgetTemp->PrototypeTemplate ();
   proto->Set ("add", v8::FunctionTemplate::New (_stacked_widget_add, _self));
   proto->Set ("remove", v8::FunctionTemplate::New (_stacked_widget_remove, _self));
   proto->Set ("count", v8::FunctionTemplate::New (_stacked_widget_count, _self));
   proto->Set ("currentIndex", v8::FunctionTemplate::New (_stacked_widget_current_index, _self));
   proto->Set ("currentWidget", v8::FunctionTemplate::New (_stacked_widget_current_widget, _self));
   proto->Set ("at", v8::FunctionTemplate::New (_stacked_widget_at, _self));
   proto->Set ("indexOf", v8::FunctionTemplate::New (_stacked_widget_index_of, _self));
}

