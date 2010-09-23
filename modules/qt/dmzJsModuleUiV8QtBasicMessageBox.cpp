#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtObject.h"
#include <QtGui/QMessageBox>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_message_box (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      if (Args.Length () == 0) {


      }
      else {

      }

      QMessageBox *msgBox = new QMessageBox;
      msgBox->setIcon (QMessageBox::Question);
      msgBox->setText("The document has been modified.");
      msgBox->setInformativeText("Do you want to save your changes?");
      msgBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      msgBox->setDefaultButton(QMessageBox::Save);

//      QMessageBox::StandardButton ret;
      QObject *obj = new QObject (0);
      msgBox->open (obj, SLOT (deleteLater ()));

//         QMessageBox::information (
//            0,
//            "Title",
//            "This is a info mb",
//            QMessageBox::Ok | QMessageBox::Cancel);

//      result = v8::Number::New (ret);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_message_box_information (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QMessageBox *msgBox = new QMessageBox;
      msgBox->setIcon (QMessageBox::Question);
      msgBox->setText("The document has been modified.");
      msgBox->setInformativeText("Do you want to save your changes?");
      msgBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      msgBox->setDefaultButton(QMessageBox::Save);

//      QMessageBox::StandardButton ret;
      QObject *obj = new QObject (0);
      msgBox->open (obj, SLOT (deleteLater ()));

//         QMessageBox::information (
//            0,
//            "Title",
//            "This is a info mb",
//            QMessageBox::Ok | QMessageBox::Cancel);

//      result = v8::Number::New (ret);
   }

   return scope.Close (result);
}


// dmz::V8Value
// dmz::JsModuleUiV8QtBasic::_message_box_question (const v8::Arguments &Args) {
//
//    v8::HandleScope scope;
//    V8Value result = v8::Undefined ();
//
//    JsModuleUiV8QtBasic *self = _to_self (Args);
//    if (self) {
//
//       QMessageBox::StandardButton retVal =
//          QMessageBox::information (
//             0,
//             "Title",
//             "This is a question mb",
//             QMessageBox::Ok);
//
//       result = v8::Number::New (retVal);
//    }
//
//    return scope.Close (result);
// }
//
//
// dmz::V8Value
// dmz::JsModuleUiV8QtBasic::_message_box_warning (const v8::Arguments &Args) {
//
//    v8::HandleScope scope;
//    V8Value result = v8::Undefined ();
//
//    JsModuleUiV8QtBasic *self = _to_self (Args);
//    if (self) {
//
//       QMessageBox::StandardButton retVal =
//          QMessageBox::information (
//             0,
//             "Title",
//             "This is a warning mb",
//             QMessageBox::Ok);
//
//       result = v8::Number::New (retVal);
//    }
//
//    return scope.Close (result);
// }


//dmz::V8Value
//dmz::JsModuleUiV8QtBasic::create_v8_dialog (QDialog *value) {

//   v8::Context::Scope cscope (_state.context);
//   v8::HandleScope scope;

//   V8Value result = v8::Undefined ();

//   if (value) {

//      V8Object obj;

//      if (!_dialogCtor.IsEmpty ()) {

//         obj = _dialogCtor->NewInstance ();

//         if (!obj.IsEmpty ()) {

//            obj->SetInternalField (0, v8::External::Wrap ((void *)value));

//            V8ObjectPersist persist = V8ObjectPersist (obj);
//            persist.MakeWeak ((void *)value, local)
//         }
//      }

//      if (!obj.IsEmpty ()) { result = obj; }
//   }

//   return scope.Close (result);
//}


void
dmz::JsModuleUiV8QtBasic::_init_message_box () {

   v8::HandleScope scope;

   _messageBoxApi.add_constant ("Ok", (Float64)QMessageBox::Ok);
   _messageBoxApi.add_constant ("Open", (Float64)QMessageBox::Open);
   _messageBoxApi.add_constant ("Save", (Float64)QMessageBox::Save);
   _messageBoxApi.add_constant ("Cancel", (Float64)QMessageBox::Cancel);
   _messageBoxApi.add_constant ("Close", (Float64)QMessageBox::Close);
   _messageBoxApi.add_constant ("Apply", (Float64)QMessageBox::Apply);
   _messageBoxApi.add_constant ("Reset", (Float64)QMessageBox::Reset);
   _messageBoxApi.add_constant ("Help", (Float64)QMessageBox::Help);
   _messageBoxApi.add_constant ("SaveAll", (Float64)QMessageBox::SaveAll);
   _messageBoxApi.add_constant ("Yes", (Float64)QMessageBox::Yes);
   _messageBoxApi.add_constant ("YesToAll", (Float64)QMessageBox::YesToAll);
   _messageBoxApi.add_constant ("No", (Float64)QMessageBox::No);
   _messageBoxApi.add_constant ("NoToAll", (Float64)QMessageBox::NoToAll);
   _messageBoxApi.add_constant ("Abort", (Float64)QMessageBox::Abort);
   _messageBoxApi.add_constant ("Retry", (Float64)QMessageBox::Retry);
   _messageBoxApi.add_constant ("Ignore", (Float64)QMessageBox::Ignore);
   _messageBoxApi.add_constant ("NoButton", (Float64)QMessageBox::NoButton);

   // _messageBoxApi.add_function ("create", _create_message_box, _self);
   // _messageBoxApi.add_function ("critical", _message_box_critical, _self);
   _messageBoxApi.add_function ("information", _message_box_information, _self);
   // _messageBoxApi.add_function ("question", _message_box_question, _self);
   // _messageBoxApi.add_function ("warning", _message_box_warning, _self);

   // _messageBoxTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   // _messageBoxTemp->Inherit (_widgetTemp);
   //
   // V8ObjectTemplate instance = _messageBoxTemp->InstanceTemplate ();
   // instance->SetInternalFieldCount (1);
   //
   // V8ObjectTemplate proto = _messageBoxTemp->PrototypeTemplate ();
   // proto->Set ("addItem", v8::FunctionTemplate::New (_message_box_set_title, _self));
}


//void
//dmz::JsModuleUiV8QtBasic::_reset_message_box_obervers () { _msgTable.empty (); }


//void
//dmz::JsModuleUiV8QtBasic::_release_message_box_observer (const Handle InstanceHandle) {

//   MessageStruct *ms = _msgTable.remove (InstanceHandle);

//   if (ms) {

//      if (!_v8Context.IsEmpty ()) {

//         v8::Context::Scope cscope (_v8Context);

//         ms->cbTable.empty ();
//         ms->unsubscribe_to_all_messages ();
//      }

//      delete ms; ms = 0;
//   }
//}


//dmz::V8QtMessageBox *
//dmz::JsModuleUiV8QtBasic::_to_message_box_ptr (V8Value value) {

//   v8::HandleScope scope;

//   Message *result (0);

//   V8Object obj = v8_to_object (value);

//   if (!obj.IsEmpty ()) {

//      if (_messageBoxTemp->HasInstance (obj)) {

//         result = (V8QtMessageBox *)v8::External::Unwrap (obj->GetInternalField (0));
//      }
//   }

//   return result;
//}
