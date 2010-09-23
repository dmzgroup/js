#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QComboBox>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_combobox_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            result = v8::Number::New (cb->count ());
         }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            result = v8::Number::New (cb->currentIndex ());
         }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            result = v8::String::New (qPrintable(cb->currentText ()));
         }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            if (Args.Length () > 0) {

               String item = v8_to_string(Args[0]);
               cb->addItem (item.get_buffer ());
            }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            if (Args.Length () > 0) {

               QStringList list;
               V8Array array = v8_to_array (Args[0]);
               const int Length = array->Length ();
               for (int ix = 0; ix < Length; ++ix) {

                  V8Value value = array->Get (v8::Integer::New (ix));
                  String val = v8_to_string(value);
                  list << val.get_buffer ();
               }
               cb->addItems (list);
            }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            if (Args.Length () > 0) {

               String param = v8_to_string (Args[0]);
               result = v8::Number::New (cb->findText (param.get_buffer ()));
            }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            if (Args.Length () > 0) {

               result = v8::String::New (
                  qPrintable (cb->itemText (v8_to_number(Args[0]))));
            }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            if (Args.Length () > 0) {

               cb->removeItem (v8_to_number(Args[0]));
            }
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

      QWidget *widget = self->_to_qt_widget (Args.This ());

      if (widget) {

         QComboBox *cb = qobject_cast<QComboBox *>(widget);

         if (cb) {

            cb->clear ();
         }
      }
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
}
