#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <dmzV8QtUtil.h>
#include <QtGui/QLabel>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_label_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLabel *label = self->v8_to_qobject<QLabel>(Args.This ());
      if (label) {

         if (Args.Length ()) {

            label->setText (v8_to_string (Args[0]).get_buffer ());
         }
         else {

            result = v8::String::New (qPrintable(label->text ()));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_label_word_wrap (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLabel *label = self->v8_to_qobject<QLabel>(Args.This ());
      if (label) {

         if (Args.Length ()) {

            label->setWordWrap (v8_to_boolean (Args[0]));
         }
         else {

            result = v8::Boolean::New (label->wordWrap ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_label_clear (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLabel *label = self->v8_to_qobject<QLabel>(Args.This ());
      if (label) {

         label->clear ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_label_pixmap (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLabel *label = self->v8_to_qobject<QLabel>(Args.This ());
      if (label) {

         if (Args.Length ()) {

            QPixmap *pixmap = self->_to_gpixmap (Args[0]);
            if (pixmap) { label->setPixmap (*pixmap); }
         }

         result = self->create_v8_gpixmap (new QPixmap (*label->pixmap ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_label (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      QLabel *widget = 0;
      QString str;
      if (Args.Length ()) {

         if (Args[0]->IsString ()) {

            str = v8_to_qstring (Args[0]);
            if (Args.Length () > 1) { parent = self->_to_qwidget (Args[1]); }
         }
         else { parent = self->_to_qwidget (Args[0]); }
      }
      if (str.isEmpty ()) { widget = new QLabel (parent); }
      else { widget = new QLabel (str, parent); }
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_label () {

   v8::HandleScope scope;

   _labelTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _labelTemp->Inherit (_frameTemp);

   V8ObjectTemplate instance = _labelTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _labelTemp->PrototypeTemplate ();
   proto->Set ("text", v8::FunctionTemplate::New (_label_text, _self));
   proto->Set ("wordWrap", v8::FunctionTemplate::New (_label_word_wrap, _self));
   proto->Set ("clear", v8::FunctionTemplate::New (_label_clear, _self));
   proto->Set ("pixmap", v8::FunctionTemplate::New (_label_pixmap, _self));

   _labelApi.add_function ("create", _create_label, _self);
}
