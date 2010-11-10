#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
//#include "dmzV8QttableWidget.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QTableWidgetItem>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetSelectionRange>


using namespace dmz;


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_qtablewidgetitem (QTableWidgetItem *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_tableWidgetItemCtor.IsEmpty ()) {

         obj = _tableWidgetItemCtor->NewInstance ();
      }

      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;
      }
   }

   return scope.Close (result);
}


QTableWidgetItem *
dmz::JsModuleUiV8QtBasic::_to_qtablewidgetitem (V8Value value) {

   v8::HandleScope scope;
   QTableWidgetItem *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_tableWidgetItemTemp->HasInstance (obj)) {

         result = (QTableWidgetItem *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_qtablewidgetselectionrange (
   QTableWidgetSelectionRange *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_tableSelectionCtor.IsEmpty ()) { obj = _tableSelectionCtor->NewInstance (); }

      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;
      }
   }

   return scope.Close (result);
}


QTableWidgetSelectionRange *
dmz::JsModuleUiV8QtBasic::_to_qtablewidgetselectionrange (V8Value value) {

   v8::HandleScope scope;
   QTableWidgetSelectionRange *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_tableSelectionTemp->HasInstance (obj)) {

         result =
            (QTableWidgetSelectionRange *)
               v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         table->clear ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_clear_contents (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         table->clearContents ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_clear_spans (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         table->clearSpans ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_set_h_labels (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () && (Args[0]->IsArray ())) {

            QStringList list;
            V8Array array = v8_to_array (Args[0]);
            const int Length = array->Length ();
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = array->Get (v8::Integer::New (ix));
               list << v8_to_qstring (value);
            }
            table->setHorizontalHeaderLabels (list);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_set_v_labels (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () && (Args[0]->IsArray ())) {

            QStringList list;
            V8Array array = v8_to_array (Args[0]);
            const int Length = array->Length ();
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = array->Get (v8::Integer::New (ix));
               list << v8_to_qstring (value);
            }
            table->setVerticalHeaderLabels (list);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_col_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            if (Args.Length () > 1) {

               table->setColumnWidth (v8_to_uint32 (Args[0]), v8_to_uint32 (Args[1]));
            }
            result = v8::Number::New (table->columnWidth (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_col_hidden (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            if (Args.Length () > 1) {

               table->setColumnHidden (v8_to_uint32 (Args[0]), v8_to_boolean (Args[1]));
            }
            result = v8::Boolean::New (table->isColumnHidden (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_row_height (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            if (Args.Length () > 1) {

               table->setRowHeight (v8_to_uint32 (Args[0]), v8_to_uint32 (Args[1]));
            }
            result = v8::Number::New (table->rowHeight (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_row_hidden (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            if (Args.Length () > 1) {

               table->setRowHidden (v8_to_uint32 (Args[0]), v8_to_boolean (Args[1]));
            }
            result = v8::Boolean::New (table->isRowHidden (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_word_wrap (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->setWordWrap (v8_to_boolean (Args[0]));
         }
         result = v8::Boolean::New (table->wordWrap ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_col_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) { table->setColumnCount (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (table->columnCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_curr_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         result = v8::Number::New (table->currentColumn ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_row_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) { table->setRowCount (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (table->rowCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_curr_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         result = v8::Number::New (table->currentRow ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_curr_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            QTableWidgetItem *item = self->_to_qtablewidgetitem (Args[0]);
            if (item) { table->setCurrentItem (item); }
         }
         result = self->create_v8_qtablewidgetitem (table->currentItem ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_find_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            QString str (v8_to_qstring (Args[0]));
            Qt::MatchFlags flag = Qt::MatchExactly;
            if (Args.Length () > 1) {

               flag = (Qt::MatchFlags)v8_to_uint32 (Args[1]);
            }
            QList<QTableWidgetItem *> items = table->findItems (str, flag);
            const int Length = items.count ();
            V8Array array = v8::Array::New (Length);
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = self->create_v8_qtablewidgetitem (items.at (ix));
               array->Set (v8::Integer::New (ix), value);
            }
            result = array;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            QTableWidgetItem *item = self->_to_qtablewidgetitem (Args[0]);
            result = v8::Number::New (table->column (item));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         QTableWidgetItem *item = self->_to_qtablewidgetitem (Args[0]);
         result = v8::Number::New (table->row (item));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_cell_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 1) {

            int row = v8_to_uint32 (Args[0]);
            int col = v8_to_uint32 (Args[1]);
            if (Args.Length () > 2) {

               table->setCellWidget (row, col, self->_to_qwidget (Args[2]));
            }
            result = self->create_v8_qwidget (table->cellWidget (row, col));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_remove_cell_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 1) {

            int row = v8_to_uint32 (Args[0]);
            int col = v8_to_uint32 (Args[1]);
            table->removeCellWidget (row, col);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_selected_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         QList<QTableWidgetItem *> items = table->selectedItems ();
         const int Length = items.count ();
         V8Array array = v8::Array::New (Length);
         for (int ix = 0; ix < Length; ++ix) {

            V8Value value = self->create_v8_qtablewidgetitem (items.at (ix));
            array->Set (v8::Integer::New (ix), value);
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_selected_ranges (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         QList<QTableWidgetSelectionRange> items = table->selectedRanges ();
         const int Length = items.count ();
         V8Array array = v8::Array::New (Length);
         for (int ix = 0; !items.isEmpty (); ++ix) {

            QTableWidgetSelectionRange *range =
               new QTableWidgetSelectionRange(items.takeFirst ());
            V8Value value = self->create_v8_qtablewidgetselectionrange (range);
            array->Set (v8::Integer::New (ix), value);
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_sort_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 1) {

            table->sortItems (
               v8_to_uint32 (Args[0]),
               (Qt::SortOrder)v8_to_uint32 (Args[1]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_take_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 1) {

            int row = v8_to_uint32 (Args[0]);
            int col = v8_to_uint32 (Args[1]);
            QTableWidgetItem *item = table->takeItem (row, col);
            if (item) { result = self->create_v8_qtablewidgetitem (item); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_item_at (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 1) {

            int row = v8_to_uint32 (Args[0]);
            int col = v8_to_uint32 (Args[1]);
            if (Args.Length () > 2) {

               QTableWidgetItem *item = self->_to_qtablewidgetitem (Args[2]);
               if (item) { table->setItem (row, col, item); }
            }
            QTableWidgetItem *item = table->item (row, col);
            if (item) { result = self->create_v8_qtablewidgetitem (item); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_set_current_cell (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 1) {

            int row = v8_to_uint32 (Args[0]);
            int col = v8_to_uint32 (Args[1]);
            table->setCurrentCell (row, col);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_horiz_header (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            QTableWidgetItem *item = table->horizontalHeaderItem (v8_to_uint32 (Args[0]));
            result = self->create_v8_qtablewidgetitem (item);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_vert_header (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            QTableWidgetItem *item = table->verticalHeaderItem (v8_to_uint32 (Args[0]));
            result = self->create_v8_qtablewidgetitem (item);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_set_range_selected (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 1) {

            QTableWidgetSelectionRange *range =
               self->_to_qtablewidgetselectionrange (Args[0]);
            if (range) { table->setRangeSelected (*range, v8_to_boolean (Args[1])); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_insert_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->insertColumn (v8_to_uint32 (Args[0]));
         }
         else {

            table->insertColumn (table->columnCount ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_insert_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->insertRow (v8_to_uint32 (Args[0]));
         }
         else {

            table->insertRow (table->rowCount ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_remove_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->removeColumn (v8_to_uint32 (Args[0]));
         }
         else {

            table->removeColumn (table->columnCount () - 1);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_remove_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->removeRow (v8_to_uint32 (Args[0]));
         }
         else {

            table->removeRow (table->rowCount () - 1);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_set_span (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length () > 3) {

            int row = v8_to_uint32 (Args[0]);
            int col = v8_to_uint32 (Args[1]);
            int rowCount = v8_to_uint32 (Args[2]);
            int colCount = v8_to_uint32 (Args[3]);
            table->setSpan (row, col, rowCount, colCount);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_resize_single_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->resizeColumnToContents (v8_to_uint32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_resize_all_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) { table->resizeColumnsToContents (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_resize_single_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->resizeRowToContents (v8_to_uint32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_resize_all_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) { table->resizeRowsToContents (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_select_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->selectColumn (v8_to_uint32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_select_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidget *table = self->v8_to_qobject<QTableWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            table->selectRow (v8_to_uint32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_item_column (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetItem *item = self->_to_qtablewidgetitem (Args.This ());
      if (item) {

         result = v8::Number::New (item->column ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_item_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetItem *item = self->_to_qtablewidgetitem (Args.This ());
      if (item) {

         result = v8::Number::New (item->row ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_item_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetItem *item = self->_to_qtablewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QVariant variant = self->_qvariant_wrap_v8 (Args[0]);
            item->setData (Qt::UserRole, variant);
         }
         result = self->_qvariant_unwrap_v8 (item->data (Qt::UserRole));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_item_selected (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetItem *item = self->_to_qtablewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setSelected (v8_to_boolean (Args[0])); }
         result = v8::Boolean::New (item->isSelected ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_item_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetItem *item = self->_to_qtablewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setText (v8_to_qstring (Args[0])); }
         result = v8::String::New (qPrintable (item->text ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_range_bottom (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetSelectionRange *range =
         self->_to_qtablewidgetselectionrange (Args.This ());
      if (range) { result = v8::Number::New (range->bottomRow ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_range_top (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetSelectionRange *range =
         self->_to_qtablewidgetselectionrange (Args.This ());
      if (range) { result = v8::Number::New (range->topRow ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_range_left (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetSelectionRange *range =
         self->_to_qtablewidgetselectionrange (Args.This ());
      if (range) { result = v8::Number::New (range->leftColumn ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_range_right (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetSelectionRange *range =
         self->_to_qtablewidgetselectionrange (Args.This ());
      if (range) { result = v8::Number::New (range->rightColumn ()); }
   }

   return scope.Close (result);
}

dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_range_col_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetSelectionRange *range =
         self->_to_qtablewidgetselectionrange (Args.This ());
      if (range) { result = v8::Number::New (range->columnCount ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_table_range_row_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTableWidgetSelectionRange *range =
         self->_to_qtablewidgetselectionrange (Args.This ());
      if (range) { result = v8::Number::New (range->rowCount ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_table_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      int row = -1;
      int col = -1;
      if (Args.Length ()) {

         if (Args.Length () == 1) { parent = self->_to_qwidget (Args[0]); }
         if (Args.Length () >= 2) {

            row = v8_to_uint32 (Args[0]);
            col = v8_to_uint32 (Args[1]);
         }
         if (Args.Length () == 3) { parent = self->_to_qwidget (Args[2]); }
      }
      QTableWidget *widget = new QTableWidget (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_table_selection () {

   v8::HandleScope scope;

   _tableSelectionTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _tableSelectionTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _tableSelectionTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _tableSelectionTemp->PrototypeTemplate ();
   proto->Set ("bottomRow", v8::FunctionTemplate::New (_table_range_bottom, _self));
   proto->Set ("topRow", v8::FunctionTemplate::New (_table_range_top, _self));
   proto->Set ("leftColumn", v8::FunctionTemplate::New (_table_range_left, _self));
   proto->Set ("rightColumn", v8::FunctionTemplate::New (_table_range_right, _self));
   proto->Set ("columnCount", v8::FunctionTemplate::New (_table_range_col_count, _self));
   proto->Set ("rowCount", v8::FunctionTemplate::New (_table_range_row_count, _self));
}


void
dmz::JsModuleUiV8QtBasic::_init_table_widget_item () {

   v8::HandleScope scope;

   _tableWidgetItemTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _tableWidgetItemTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _tableWidgetItemTemp->PrototypeTemplate ();
   proto->Set ("column", v8::FunctionTemplate::New (_table_item_column, _self));
   proto->Set ("row", v8::FunctionTemplate::New (_table_item_row, _self));
   proto->Set ("data", v8::FunctionTemplate::New (_table_item_data, _self));
   proto->Set ("selected", v8::FunctionTemplate::New (_table_item_selected, _self));
   proto->Set ("text", v8::FunctionTemplate::New (_table_item_text, _self));
}


void
dmz::JsModuleUiV8QtBasic::_init_table_widget () {

   v8::HandleScope scope;

   _tableWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _tableWidgetTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _tableWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _tableWidgetTemp->PrototypeTemplate ();
   proto->Set ("clear", v8::FunctionTemplate::New (_table_clear, _self));
   proto->Set ("clearSpans", v8::FunctionTemplate::New (_table_clear_spans, _self));
   proto->Set (
      "clearContents",
      v8::FunctionTemplate::New (_table_clear_contents, _self));
   proto->Set ("setSpan", v8::FunctionTemplate::New (_table_set_span, _self));
   proto->Set ("columnWidth", v8::FunctionTemplate::New (_table_col_width, _self));
   proto->Set ("columnHidden", v8::FunctionTemplate::New (_table_col_hidden, _self));
   proto->Set ("insertColumn", v8::FunctionTemplate::New (_table_insert_col, _self));
   proto->Set ("removeColumn", v8::FunctionTemplate::New (_table_remove_col, _self));
   proto->Set ("selectColumn", v8::FunctionTemplate::New (_table_select_col, _self));
   proto->Set ("column", v8::FunctionTemplate::New (_table_col, _self));
   proto->Set ("columnCount", v8::FunctionTemplate::New (_table_col_count, _self));
   proto->Set ("currentColumn", v8::FunctionTemplate::New (_table_curr_col, _self));
   proto->Set ("rowHeight", v8::FunctionTemplate::New (_table_row_height, _self));
   proto->Set ("rowHidden", v8::FunctionTemplate::New (_table_row_hidden, _self));
   proto->Set ("insertRow", v8::FunctionTemplate::New (_table_insert_row, _self));
   proto->Set ("removeRow", v8::FunctionTemplate::New (_table_remove_row, _self));
   proto->Set ("selectRow", v8::FunctionTemplate::New (_table_select_row, _self));
   proto->Set ("row", v8::FunctionTemplate::New (_table_row, _self));
   proto->Set ("rowCount", v8::FunctionTemplate::New (_table_row_count, _self));
   proto->Set ("currentRow", v8::FunctionTemplate::New (_table_curr_row, _self));
   proto->Set ("wordWrap", v8::FunctionTemplate::New (_table_word_wrap, _self));
   proto->Set (
      "resizeColumnToContents",
      v8::FunctionTemplate::New (_table_resize_single_col, _self));
   proto->Set (
      "resizeColumnsToContents",
      v8::FunctionTemplate::New (_table_resize_all_col, _self));
   proto->Set (
      "resizeRowToContents",
      v8::FunctionTemplate::New (_table_resize_single_row, _self));
   proto->Set (
      "resizeRowsToContents",
      v8::FunctionTemplate::New (_table_resize_all_row, _self));
   proto->Set ("cellWidget", v8::FunctionTemplate::New (_table_cell_widget, _self));
   proto->Set (
      "removeCellWidget",
      v8::FunctionTemplate::New (_table_remove_cell_widget, _self));
   proto->Set ("item", v8::FunctionTemplate::New (_table_item_at, _self));
   proto->Set ("findItems", v8::FunctionTemplate::New (_table_find_items, _self));
   proto->Set ("currentItem", v8::FunctionTemplate::New (_table_curr_item, _self));
   proto->Set (
      "selectedItems",
      v8::FunctionTemplate::New (_table_selected_items, _self));
   proto->Set (
      "selectedRanges",
      v8::FunctionTemplate::New (_table_selected_ranges, _self));
   proto->Set (
      "setCurrentCell",
      v8::FunctionTemplate::New (_table_set_current_cell, _self));
   proto->Set (
      "setHorizontalLabels",
      v8::FunctionTemplate::New (_table_set_h_labels, _self));
   proto->Set (
      "setVerticalLabels",
      v8::FunctionTemplate::New (_table_set_v_labels, _self));
   proto->Set (
      "setRangeSelected",
      v8::FunctionTemplate::New (_table_set_range_selected, _self));
   proto->Set ("sortItems", v8::FunctionTemplate::New (_table_sort_items, _self));
   proto->Set ("takeItem", v8::FunctionTemplate::New (_table_take_item, _self));
   proto->Set (
      "horizontalHeaderItem",
      v8::FunctionTemplate::New (_table_horiz_header, _self));
   proto->Set (
      "verticalHeaderItem",
      v8::FunctionTemplate::New (_table_vert_header, _self));

   _tableApi.add_function ("create", _create_table_widget, _self);
}
