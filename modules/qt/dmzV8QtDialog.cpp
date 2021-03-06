#include <dmzJsModuleV8.h>
#include "dmzV8QtDialog.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QDialog>
#include <QtGui/QInputDialog>


namespace {

   static const dmz::String LocalSignalFinished ("finished");
};


dmz::V8QtDialog::V8QtDialog (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtDialog::~V8QtDialog () {;}


dmz::Boolean
dmz::V8QtDialog::bind (
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
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtDialog::open (
      const V8Object &Self,
      const V8Function &Func) {

   QDialog *dialog = qobject_cast<QDialog *>(_widget);
   if (dialog) {

      if (bind (LocalSignalFinished, Self, Func)) {

         _make_weak (False);
         dialog->open ();
      }
   }
}


void
dmz::V8QtDialog::clean_up () {

   V8QtWidget::clean_up ();

   // remove the widget's parent so it will get deleted in the dtor
   if (_widget) { _widget->setParent (0); }
}


void
dmz::V8QtDialog::on_finished (int value) {

   _do_callback (LocalSignalFinished, value);
   _make_weak (True);
}
