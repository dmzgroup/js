#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtTableWidget.h"
#include <QtGui/QTableWidget>


namespace {

   static const dmz::String LocalSignalCellClicked ("cellClicked");
   static const dmz::String LocalSignalCellDoubleClicked ("cellDoubleClicked");
   static const dmz::String LocalSignalCurrentItemChanged ("currentItemChanged");
   static const dmz::String LocalSignalItemSelectionChanged ("itemSelectionChanged");
};


dmz::V8QtTableWidget::V8QtTableWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtTableWidget::~V8QtTableWidget () {;}


dmz::Boolean
dmz::V8QtTableWidget::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalCurrentItemChanged) {

         connect (
            _widget,
            SIGNAL (currentItemChanged (QTableWidgetItem *, QTableWidgetItem *)),
            SLOT (on_currentItemChanged (QTableWidgetItem *, QTableWidgetItem *)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalCellClicked) {

         connect (
            _widget,
            SIGNAL (cellClicked (int, int)),
            SLOT (on_cellClicked (int, int)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalItemSelectionChanged) {

         connect (
            _widget,
            SIGNAL (itemSelectionChanged ()),
            SLOT (on_itemSelectionChanged ()),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalCellDoubleClicked) {

         connect (
            _widget,
            SIGNAL (cellDoubleClicked (int, int)),
            SLOT (on_cellDoubleClicked (int, int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtTableWidget::on_cellClicked (int row, int col) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (v8::Number::New (row));
      args.append (v8::Number::New (col));

      _do_callback (LocalSignalCellClicked, args);
   }
}


void
dmz::V8QtTableWidget::on_cellDoubleClicked (int row, int col) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (v8::Number::New (row));
      args.append (v8::Number::New (col));

      _do_callback (LocalSignalCellDoubleClicked, args);
   }
}


void
dmz::V8QtTableWidget::on_currentItemChanged (
      QTableWidgetItem *current,
      QTableWidgetItem *previous) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (_state->ui->create_v8_qtablewidgetitem (current));
      args.append (_state->ui->create_v8_qtablewidgetitem (previous));

      _do_callback (LocalSignalCurrentItemChanged, args);
   }
}


void
dmz::V8QtTableWidget::on_itemSelectionChanged () {

   _do_callback (LocalSignalItemSelectionChanged);
}
