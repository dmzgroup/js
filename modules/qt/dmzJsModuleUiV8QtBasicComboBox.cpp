#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtGui/QComboBox>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         result = v8::Number::New (cb->count ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_current_index (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         if (Args.Length ()) {

            cb->setCurrentIndex (v8_to_int32 (Args[0]));
         }

         result = v8::Number::New (cb->currentIndex ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_current_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         if (Args.Length ()) {

            cb->setCurrentIndex (cb->findText (v8_to_qstring (Args[0])));
         }
         result = qstring_to_v8 (cb->currentText ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_add_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         if (Args.Length () > 0) {

            QString item = v8_to_qstring(Args[0]);
            cb->addItem (item);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_add_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         if (Args.Length () > 0) {

            QStringList list;
            V8Array array = v8_to_array (Args[0]);
            const int Length = array->Length ();
            for (int ix = 0; ix < Length; ++ix) {

               V8Value value = array->Get (v8::Integer::New (ix));
               list << v8_to_qstring(value);
            }

            cb->addItems (list);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_find_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         if (Args.Length () > 0) {

            QString param = v8_to_qstring (Args[0]);
            result = v8::Number::New (cb->findText (param));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_item_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         if (Args.Length () > 0) {

            result = qstring_to_v8 (cb->itemText (v8_to_number(Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_remove_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         if (Args.Length () > 0) {

            cb->removeItem (v8_to_number(Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QComboBox *cb = self->v8_to_qobject<QComboBox> (Args.This ());;
      if (cb) {

         cb->clear ();
      }
   }

   return scope.Close (result);

}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_comboBox (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QComboBox *widget = new QComboBox (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_combobox () {

   v8::HandleScope scope;

   _comboBoxTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _comboBoxTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _comboBoxTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _comboBoxTemp->PrototypeTemplate ();
   proto->Set ("count", v8::FunctionTemplate::New (_combobox_count,_self));

   proto->Set (
      "currentIndex",
      v8::FunctionTemplate::New (_combobox_current_index, _self));

   proto->Set (
      "currentText",
      v8::FunctionTemplate::New (_combobox_current_text, _self));

   proto->Set ("addItem", v8::FunctionTemplate::New (_combobox_add_item, _self));
   proto->Set ("addItems", v8::FunctionTemplate::New (_combobox_add_items, _self));
   proto->Set ("itemText", v8::FunctionTemplate::New (_combobox_item_text, _self));
   proto->Set ("findText", v8::FunctionTemplate::New (_combobox_find_text, _self));
   proto->Set ("removeIndex", v8::FunctionTemplate::New (_combobox_remove_item, _self));
   proto->Set ("clear", v8::FunctionTemplate::New (_combobox_clear, _self));

   _comboBoxApi.add_function ("create", _create_comboBox, _self);
}
