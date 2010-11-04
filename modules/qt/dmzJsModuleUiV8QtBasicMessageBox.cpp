#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzV8QtUtil.h>
#include "dmzV8QtObject.h"
#include <QtGui/QMessageBox>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_message_box (const v8::Arguments &Args) {

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

         QMessageBox *dialog = self->_create_message_box (params, parent);
         result = self->create_v8_qwidget (dialog);
      }
   }

   return scope.Close (result);
}


QMessageBox *
dmz::JsModuleUiV8QtBasic::_create_message_box (V8Object params, QWidget *parent) {

   QMessageBox *dialog (0);

   if (!params.IsEmpty ()) {

      dialog = new QMessageBox (parent);

      if (params->Has (_typeStr)) {

         QMessageBox::Icon icon (QMessageBox::Information);
         const UInt32 TypeValue = v8_to_uint32 (params->Get (_typeStr));
         if (TypeValue == QMessageBox::Question) { icon = QMessageBox::Question; }
         else if (TypeValue == QMessageBox::Warning) { icon = QMessageBox::Warning; }
         else if (TypeValue == QMessageBox::Critical) { icon = QMessageBox::Critical; }
         dialog->setIcon (icon);
      }

      if (params->Has (_textStr)) {

         String text = v8_to_string (params->Get (_textStr));
         if (text) { dialog->setText (text.get_buffer ()); }
      }

      if (params->Has (_detailedTextStr)) {

         String text = v8_to_string (params->Get (_detailedTextStr));
         if (text) { dialog->setDetailedText (text.get_buffer ()); }
      }

      if (params->Has (_infoTextStr)) {

         String infoText = v8_to_string (params->Get (_infoTextStr));
         if (infoText) { dialog->setInformativeText (infoText.get_buffer ()); }
      }

      if (params->Has (_standardButtonsStr)) {

         V8Array buttonArray = v8_to_array (params->Get (_standardButtonsStr));
         if (!buttonArray.IsEmpty ()) {

            QMessageBox::StandardButtons buttons;
            const uint32_t Length = buttonArray->Length ();
            for (uint32_t ix = 0; ix < Length; ix++) {

               const UInt32 ButtonValue =
                  v8_to_uint32 (buttonArray->Get (v8::Integer::NewFromUnsigned (ix)));

               buttons |= (QMessageBox::StandardButton)ButtonValue;
            }

            dialog->setStandardButtons (buttons);
         }
      }

      if (params->Has (_defaultButtonStr)) {

         const UInt32 ButtonValue = v8_to_uint32 (params->Get (_defaultButtonStr));
         dialog->setDefaultButton ((QMessageBox::StandardButton)ButtonValue);
      }
   }

   return dialog;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_messagebox_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QMessageBox *mbox = self->v8_to_qobject<QMessageBox> (Args.This ());
      if (mbox) {

         self->_log.warn <<"Set text" << endl;
         if (Args.Length ()) { mbox->setText (v8_to_qstring (Args[0])); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_messagebox_itext (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QMessageBox *mbox = self->v8_to_qobject<QMessageBox> (Args.This ());
      if (mbox) {

         self->_log.warn <<"Set itext" << endl;
         if (Args.Length ()) { mbox->setInformativeText (v8_to_qstring (Args[0])); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_messagebox_dtext (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QMessageBox *mbox = self->v8_to_qobject<QMessageBox> (Args.This ());
      if (mbox) {

         self->_log.warn <<"Set dtext" << endl;
         if (Args.Length ()) { mbox->setDetailedText (v8_to_qstring (Args[0])); }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_message_box () {

   v8::HandleScope scope;

   _messageBoxApi.add_constant ("Infomation", (UInt32)QMessageBox::Information);
   _messageBoxApi.add_constant ("Quesiton", (UInt32)QMessageBox::Question);
   _messageBoxApi.add_constant ("Warning", (UInt32)QMessageBox::Warning);
   _messageBoxApi.add_constant ("Critical", (UInt32)QMessageBox::Critical);

   _messageBoxApi.add_constant ("Ok", (UInt32)QMessageBox::Ok);
   _messageBoxApi.add_constant ("Open", (UInt32)QMessageBox::Open);
   _messageBoxApi.add_constant ("Save", (UInt32)QMessageBox::Save);
   _messageBoxApi.add_constant ("Cancel", (UInt32)QMessageBox::Cancel);
   _messageBoxApi.add_constant ("Close", (UInt32)QMessageBox::Close);
   _messageBoxApi.add_constant ("Apply", (UInt32)QMessageBox::Apply);
   _messageBoxApi.add_constant ("Reset", (UInt32)QMessageBox::Reset);
   _messageBoxApi.add_constant ("Help", (UInt32)QMessageBox::Help);
   _messageBoxApi.add_constant ("SaveAll", (UInt32)QMessageBox::SaveAll);
   _messageBoxApi.add_constant ("Yes", (UInt32)QMessageBox::Yes);
   _messageBoxApi.add_constant ("YesToAll", (UInt32)QMessageBox::YesToAll);
   _messageBoxApi.add_constant ("No", (UInt32)QMessageBox::No);
   _messageBoxApi.add_constant ("NoToAll", (UInt32)QMessageBox::NoToAll);
   _messageBoxApi.add_constant ("Abort", (UInt32)QMessageBox::Abort);
   _messageBoxApi.add_constant ("Retry", (UInt32)QMessageBox::Retry);
   _messageBoxApi.add_constant ("Ignore", (UInt32)QMessageBox::Ignore);
   _messageBoxApi.add_constant ("NoButton", (UInt32)QMessageBox::NoButton);

   _messageBoxApi.add_function ("create", _create_message_box, _self);


   _messageboxTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _messageboxTemp->Inherit (_dialogTemp);

   V8ObjectTemplate instance = _messageboxTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _messageboxTemp->PrototypeTemplate ();
   proto->Set ("setText", v8::FunctionTemplate::New (_messagebox_text, _self));
   proto->Set ("setDetailedText", v8::FunctionTemplate::New (_messagebox_dtext, _self));
   proto->Set ("setInformativeText", v8::FunctionTemplate::New (_messagebox_itext, _self));
}
