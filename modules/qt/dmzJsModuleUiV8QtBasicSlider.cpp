#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QAbstractSlider>
#include <QtGui/QDial>
#include <QtGui/QSlider>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_slider_maximum (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAbstractSlider *slider = self->v8_to_qobject<QAbstractSlider>(Args.This ());
      if (slider) {

         if (Args.Length ()) {

            slider->setMaximum (v8_to_number(Args[0]));
         }
         else {

            result = v8::Number::New (slider->maximum ());
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

      QAbstractSlider *slider = self->v8_to_qobject<QAbstractSlider>(Args.This ());
      if (slider) {

         if (Args.Length ()) {

            slider->setMinimum (v8_to_number(Args[0]));
         }
         else {

            result = v8::Number::New (slider->minimum ());
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

      QAbstractSlider *slider = self->v8_to_qobject<QAbstractSlider>(Args.This ());
      if (slider) {

         if (Args.Length ()) {

            slider->setSliderPosition (v8_to_number(Args[0]));
         }
         else {

            result = v8::Number::New (slider->value ());
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

      QAbstractSlider *slider = self->v8_to_qobject<QAbstractSlider>(Args.This ());
      if (slider) {

         result = v8::Boolean::New (slider->isSliderDown ());
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

      QDial *dial = self->v8_to_qobject<QDial>(Args.This ());
      if (dial) {

         if (Args.Length ()) {

            dial->setWrapping (v8_to_boolean (Args[0]));
         }

         result = v8::Boolean::New (dial->wrapping ());
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

      QDial *dial = self->v8_to_qobject<QDial>(Args.This ());
      if (dial) {

         if (Args.Length ()) {

            dial->setNotchesVisible (v8_to_boolean (Args[0]));
         }

         result = v8::Boolean::New (dial->notchesVisible ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_slider (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      QSlider *widget = 0;
      Qt::Orientation ori = Qt::Vertical;
      if (Args.Length ()) {

         if (Args[0]->IsInt32 ()) {

            ori = (Qt::Orientation)v8_to_uint32 (Args[0]);
            if (Args.Length () > 1) { parent = self->_to_qwidget (Args[1]); }
         }
         else { parent = self->_to_qwidget (Args[0]); }
      }
      widget = new QSlider (ori, parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_dial (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QDial *widget = new QDial (parent);
      result = self->create_v8_qobject (widget);
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

   _sliderApi.add_function ("create", _create_slider, _self);
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

   _dialApi.add_function ("create", _create_dial, _self);
}
