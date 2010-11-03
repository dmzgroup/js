#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtToolBox.h"
#include <QtGui/QToolBox>


namespace {

   static const dmz::String CurrentChanged ("currentChanged");

};


dmz::V8QtToolBox::V8QtToolBox (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtToolBox::~V8QtToolBox () {;}


dmz::Boolean
dmz::V8QtToolBox::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == CurrentChanged) {

         connect (
            _widget,
            SIGNAL (currentChanged (int)),
            SLOT (on_currentChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtToolBox::on_currentChanged (int val) {

   _do_callback (CurrentChanged, val);
}

