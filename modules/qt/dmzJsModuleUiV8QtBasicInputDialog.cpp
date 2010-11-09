#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtObject.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QInputDialog>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_input_dialog (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      V8Object params = v8_to_object (Args[0]);

      if (Args.Length () >= 2) {

         parent = self->_to_qwidget (Args[1]);
      }

      if (!params.IsEmpty ()) {

         QInputDialog *dialog = self->_create_input_dialog (params, parent);
         result = self->create_v8_qwidget (dialog);
      }
   }

   return scope.Close (result);
}


QInputDialog *
dmz::JsModuleUiV8QtBasic::_create_input_dialog (V8Object params, QWidget *parent) {

   QInputDialog *dialog (0);

   if (!params.IsEmpty ()) {

#if 0
      QWidget *parent
      QString &title
      QString &label

      // getText
      QLineEdit::EchoMode mode
      QString &text

      // getInt
      int value
      int min
      int max
      int step

      // getDouble
      double value
      double min
      double max
      int decimals

      // getItem
      QStringList &items
      int current
      bool editable
#endif

      if (params->Has (_titleStr) && params->Has (_labelStr)) {

         dialog = new QInputDialog (parent);

         dialog->setWindowTitle (v8_to_qstring (params->Get (_titleStr)));
         dialog->setLabelText (v8_to_qstring (params->Get (_labelStr)));

         if (params->Has (_textStr)) { // getText

            dialog->setTextValue (v8_to_qstring (params->Get (_textStr)));

            if (params->Has (_modeStr)) {

               QLineEdit::EchoMode mode (QLineEdit::Normal);
               const UInt32 Value = v8_to_uint32 (params->Get (_modeStr));
               mode = (QLineEdit::EchoMode)Value;
               dialog->setTextEchoMode (mode);
            }
         }
         else if (params->Has (_stepStr)) { // getInt

            dialog->setIntStep (v8_to_int32 (params->Get (_stepStr)));

            if (params->Has (_valueStr)) {

               dialog->setIntValue (v8_to_int32 (params->Get (_valueStr)));
            }

            if (params->Has (_minStr)) {

               dialog->setIntMinimum (v8_to_int32 (params->Get (_minStr)));
            }

            if (params->Has (_maxStr)) {

               dialog->setIntMaximum (v8_to_int32 (params->Get (_maxStr)));
            }
         }
         else if (params->Has (_decimalStr)) { // getDouble

            dialog->setDoubleDecimals (v8_to_int32 (params->Get (_decimalStr)));

            if (params->Has (_valueStr)) {

               dialog->setDoubleValue (v8_to_number (params->Get (_valueStr)));
            }

            if (params->Has (_minStr)) {

               dialog->setDoubleMinimum (v8_to_number (params->Get (_minStr)));
            }

            if (params->Has (_maxStr)) {

               dialog->setDoubleMaximum (v8_to_number (params->Get (_maxStr)));
            }
         }
         else if (params->Has (_itemsStr)) { // getItem

            QStringList items = v8_to_qstringlist (params->Get (_itemsStr));
            dialog->setComboBoxItems (items);

            if (params->Has (_currentStr)) {

               Int32 current = v8_to_int32 (params->Get (_currentStr));
               QString text (items.value (current));
               dialog->setTextValue (text);
            }

            if (params->Has (_editableStr)) {

               dialog->setComboBoxEditable (v8_to_boolean (params->Get (_editableStr)));
            }
         }
      }
   }

   return dialog;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_ddec (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setDoubleDecimals (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (input->doubleDecimals ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_dmax (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setDoubleMaximum (v8_to_number (Args[0])); }
         result = v8::Number::New (input->doubleMaximum ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_dmin (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setDoubleMinimum (v8_to_number (Args[0])); }
         result = v8::Number::New (input->doubleMinimum ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_dval (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setDoubleValue (v8_to_number (Args[0])); }
         result = v8::Number::New (input->doubleValue ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_imax (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setIntMaximum (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (input->intMaximum ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_imin (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setIntMinimum (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (input->intMinimum ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_istep (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setIntStep (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (input->intStep ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_ival (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setIntValue (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (input->intValue ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_label_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setLabelText (v8_to_qstring (Args[0])); }
         result = v8::String::New (qPrintable (input->labelText ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_ok_btn_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setOkButtonText (v8_to_qstring (Args[0])); }
         result = v8::String::New (qPrintable (input->okButtonText ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_cancel_btn_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setCancelButtonText (v8_to_qstring (Args[0])); }
         result = v8::String::New (qPrintable (input->cancelButtonText ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_tval (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setTextValue (v8_to_qstring (Args[0])); }
         result = v8::String::New (qPrintable (input->textValue ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_cbox_items (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setComboBoxItems (v8_to_qstringlist (Args[0])); }
         result = qstringlist_to_v8 (input->comboBoxItems ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_input_dialog_cbox_editable (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QInputDialog *input = self->v8_to_qobject<QInputDialog> (Args.This ());
      if (input) {

         if (Args.Length ()) { input->setComboBoxEditable (v8_to_boolean (Args[0])); }
         result = v8::Boolean::New (input->isComboBoxEditable ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_input_dialog () {

   v8::HandleScope scope;

   _inputDialogTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _inputDialogTemp->Inherit (_dialogTemp);

   V8ObjectTemplate instance = _inputDialogTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _inputDialogTemp->PrototypeTemplate ();

   proto->Set ("doubleDecimals", v8::FunctionTemplate::New (_input_dialog_ddec, _self));
   proto->Set ("doubleMaximum", v8::FunctionTemplate::New (_input_dialog_dmax, _self));
   proto->Set ("doubleMinimum", v8::FunctionTemplate::New (_input_dialog_dmin, _self));
   proto->Set ("doubleValue", v8::FunctionTemplate::New (_input_dialog_dval, _self));
   proto->Set ("intMaximum", v8::FunctionTemplate::New (_input_dialog_imax, _self));
   proto->Set ("intMinimum", v8::FunctionTemplate::New (_input_dialog_imin, _self));
   proto->Set ("intStep", v8::FunctionTemplate::New (_input_dialog_istep, _self));
   proto->Set ("intValue", v8::FunctionTemplate::New (_input_dialog_ival, _self));
   proto->Set ("labelText", v8::FunctionTemplate::New (_input_dialog_label_text, _self));
   proto->Set ("okButtonText", v8::FunctionTemplate::New (_input_dialog_ok_btn_text, _self));
   proto->Set ("cancelButtonText", v8::FunctionTemplate::New (_input_dialog_cancel_btn_text, _self));
   proto->Set ("textValue", v8::FunctionTemplate::New (_input_dialog_tval, _self));
   proto->Set ("comboBoxItems", v8::FunctionTemplate::New (_input_dialog_cbox_items, _self));
   proto->Set ("comboBoxEditable", v8::FunctionTemplate::New (_input_dialog_cbox_editable, _self));


//   _inputDialogApi.add_constant ("TextInput", (UInt32)QInputDialog::TextInput);
//   _inputDialogApi.add_constant ("IntInput", (UInt32)QInputDialog::IntInput);
//   _inputDialogApi.add_constant ("DoubleInput", (UInt32)QInputDialog::DoubleInput);
//   _inputDialogApi.add_constant ("NoButtons", (UInt32)QInputDialog::NoButtons);
//   _inputDialogApi.add_constant ("Normal", (UInt32)QLineEdit::Normal);
//   _inputDialogApi.add_constant ("Password", (UInt32)QLineEdit::Password);

//   _inputDialogApi.add_constant (
//      "UseListViewForComboBoxItems",
//      (UInt32)QInputDialog::UseListViewForComboBoxItems);

   _inputDialogApi.add_function ("create", _create_input_dialog, _self);
}
