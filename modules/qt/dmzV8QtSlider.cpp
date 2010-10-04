#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtSlider.h"
#include <QtGui/QSlider>
#include <QtCore/QDebug>


namespace {

   static const dmz::String SliderPressed ("sliderPressed");
   static const dmz::String SliderReleased ("sliderReleased");
   static const dmz::String ValueChanged ("valueChanged");
};


dmz::V8QtSlider::V8QtSlider (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtSlider::~V8QtSlider () {;}


dmz::Boolean
dmz::V8QtSlider::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == SliderPressed) {

         connect (
            _widget,
            SIGNAL (sliderPressed ()),
            SLOT (on_sliderPressed ()),
            Qt::UniqueConnection);

         results = True;
      }

      if (Signal == SliderReleased) {

         connect (
            _widget,
            SIGNAL (sliderReleased ()),
            SLOT (on_sliderReleased ()),
            Qt::UniqueConnection);

         results = True;
      }

      if (Signal == ValueChanged) {

         connect (
            _widget,
            SIGNAL (valueChanged (int)),
            SLOT (on_valueChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}

void
dmz::V8QtSlider::on_sliderPressed () {

   _do_callback (SliderPressed);
}


void
dmz::V8QtSlider::on_sliderReleased () {

   _do_callback (SliderReleased);
}


void
dmz::V8QtSlider::on_valueChanged (int val) {

   _do_callback (ValueChanged, val);
}

