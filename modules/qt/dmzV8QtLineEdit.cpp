#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtLineEdit.h"
#include <QtGui/QLineEdit>
#include <QtCore/QDebug>


namespace {

   static const dmz::String TextChanged ("textChanged");
};


dmz::V8QtLineEdit::V8QtLineEdit (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtLineEdit::~V8QtLineEdit () {;}


dmz::Boolean
dmz::V8QtLineEdit::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == TextChanged) {

         connect (
            _widget,
            SIGNAL (textChanged (const QString &)),
            SLOT (on_textChanged (const QString &)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtLineEdit::on_textChanged (const QString &Val) {

   _do_callback (TextChanged, Val);
}

