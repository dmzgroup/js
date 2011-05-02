#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtComboBox.h"
#include <QtGui/QComboBox>


namespace {

   static const dmz::String CurrentIndexChanged ("currentIndexChanged");
   static const dmz::String CurrentTextChanged ("currentTextChanged");
};


dmz::V8QtComboBox::V8QtComboBox (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtComboBox::~V8QtComboBox () {;}


dmz::Boolean
dmz::V8QtComboBox::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == CurrentIndexChanged) {

         connect (
            _widget,
            SIGNAL (currentIndexChanged (int)),
            SLOT (on_currentIndexChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == CurrentTextChanged) {

         connect (
            _widget,
            SIGNAL (currentIndexChanged (const QString &)),
            SLOT (on_currentIndexChanged (const QString &)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtComboBox::on_currentIndexChanged (int val) {

   _do_callback (CurrentIndexChanged, val);
}


void
dmz::V8QtComboBox::on_currentIndexChanged (const QString &val) {

   _do_callback (CurrentTextChanged, val);
}
