#include <dmzJsModuleV8.h>
#include "dmzV8QtGroupBox.h"
#include <QtGui/QWidget>


namespace {

   static const dmz::String LocalSignalClicked ("clicked");
   static const dmz::String ToggledSignal ("toggled");
};


dmz::V8QtGroupBox::V8QtGroupBox (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtGroupBox::~V8QtGroupBox () {;}


dmz::Boolean
dmz::V8QtGroupBox::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalClicked) {

         connect (
            _widget,
            SIGNAL (clicked ()),
            SLOT (on_clicked ()),
            Qt::UniqueConnection);

         results = True;
      }

      else if (Signal == ToggledSignal) {

         connect (
            _widget,
            SIGNAL (toggled (bool)),
            SLOT (on_toggle (bool)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtGroupBox::on_clicked () {

   _do_callback (LocalSignalClicked);
}


void
dmz::V8QtGroupBox::on_toggle (bool checked) {

   _do_callback (ToggledSignal, checked);
}
