#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QAbstractSlider>


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
