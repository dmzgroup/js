#include <dmzJsModuleV8.h>
#include "dmzV8QtMessageBox.h"
#include <QtGui/QWidget>
#include <QtGui/QAbstractButton>


namespace {

   static const dmz::String LocalSignalButtonClicked ("buttonClicked");
};


dmz::V8QtMessageBox::V8QtMessageBox (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtDialog (Self, widget, state) {;}


dmz::V8QtMessageBox::~V8QtMessageBox () {;}


dmz::Boolean
dmz::V8QtMessageBox::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalButtonClicked) {

         connect (
            _widget,
            SIGNAL (buttonClicked (QAbstractButton *)),
            SLOT (on_buttonClicked (QAbstractButton *)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }
   else { results = V8QtDialog::bind (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtMessageBox::on_buttonClicked (QAbstractButton *button) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (_state->ui->create_v8_qwidget (button));
      _do_callback (LocalSignalButtonClicked, args);
   }
}
