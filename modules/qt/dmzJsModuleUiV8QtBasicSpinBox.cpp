#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QSpinBox>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_spinbox_maximum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QSpinBox *spinbox = qobject_cast<QSpinBox *>(widget);

         if (spinbox) {

            if (!Args.Length ()) {

               result = v8::Number::New (spinbox->maximum ());
            }
            else {

               spinbox->setMaximum (v8_to_number (Args[0]));
            }
         }
         else {

            QDoubleSpinBox *dspinbox = qobject_cast<QDoubleSpinBox *>(widget);
            if (dspinbox) {

               if (!Args.Length ()) {

                  result = v8::Number::New (dspinbox->maximum ());
               }
               else {

                  dspinbox->setMaximum (v8_to_number (Args[0]));
               }
            }

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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QSpinBox *spinbox = qobject_cast<QSpinBox *>(widget);

         if (spinbox) {

            if (!Args.Length ()) {

               result = v8::Number::New (spinbox->minimum ());
            }
            else {

               spinbox->setMinimum (v8_to_number (Args[0]));
            }
         }
         else {

            QDoubleSpinBox *dspinbox = qobject_cast<QDoubleSpinBox *>(widget);
            if (dspinbox) {

               if (!Args.Length ()) {

                  result = v8::Number::New (dspinbox->minimum ());
               }
               else {

                  dspinbox->setMinimum (v8_to_number (Args[0]));
               }
            }

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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QSpinBox *spinbox = qobject_cast<QSpinBox *>(widget);

         if (spinbox) {

            if (!Args.Length ()) {

               result = v8::Number::New (spinbox->value ());
            }
            else {

               spinbox->setValue (v8_to_number (Args[0]));
            }
         }
         else {

            QDoubleSpinBox *dspinbox = qobject_cast<QDoubleSpinBox *>(widget);
            if (dspinbox) {

               if (!Args.Length ()) {

                  result = v8::Number::New (dspinbox->value ());
               }
               else {

                  dspinbox->setValue (v8_to_number (Args[0]));
               }
            }

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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QSpinBox *spinbox = qobject_cast<QSpinBox *>(widget);

         if (spinbox) {

            if (!Args.Length ()) {
               self->_log.error << "spin: " << qPrintable(spinbox->text ()) << endl;
               result = v8::String::New (qPrintable(spinbox->text ()));
            }
         }
         else {

            QDoubleSpinBox *dspinbox = qobject_cast<QDoubleSpinBox *>(widget);
            if (dspinbox) {

               if (!Args.Length ()) {
                  self->_log.error << "dspin: " << qPrintable(dspinbox->text ()) << endl;
                  result = v8::String::New (qPrintable(dspinbox->text ()));
               }
            }
         }
      }
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
}
