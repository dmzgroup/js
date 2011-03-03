#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtTabWidget.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QAbstractButton>
#include <QtGui/QTabWidget>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_add (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      QWidget *qw = self->_to_qwidget (Args[0]);
      String label = v8_to_string(Args[1]);

      if (tw && qw) {

         if (Args.Length () == 2) {

            result = v8::Number::New (tw->addTab (qw, label.get_buffer ()));
         }
         else if (Args.Length () > 2) {

            result = v8::Number::New (
               tw->insertTab (v8_to_int32 (Args[2]), qw, label.get_buffer ()));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_remove (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      if (tw) {

         tw->removeTab (v8_to_int32 (Args[0]));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      if (tw) {

         result = v8::Number::New (tw->count ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_current_index (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      if (tw) {

         if (Args.Length () > 0) {

            tw->setCurrentIndex (v8_to_int32 (Args[0]));
         }

         result = v8::Number::New (tw->currentIndex ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_current_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      if (tw) {

         QWidget *qw = 0;
         if (Args.Length () > 0) {

            qw = self->_to_qwidget (Args[0]);
            if (qw) {

               tw->setCurrentWidget (qw);
            }
         }

         qw = tw->currentWidget ();
         if (qw) {

            result = self->create_v8_qwidget (qw);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_at (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      if (tw && (Args.Length () > 0)) {

         QWidget *qw = tw->widget(v8_to_int32 (Args[0]));
         if (qw) {

            result = self->create_v8_qwidget (qw);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_index_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      QWidget *qw = self->_to_qwidget (Args[0]);

      if (tw && qw) {

         result = v8::Number::New (tw->indexOf (qw));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_tab_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      if (tw && (Args.Length () > 0)) {

         int index = v8_to_int32 (Args[0]);

         if (Args.Length () > 1) {

            String label = v8_to_string (Args[1]);
            if (label) {

               tw->setTabText (index, label.get_buffer ());
            }
         }

         result = qstring_to_v8 (tw->tabText (index));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tab_widget_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTabWidget *tw = self->v8_to_qobject<QTabWidget>(Args.This ());
      if (tw) { tw->clear (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_tab_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QTabWidget *widget = new QTabWidget (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_tab_widget () {

   v8::HandleScope scope;

   _tabWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _tabWidgetTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _tabWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _tabWidgetTemp->PrototypeTemplate ();
   proto->Set ("add", v8::FunctionTemplate::New (_tab_widget_add, _self));
   proto->Set ("remove", v8::FunctionTemplate::New (_tab_widget_remove, _self));
   proto->Set ("count", v8::FunctionTemplate::New (_tab_widget_count, _self));

   proto->Set (
      "currentIndex",
      v8::FunctionTemplate::New (_tab_widget_current_index, _self));

   proto->Set (
      "currentWidget",
      v8::FunctionTemplate::New (_tab_widget_current_widget, _self));

   proto->Set ("at", v8::FunctionTemplate::New (_tab_widget_at, _self));
   proto->Set ("indexOf", v8::FunctionTemplate::New (_tab_widget_index_of, _self));
   proto->Set ("tabText", v8::FunctionTemplate::New (_tab_widget_tab_text, _self));
   proto->Set ("clear", v8::FunctionTemplate::New (_tab_widget_clear, _self));

   _tabApi.add_function ("create", _create_tab_widget, _self);
}

