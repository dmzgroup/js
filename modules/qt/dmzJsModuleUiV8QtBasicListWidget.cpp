#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtListWidget.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QAbstractButton>
#include <QtGui/QListWidgetItem>


using namespace dmz;


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
dmz::JsModuleUiV8QtBasic::_list_widget_add_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QListWidget *lw = self->v8_to_qobject<QListWidget> (Args.This ());
      if (lw) {

         if (Args.Length ()) {

            QString param = v8_to_qstring (Args[0]);
            QListWidgetItem *item = new QListWidgetItem (param, lw);

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

         if (Args.Length () > 0) {

            QListWidgetItem *item = self->_to_qlistwidgetitem (Args[0]);

            if (item) {

               item = lw->takeItem(lw->row(item));

               if (item) { result = Args[0]; }
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


void
dmz::JsModuleUiV8QtBasic::_init_list_widget_item () {

   v8::HandleScope scope;

   _listWidgetItemTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _listWidgetItemTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _listWidgetItemTemp->PrototypeTemplate ();
   proto->Set ("text", v8::FunctionTemplate::New (_list_widget_item_text, _self));
   proto->Set ("data", v8::FunctionTemplate::New (_list_widget_item_data, _self));
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
   proto->Set ("currentItem", v8::FunctionTemplate::New (_list_widget_current_item, _self));
   proto->Set ("clear", v8::FunctionTemplate::New (_list_widget_clear, _self));
   // proto->Set ("item", v8::FunctionTemplate::New (_list_widget_item, _self));
   proto->Set ("takeItem", v8::FunctionTemplate::New (_list_widget_take_item, _self));
}

