#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QAbstractSlider>
#include <QtGui/QDial>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_slider_maximum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QAbstractSlider *slider = qobject_cast<QAbstractSlider *>(widget);

         if (slider) {

            if (Args.Length ()) {

               slider->setMaximum (v8_to_number(Args[0]));
            }
            else {

               result = v8::Number::New (slider->maximum ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_slider_minimum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QAbstractSlider *slider = qobject_cast<QAbstractSlider *>(widget);

         if (slider) {

            if (Args.Length ()) {

               slider->setMinimum (v8_to_number(Args[0]));
            }
            else {

               result = v8::Number::New (slider->minimum ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_slider_value (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QAbstractSlider *slider = qobject_cast<QAbstractSlider *>(widget);

         if (slider) {

            if (Args.Length ()) {

               slider->setSliderPosition (v8_to_number(Args[0]));
            }
            else {

               result = v8::Number::New (slider->value ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_slider_is_down (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QAbstractSlider *slider = qobject_cast<QAbstractSlider *>(widget);

         if (slider) {

            result = v8::Boolean::New (slider->isSliderDown ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dial_wrapping (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QDial *dial = qobject_cast<QDial *>(widget);

         if (dial) {

            if (Args.Length ()) {

               dial->setWrapping (v8_to_boolean (Args[0]));
            }
            else {

               result = v8::Boolean::New (dial->wrapping ());
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_dial_notches_visible (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QDial *dial = qobject_cast<QDial *>(widget);

         if (dial) {

            if (Args.Length ()) {

               dial->setNotchesVisible (v8_to_boolean (Args[0]));
            }
            else {

               result = v8::Boolean::New (dial->notchesVisible ());
            }
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_slider () {

   v8::HandleScope scope;

   _sliderTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _sliderTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _sliderTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _sliderTemp->PrototypeTemplate ();
   proto->Set ("value", v8::FunctionTemplate::New (_slider_value, _self));
   proto->Set ("maximum", v8::FunctionTemplate::New (_slider_maximum, _self));
   proto->Set ("minimum", v8::FunctionTemplate::New (_slider_minimum, _self));
   proto->Set ("isDown", v8::FunctionTemplate::New (_slider_is_down, _self));
}


void
dmz::JsModuleUiV8QtBasic::_init_dial () {

   v8::HandleScope scope;

   _dialTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _dialTemp->Inherit (_sliderTemp);

   V8ObjectTemplate instance = _dialTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _dialTemp->PrototypeTemplate ();
   proto->Set ("wrapping", v8::FunctionTemplate::New (_dial_wrapping, _self));
   proto->Set (
      "notchesVisible",
      v8::FunctionTemplate::New (_dial_notches_visible, _self));
}
