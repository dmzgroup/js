#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtGui/QTextEdit>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_textEdit_append (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTextEdit *text = self->v8_to_qobject<QTextEdit>(Args.This ());
      if (text) {

         if (Args.Length ()) {

            text->append (v8_to_string (Args[0]).get_buffer ());
         }

         result = qstring_to_v8 (text->toPlainText ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_textEdit_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTextEdit *text = self->v8_to_qobject<QTextEdit>(Args.This ());
      if (text) {

         if (Args.Length ()) {

            text->setText (v8_to_string (Args[0]).get_buffer ());
         }

         result = qstring_to_v8 (text->toPlainText ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_textEdit_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTextEdit *text = self->v8_to_qobject<QTextEdit>(Args.This ());
      if (text) {

         text->clear ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_textEdit_undo (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTextEdit *text = self->v8_to_qobject<QTextEdit>(Args.This ());
      if (text) {

         text->undo ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_textEdit_redo (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTextEdit *text = self->v8_to_qobject<QTextEdit>(Args.This ());
      if (text) {

         text->redo ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_textEdit_allow_undo (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QTextEdit *text = self->v8_to_qobject<QTextEdit>(Args.This ());
      if (text) {

         if (Args.Length ()) {

            text->setUndoRedoEnabled (v8_to_boolean (Args[0]));
         }

         result = v8::Boolean::New (text->isUndoRedoEnabled ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_textEdit () {

   v8::HandleScope scope;

   _textEditTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _textEditTemp->Inherit (_frameTemp);

   V8ObjectTemplate instance = _textEditTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _textEditTemp->PrototypeTemplate ();
   proto->Set ("append", v8::FunctionTemplate::New (_textEdit_append, _self));
   proto->Set ("text", v8::FunctionTemplate::New (_textEdit_text, _self));
   proto->Set ("clear", v8::FunctionTemplate::New (_textEdit_clear, _self));
   proto->Set ("undo", v8::FunctionTemplate::New (_textEdit_undo, _self));
   proto->Set ("redo", v8::FunctionTemplate::New (_textEdit_redo, _self));
   proto->Set ("allowUndo", v8::FunctionTemplate::New (_textEdit_allow_undo, _self));
}
