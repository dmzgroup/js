#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtTextEdit.h"
#include <QtGui/QTextEdit>
#include <QtCore/QDebug>


namespace {

   static const dmz::String TextChanged ("textChanged");
};


dmz::V8QtTextEdit::V8QtTextEdit (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtTextEdit::~V8QtTextEdit () {;}


dmz::Boolean
dmz::V8QtTextEdit::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == TextChanged) {

         connect (
            _widget,
            SIGNAL (textChanged ()),
            SLOT (on_textChanged ()),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtTextEdit::on_textChanged () {

   _do_callback (TextChanged);
}
