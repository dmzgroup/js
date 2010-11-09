#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtTreeWidget.h"
#include <QtGui/QListWidget>


namespace {

   static const dmz::String LocalSignalCurrentItemChanged ("currentItemChanged");
   static const dmz::String LocalSignalCurrentRowChanged ("currentRowChanged");
   static const dmz::String LocalSignalCurrentTextChanged ("currentTextChanged");
   static const dmz::String LocalSignalItemActivated ("itemActivated");
   static const dmz::String LocalSignalItemExpanded ("itemExpanded");
   static const dmz::String LocalSignalItemSelectionChanged ("itemSelectionChanged");
};


dmz::V8QtTreeWidget::V8QtTreeWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtTreeWidget::~V8QtTreeWidget () {;}


dmz::Boolean
dmz::V8QtTreeWidget::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalCurrentItemChanged) {

         connect (
            _widget,
            SIGNAL (currentItemChanged (QTreeWidgetItem *, QTreeWidgetItem *)),
            SLOT (on_currentItemChanged (QTreeWidgetItem *, QTreeWidgetItem *)),
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
            SIGNAL (itemActivated (QTreeWidgetItem *)),
            SLOT (on_itemActivated (QTreeWidgetItem *)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalItemExpanded) {

         connect (
            _widget,
            SIGNAL (itemExpanded (QTreeWidgetItem *)),
            SLOT (on_itemExpanded (QTreeWidgetItem *)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalItemSelectionChanged) {

         connect (
            _widget,
            SIGNAL (itemSelectionChanged (void)),
            SLOT (on_itemSelectionChanged (void)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtTreeWidget::on_currentItemChanged (
      QTreeWidgetItem *current,
      QTreeWidgetItem *previous) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (_state->ui->create_v8_qtreewidgetitem (current));
      args.append (_state->ui->create_v8_qtreewidgetitem (previous));

      _do_callback (LocalSignalCurrentItemChanged, args);
   }
}


void
dmz::V8QtTreeWidget::on_currentRowChanged (int value) {

   _do_callback (LocalSignalCurrentRowChanged, value);
}


void
dmz::V8QtTreeWidget::on_currentTextChanged (const QString &Value) {

   _do_callback (LocalSignalCurrentTextChanged, Value);
}


void
dmz::V8QtTreeWidget::on_itemActivated (QTreeWidgetItem *item) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (_state->ui->create_v8_qtreewidgetitem (item));

      _do_callback (LocalSignalItemActivated, args);
   }
}

void
dmz::V8QtTreeWidget::on_itemExpanded (QTreeWidgetItem *item) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (_state->ui->create_v8_qtreewidgetitem (item));

      _do_callback (LocalSignalItemExpanded, args);
   }
}

void
dmz::V8QtTreeWidget::on_itemSelectionChanged () {

   _do_callback (LocalSignalItemSelectionChanged);
}
