#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtListWidget.h"
#include <QtGui/QListWidget>


namespace {

   static const dmz::String LocalSignalCurrentItemChanged ("currentItemChanged");
   static const dmz::String LocalSignalCurrentRowChanged ("currentRowChanged");
   static const dmz::String LocalSignalCurrentTextChanged ("currentTextChanged");
   static const dmz::String LocalSignalItemActivated ("itemActivated");
};


dmz::V8QtListWidget::V8QtListWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtListWidget::~V8QtListWidget () {;}


dmz::Boolean
dmz::V8QtListWidget::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalCurrentItemChanged) {

         connect (
            _widget,
            SIGNAL (currentItemChanged (QListWidgetItem *, QListWidgetItem *)),
            SLOT (on_currentItemChanged (QListWidgetItem *, QListWidgetItem *)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalCurrentRowChanged) {

         connect (
            _widget,
            SIGNAL (currentRowChanged (int)),
            SLOT (on_currentRowChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalCurrentTextChanged) {

         connect (
            _widget,
            SIGNAL (currentTextChanged (const QString &)),
            SLOT (on_currentTextChanged (const QString &)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalItemActivated) {

         connect (
            _widget,
            SIGNAL (itemActivated (QListWidgetItem *)),
            SLOT (on_itemActivated (QListWidgetItem *)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtListWidget::on_currentItemChanged (
      QListWidgetItem *current,
      QListWidgetItem *previous) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (_state->ui->create_v8_qlistwidgetitem (current));
      args.append (_state->ui->create_v8_qlistwidgetitem (previous));

      _do_callback (LocalSignalCurrentItemChanged, args);
   }
}


void
dmz::V8QtListWidget::on_currentRowChanged (int value) {

   _do_callback (LocalSignalCurrentRowChanged, value);
}


void
dmz::V8QtListWidget::on_currentTextChanged (const QString &Value) {

   _do_callback (LocalSignalCurrentTextChanged, Value);
}


void
dmz::V8QtListWidget::on_itemActivated (QListWidgetItem *item) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (_state->ui->create_v8_qlistwidgetitem (item));

      _do_callback (LocalSignalItemActivated, args);
   }
}
