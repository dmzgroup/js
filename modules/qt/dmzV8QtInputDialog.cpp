#include <dmzJsModuleV8.h>
#include "dmzV8QtInputDialog.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QDialog>
#include <QtGui/QInputDialog>


namespace {

   static const dmz::String LocalSignalFinished ("finished");
   static const dmz::String LocalSignalDValChanged ("doubleValueChanged");
   static const dmz::String LocalSignalDValSelected ("doubleValueSelected");
   static const dmz::String LocalSignalIValChanged ("intValueChanged");
   static const dmz::String LocalSignalIValSelected ("intValueSelected");
   static const dmz::String LocalSignalTValChanged ("textValueChanged");
   static const dmz::String LocalSignalTValSelected ("textValueSelected");
};


dmz::V8QtInputDialog::V8QtInputDialog (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtDialog (Self, widget, state) {;}


dmz::V8QtInputDialog::~V8QtInputDialog () {;}


dmz::Boolean
dmz::V8QtInputDialog::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalFinished) {

         connect (
            _widget,
            SIGNAL (finished (int)),
            SLOT (on_finished (int)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalDValChanged) {

         connect (
            _widget,
            SIGNAL (doubleValueChanged (double)),
            SLOT (on_doubleValueSelected (double)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalDValSelected) {

         connect (
            _widget,
            SIGNAL (doubleValueSelected (double)),
            SLOT (on_doubleValueSelected (double)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalIValChanged) {

         connect (
            _widget,
            SIGNAL (intValueChanged (int)),
            SLOT (on_intValueChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalIValSelected) {

         connect (
            _widget,
            SIGNAL (intValueSelected (int)),
            SLOT (on_intValueSelected (int)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalTValChanged) {

         connect (
            _widget,
            SIGNAL (textValueChanged (const QString &)),
            SLOT (on_textValueChanged (const QString &)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalTValSelected) {

         connect (
            _widget,
            SIGNAL (textValueSelected (const QString &)),
            SLOT (on_textValueSelected (const QString &)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtInputDialog::on_finished (int value) {

   if (_state) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (v8::Integer::New (value));

      QInputDialog *input = qobject_cast<QInputDialog *>(_widget);
      if (input) {

         if (value) {

            if (input->inputMode () == QInputDialog::IntInput) {

               args.append (v8::Integer::New (input->intValue ()));
            }
            else if (input->inputMode () == QInputDialog::DoubleInput) {

               args.append (v8::Number::New (input->doubleValue ()));
            }
            else {

               args.append (qstring_to_v8 (input->textValue ()));
            }
         }
         else {

            args.append (v8::Undefined ());
         }
      }

      _do_callback (LocalSignalFinished, args);
      _make_weak (True);
   }
}


void
dmz::V8QtInputDialog::on_doubleValueChanged (double value) {

   _do_callback (LocalSignalDValChanged, value);
}


void
dmz::V8QtInputDialog::on_doubleValueSelected (double value) {

   _do_callback (LocalSignalDValSelected, value);
}


void
dmz::V8QtInputDialog::on_intValueChanged (int value) {

   _do_callback (LocalSignalIValChanged, value);
}



void
dmz::V8QtInputDialog::on_intValueSelected (int value) {

   _do_callback (LocalSignalIValSelected, value);
}



void
dmz::V8QtInputDialog::on_textValueChanged (const QString &value) {

   _do_callback (LocalSignalTValChanged, value);
}



void
dmz::V8QtInputDialog::on_textValueSelected (const QString &value) {

   _do_callback (LocalSignalTValSelected, value);
}


