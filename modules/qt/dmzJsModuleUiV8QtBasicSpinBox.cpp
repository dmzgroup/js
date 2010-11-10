#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtGui/QSpinBox>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_spinbox_maximum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QSpinBox *spinbox = self->v8_to_qobject<QSpinBox>(Args.This ());
      if (spinbox) {

         if (Args.Length ()) {

            spinbox->setMaximum (v8_to_number (Args[0]));
         }

         result = v8::Number::New (spinbox->maximum ());
      }
      else {

         QDoubleSpinBox *dspinbox = self->v8_to_qobject<QDoubleSpinBox>(Args.This ());
         if (dspinbox) {

            if (Args.Length ()) {

               dspinbox->setMaximum (v8_to_number (Args[0]));
            }

            result = v8::Number::New (dspinbox->maximum ());
         }

      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_spinbox_minimum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QSpinBox *spinbox = self->v8_to_qobject<QSpinBox>(Args.This ());
      if (spinbox) {

         if (Args.Length ()) {

            spinbox->setMinimum (v8_to_number (Args[0]));
         }

         result = v8::Number::New (spinbox->minimum ());
      }
      else {

         QDoubleSpinBox *dspinbox = self->v8_to_qobject<QDoubleSpinBox>(Args.This ());
         if (dspinbox) {

            if (Args.Length ()) {

               dspinbox->setMinimum (v8_to_number (Args[0]));
            }

            result = v8::Number::New (dspinbox->minimum ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_spinbox_value (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QSpinBox *spinbox = self->v8_to_qobject<QSpinBox>(Args.This ());
      if (spinbox) {

         if (Args.Length ()) {

            spinbox->setValue (v8_to_number (Args[0]));
         }

         result = v8::Number::New (spinbox->value ());
      }
      else {

         QDoubleSpinBox *dspinbox = self->v8_to_qobject<QDoubleSpinBox>(Args.This ());
         if (dspinbox) {

            if (Args.Length ()) {

               dspinbox->setValue (v8_to_number (Args[0]));
            }

            result = v8::Number::New (dspinbox->value ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_spinbox_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractSpinBox *spinbox = self->v8_to_qobject<QAbstractSpinBox>(Args.This ());
      if (spinbox) {

         result = qstring_to_v8 (spinbox->text ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_spinbox (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QSpinBox *widget = new QSpinBox (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_spinbox () {

   v8::HandleScope scope;

   _spinBoxTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _spinBoxTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _spinBoxTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _spinBoxTemp->PrototypeTemplate ();
   proto->Set ("maximum", v8::FunctionTemplate::New (_spinbox_maximum, _self));
   proto->Set ("minimum", v8::FunctionTemplate::New (_spinbox_minimum, _self));
   proto->Set ("value", v8::FunctionTemplate::New (_spinbox_value, _self));
   proto->Set ("text", v8::FunctionTemplate::New (_spinbox_text, _self));

   _spinBoxApi.add_function ("create", _create_spinbox, _self);
}
