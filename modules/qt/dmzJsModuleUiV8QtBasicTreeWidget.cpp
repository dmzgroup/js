#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
//#include "dmzV8QtTreeWidget.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QTreeWidgetItem>


using namespace dmz;


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_qtreewidgetitem (QTreeWidgetItem *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_treeWidgetItemCtor.IsEmpty ()) { obj = _treeWidgetItemCtor->NewInstance (); }

      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;
      }
   }

   return scope.Close (result);
}


QTreeWidgetItem *
dmz::JsModuleUiV8QtBasic::_to_qtreewidgetitem (V8Value value) {

   v8::HandleScope scope;
   QTreeWidgetItem *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_treeWidgetItemTemp->HasInstance (obj)) {

         result = (QTreeWidgetItem *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         tree->clear ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_header_labels (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         QTreeWidgetItem *item = tree->headerItem ();

         if (Args.Length () && (Args[0]->IsArray ())) {

            V8Array array = v8_to_array (Args[0]);
            const int Length = array->Length ();
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = array->Get (v8::Integer::New (ix));
               item->setText (ix, v8_to_qstring (value));
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_col_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            if (Args.Length () > 1) {

               tree->setColumnWidth (v8_to_uint32 (Args[0]), v8_to_uint32 (Args[1]));
            }
            result = v8::Number::New (tree->columnWidth (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_col_hidden (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            if (Args.Length () > 1) {

               tree->setColumnHidden (v8_to_uint32 (Args[0]), v8_to_boolean (Args[1]));
            }
            result = v8::Boolean::New (tree->isColumnHidden (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_word_wrap (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            tree->setWordWrap (v8_to_boolean (Args[0]));
         }
         result = v8::Boolean::New (tree->wordWrap ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_add (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            QTreeWidgetItem *item = 0;
            if (Args[0]->IsArray ()) {

               QStringList list;
               V8Array array = v8_to_array (Args[0]);
               const int Length = array->Length ();
               for (int ix = 0; ix < Length; ++ix) {

                  V8Value value = array->Get (v8::Integer::New (ix));
                  list << v8_to_qstring (value);
               }
               item = new QTreeWidgetItem (tree, list);
            }
            else {

               QTreeWidgetItem *prev = self->_to_qtreewidgetitem (Args[0]);
               if (prev) {

                  item = new QTreeWidgetItem (tree, prev);
               }
               else {

                  item = new QTreeWidgetItem (tree);
               }
            }

            if ((Args.Length () > 2) && item) {

               QVariant variant = self->_qvariant_wrap_v8 (Args[1]);
               int col = v8_to_uint32 (Args[2]);
               item->setData (col, Qt::UserRole, variant);
            }

            if (item) {

               result = self->create_v8_qtreewidgetitem (item);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_col_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) { tree->setColumnCount (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (tree->columnCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_curr_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         result = v8::Number::New (tree->currentColumn ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_curr_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args[0]);
            if (item) { tree->setCurrentItem (item); }
         }
         result = self->create_v8_qtreewidgetitem (tree->currentItem ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_find_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            QString str (v8_to_qstring (Args[0]));
            Qt::MatchFlags flag = Qt::MatchExactly;
            if (Args.Length () > 1) {

               flag = (Qt::MatchFlags)v8_to_uint32 (Args[1]);
            }
            QList<QTreeWidgetItem *> items = tree->findItems (str, flag);
            const int Length = items.count ();
            V8Array array = v8::Array::New (Length);
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = self->create_v8_qtreewidgetitem (items.at (ix));
               array->Set (v8::Integer::New (ix), value);
            }
            result = array;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_above (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args[0]);
            result = self->create_v8_qtreewidgetitem (tree->itemAbove (item));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_below (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args[0]);
         result = self->create_v8_qtreewidgetitem (tree->itemBelow (item));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length () > 1) {

            QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args[0]);
            int col = v8_to_uint32 (Args[1]);
            if (item) {
               if (Args.Length () > 2) {

                  tree->setItemWidget (item, col, self->_to_qwidget (Args[2]));
               }

               result = self->create_v8_qwidget (tree->itemWidget (item, col));
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_rem_item_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length () > 1) {

            QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args[0]);
            if (item) { tree->removeItemWidget (item, v8_to_int32 (Args[1])); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_selected_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         QList<QTreeWidgetItem *> items = tree->selectedItems ();
         const int Length = items.count ();
         V8Array array = v8::Array::New (Length);
         for (int ix = 0; ix < Length; ++ix) {

            V8Value value = self->create_v8_qtreewidgetitem (items.at (ix));
            array->Set (v8::Integer::New (ix), value);
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_sort_col (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length () > 1) {

            tree->sortItems (
               v8_to_uint32 (Args[0]),
               (Qt::SortOrder)v8_to_uint32 (Args[1]));
         }
         result = v8::Number::New (tree->sortColumn ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_take_item_at (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         if (Args.Length ()) {

            QTreeWidgetItem *item = tree->takeTopLevelItem (v8_to_uint32 (Args[0]));
            if (item) { result = self->create_v8_qtreewidgetitem (item); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_at (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         QTreeWidgetItem *item = tree->topLevelItem (v8_to_uint32 (Args[0]));
         if (item) { result = self->create_v8_qtreewidgetitem (item); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidget *tree = self->v8_to_qobject<QTreeWidget> (Args.This ());
      if (tree) {

         result = v8::Number::New (tree->topLevelItemCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_add (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QTreeWidgetItem *child = 0;
            if (Args[0]->IsArray ()) {

               QStringList list;
               V8Array array = v8_to_array (Args[0]);
               const int Length = array->Length ();
               for (int ix = 0; ix < Length; ++ix) {

                  V8Value value = array->Get (v8::Integer::New (ix));
                  list << v8_to_qstring (value);
               }
               child = new QTreeWidgetItem (item, list);
            }
            else {

               QTreeWidgetItem *prev = self->_to_qtreewidgetitem (Args[0]);
               if (prev) {

                  child = new QTreeWidgetItem (item, prev);
               }
               else {

                  child = new QTreeWidgetItem (item);
               }
            }

            if ((Args.Length () > 2) && child) {

               QVariant variant = self->_qvariant_wrap_v8 (Args[1]);
               int col = v8_to_uint32 (Args[2]);
               child->setData (col, Qt::UserRole, variant);
            }

            if (child) {

               result = self->create_v8_qtreewidgetitem (child);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_child (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            result = self->create_v8_qtreewidgetitem (
               item->child (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_child_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         result = v8::Number::New (item->childCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_col_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         result = v8::Number::New (item->columnCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            int col = v8_to_uint32 (Args[0]);

            if (Args.Length () > 1) {

               QVariant variant = self->_qvariant_wrap_v8 (Args[1]);
               item->setData (col, Qt::UserRole, variant);
            }
            result = self->_qvariant_unwrap_v8 (item->data (col, Qt::UserRole));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_parent (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         result = self->create_v8_qtreewidgetitem (item->parent ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_hidden (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            item->setHidden (v8_to_boolean (Args[0]));
         }
         result = v8::Boolean::New (item->isHidden ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            int col = v8_to_uint32 (Args[0]);
            if (Args.Length () > 1) {

               item->setText (col, v8_to_qstring (Args[1]));
            }
            result = v8::String::New (qPrintable (item->text (col)));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_sort_children (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         if (Args.Length () > 1) {

            item->sortChildren (
               v8_to_uint32 (Args[0]),
               (Qt::SortOrder)v8_to_uint32 (Args[1]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_take_child (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            result = self->create_v8_qtreewidgetitem (
            item->takeChild (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_take_children (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         QList<QTreeWidgetItem *> items = item->takeChildren ();
         const int Length = items.count ();
         V8Array array = v8::Array::New (Length);
         for (int ix = 0; ix < Length; ++ix) {

            V8Value value = self->create_v8_qtreewidgetitem (items.at (ix));
            array->Set (v8::Integer::New (ix), value);
         }
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_tree_item_tree_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTreeWidgetItem *item = self->_to_qtreewidgetitem (Args.This ());
      if (item) {

         QTreeWidget *tree = item->treeWidget ();
         result = self->create_v8_qwidget (tree);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_tree_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QTreeWidget *widget = new QTreeWidget (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_tree_widget_item () {

   v8::HandleScope scope;

   _treeWidgetItemTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _treeWidgetItemTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _treeWidgetItemTemp->PrototypeTemplate ();
   proto->Set ("add", v8::FunctionTemplate::New (_tree_item_add, _self));
   proto->Set ("child", v8::FunctionTemplate::New (_tree_item_child, _self));
   proto->Set ("childCount", v8::FunctionTemplate::New (_tree_item_child_count, _self));
   proto->Set ("columnCount", v8::FunctionTemplate::New (_tree_item_col_count, _self));
   proto->Set ("data", v8::FunctionTemplate::New (_tree_item_data, _self));
   proto->Set ("parent", v8::FunctionTemplate::New (_tree_item_parent, _self));
   proto->Set ("hidden", v8::FunctionTemplate::New (_tree_item_hidden, _self));
   proto->Set ("text", v8::FunctionTemplate::New (_tree_item_text, _self));
   proto->Set (
      "sortChildren",
      v8::FunctionTemplate::New (_tree_item_sort_children, _self));
   proto->Set ("takeChild", v8::FunctionTemplate::New (_tree_item_take_child, _self));
   proto->Set (
      "takeChildren",
      v8::FunctionTemplate::New (_tree_item_take_children, _self));
   proto->Set ("treeWidget", v8::FunctionTemplate::New (_tree_item_tree_widget, _self));
}


void
dmz::JsModuleUiV8QtBasic::_init_tree_widget () {

   v8::HandleScope scope;

   _treeWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _treeWidgetTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _treeWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _treeWidgetTemp->PrototypeTemplate ();
   proto->Set ("clear", v8::FunctionTemplate::New (_tree_clear, _self));
   proto->Set ("headerLabels", v8::FunctionTemplate::New (_tree_header_labels, _self));
   proto->Set ("columnWidth", v8::FunctionTemplate::New (_tree_col_width, _self));
   proto->Set ("columnHidden", v8::FunctionTemplate::New (_tree_col_hidden, _self));
   proto->Set ("wordWrap", v8::FunctionTemplate::New (_tree_word_wrap, _self));
   proto->Set ("add", v8::FunctionTemplate::New (_tree_add, _self));
   proto->Set ("columnCount", v8::FunctionTemplate::New (_tree_col_count, _self));
   proto->Set ("currentColumn", v8::FunctionTemplate::New (_tree_curr_col, _self));
   proto->Set ("currentItem", v8::FunctionTemplate::New (_tree_curr_item, _self));
   proto->Set ("findItems", v8::FunctionTemplate::New (_tree_find_items, _self));
   proto->Set ("itemAbove", v8::FunctionTemplate::New (_tree_item_above, _self));
   proto->Set ("itemBelow", v8::FunctionTemplate::New (_tree_item_below, _self));
   proto->Set ("itemWidget", v8::FunctionTemplate::New (_tree_item_widget, _self));
   proto->Set (
      "removeItemWidget",
      v8::FunctionTemplate::New (_tree_rem_item_widget, _self));
   proto->Set ("selectedItems", v8::FunctionTemplate::New (_tree_selected_items, _self));
   proto->Set ("sortColumn", v8::FunctionTemplate::New (_tree_sort_col, _self));
   proto->Set ("takeItemAt", v8::FunctionTemplate::New (_tree_take_item_at, _self));
   proto->Set ("itemAt", v8::FunctionTemplate::New (_tree_item_at, _self));
   proto->Set ("itemCount", v8::FunctionTemplate::New (_tree_item_count, _self));

   _treeApi.add_function ("create", _create_tree_widget, _self);
}

