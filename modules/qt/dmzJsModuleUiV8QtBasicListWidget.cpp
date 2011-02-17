#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtListWidget.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QAbstractButton>
#include <QtGui/QListWidgetItem>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_item_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidgetItem *item = self->_to_qlistwidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            item->setText (v8_to_qstring (Args[0]));
         }

         result = qstring_to_v8 (item->text ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_item_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidgetItem *item = self->_to_qlistwidgetitem (Args.This ());
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
dmz::JsModuleUiV8QtBasic::_list_widget_item_bg_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidgetItem *item = self->_to_qlistwidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) {

            QBrush *brush = self->_to_gbrush (Args[0]);
            if (brush) { item->setBackground (*brush); }
         }

         result = self->create_v8_gbrush (new QBrush (item->background ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_item_hidden (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidgetItem *item = self->_to_qlistwidgetitem (Args.This ());
      if (item) {

         if (Args.Length ()) { item->setHidden (v8_to_boolean (Args[0])); }

         result = v8::Boolean::New (item->isHidden ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_add_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         if (Args.Length ()) {

            QListWidgetItem *item = self->_to_qlistwidgetitem (Args[0]);

            if (!item) {

               QString param = v8_to_qstring (Args[0]);
               item = new QListWidgetItem (param, lw);
            }
            else { lw->addItem (item); }

            if (Args.Length () >= 2) {

               QVariant variant = self->_qvariant_wrap_v8 (Args[1]);
               item->setData (Qt::UserRole, variant);
            }

            result = self->create_v8_qlistwidgetitem (item);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_take_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         if (Args.Length ()) {

            result =
               self->create_v8_qlistwidgetitem (lw->takeItem (v8_to_uint32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_remove_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::False ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         if (Args.Length ()) {

            QListWidgetItem *item = self->_to_qlistwidgetitem (Args[0]);
            if (item) {

               item = lw->takeItem (lw->row (item));
               result = v8::Boolean::New (item ? true : false);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_current_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         QListWidgetItem *item = lw->currentItem ();
         if (item) {

            result = self->create_v8_qlistwidgetitem (item);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         lw->clear ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         result = v8::Number::New (lw->count ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         if (Args.Length ()) {

            QListWidgetItem *item = lw->item (v8_to_int32 (Args[0]));
            if (item) {

               result = self->create_v8_qlistwidgetitem (item);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         if (Args.Length ()) {

            QListWidgetItem *item = self->_to_qlistwidgetitem (Args[0]);
            if (item) {

               result = v8::Number::New (lw->row (item));
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_list_widget_find_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *table = self->v8_to_qobject<QListWidget> (Args.This ());
      if (table) {

         if (Args.Length ()) {

            QString str (v8_to_qstring (Args[0]));
            Qt::MatchFlags flag = Qt::MatchExactly;
            if (Args.Length () > 1) {

               flag = (Qt::MatchFlags)v8_to_uint32 (Args[1]);
            }
            QList<QListWidgetItem *> items = table->findItems (str, flag);
            const int Length = items.count ();
            V8Array array = v8::Array::New (Length);
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = self->create_v8_qlistwidgetitem (items.at (ix));
               array->Set (v8::Integer::New (ix), value);
            }
            result = array;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_qlistwidgetitem (QListWidgetItem *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_listWidgetItemCtor.IsEmpty ()) { obj = _listWidgetItemCtor->NewInstance (); }

      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;
      }
   }

   return scope.Close (result);
}


QListWidgetItem *
dmz::JsModuleUiV8QtBasic::_to_qlistwidgetitem (V8Value value) {

   v8::HandleScope scope;
   QListWidgetItem *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_listWidgetItemTemp->HasInstance (obj)) {

         result = (QListWidgetItem *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_list (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QListWidget *widget = new QListWidget (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_list_widget_item () {

   v8::HandleScope scope;

   _listWidgetItemTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _listWidgetItemTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _listWidgetItemTemp->PrototypeTemplate ();
   proto->Set ("text", v8::FunctionTemplate::New (_list_widget_item_text, _self));
   proto->Set ("data", v8::FunctionTemplate::New (_list_widget_item_data, _self));
   proto->Set ("background", v8::FunctionTemplate::New (_list_widget_item_bg_brush, _self));
   proto->Set ("hidden", v8::FunctionTemplate::New (_list_widget_item_hidden, _self));
}


void
dmz::JsModuleUiV8QtBasic::_init_list_widget () {

   v8::HandleScope scope;

   _listWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _listWidgetTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _listWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _listWidgetTemp->PrototypeTemplate ();
   proto->Set ("addItem", v8::FunctionTemplate::New (_list_widget_add_item, _self));

   proto->Set (
      "currentItem",
      v8::FunctionTemplate::New (_list_widget_current_item, _self));

   proto->Set ("clear", v8::FunctionTemplate::New (_list_widget_clear, _self));
   proto->Set ("count", v8::FunctionTemplate::New (_list_widget_count, _self));
   proto->Set ("item", v8::FunctionTemplate::New (_list_widget_item, _self));
   proto->Set ("row", v8::FunctionTemplate::New (_list_widget_row, _self));
   proto->Set ("takeItem", v8::FunctionTemplate::New (_list_widget_take_item, _self));
   proto->Set ("removeItem", v8::FunctionTemplate::New (_list_widget_remove_item, _self));
   proto->Set ("findItems", v8::FunctionTemplate::New (_list_widget_find_items, _self));

   _listApi.add_function ("create", _create_list, _self);
}

