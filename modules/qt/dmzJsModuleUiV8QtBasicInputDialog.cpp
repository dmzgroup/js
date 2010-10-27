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

//            if (params->Has (_currentStr)) {

//               dialog->setIntValue (v8_to_int32 (params->Get (_currentStr)));
//            }

//            if (params->Has (_editableStr)) {

//            }
         }
         else {

            delete dialog; dialog = 0;
         }
      }
   }

   return dialog;
}


void
dmz::JsModuleUiV8QtBasic::_init_input_dialog () {

   v8::HandleScope scope;

   _inputDialogApi.add_constant ("TextInput", (UInt32)QInputDialog::TextInput);
   _inputDialogApi.add_constant ("IntInput", (UInt32)QInputDialog::IntInput);
   _inputDialogApi.add_constant ("DoubleInput", (UInt32)QInputDialog::DoubleInput);
   _inputDialogApi.add_constant ("NoButtons", (UInt32)QInputDialog::NoButtons);

   _inputDialogApi.add_constant (
      "UseListViewForComboBoxItems",
      (UInt32)QInputDialog::UseListViewForComboBoxItems);

   _inputDialogApi.add_function ("create", _create_input_dialog, _self);
}
