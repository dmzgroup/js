#include "dmzJsModuleUiV8QtBasic.h"
#include "dmzV8QtUtil.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QGroupBox>


//dmz::V8Value
//dmz::JsModuleUiV8QtBasic::_group_box_create (const v8::Arguments &Args) {

//   v8::HandleScope scope;
//   V8Value result = v8::Undefined ();

//   JsModuleUiV8QtBasic *self = _to_self (Args);

//   if (self) {

//      QWidget *parent = self->_to_qwidget (Args[0]);
//      result = self->create_v8_qwidget (new QGroupBox (parent ? parent : 0));
//   }

//   return scope.Close (result);
//}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_group_box_flat (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QGroupBox *gbox = self->v8_to_qobject<QGroupBox> (Args.This ());
      if (Args.Length ()) {

         gbox->setFlat (v8_to_boolean (Args[0]));
      }

      result = v8::Boolean::New (gbox->isFlat ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_group_box_title (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QGroupBox *gbox = self->v8_to_qobject<QGroupBox> (Args.This ());
      if (Args.Length ()) {

         gbox->setTitle (v8_to_qstring (Args[0]));
      }

      result = v8::String::New (qPrintable (gbox->title ()));
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_group_box_alignment (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QGroupBox *gbox = self->v8_to_qobject<QGroupBox> (Args.This ());
      if (Args.Length ()) {

         gbox->setAlignment ((Qt::Alignment)v8_to_int32 (Args[0]));
      }

      result = v8::Number::New (gbox->alignment ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_group_box_checkable (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QGroupBox *gbox = self->v8_to_qobject<QGroupBox> (Args.This ());
      if (Args.Length ()) {

         gbox->setCheckable (v8_to_boolean (Args[0]));
      }

      result = v8::Boolean::New (gbox->isCheckable ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_group_box (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *parent = 0;
      QGroupBox *gbox = 0;
      QString str;

      if (Args.Length ()) {

         if (Args[0]->IsString ()) {

            str = v8_to_qstring (Args[0]);
         }
         else if (Args.Length () > 1) {

            parent = self->v8_to_qobject<QWidget> (Args[0]);
            str = v8_to_qstring (Args[1]);
         }
      }

      if (parent && (!str.isEmpty ())) { gbox = new QGroupBox (str, parent); }
      else if (parent) { gbox = new QGroupBox (parent); }
      else if (!str.isEmpty ()) { gbox = new QGroupBox (str); }
      else { gbox = new QGroupBox (); }

      result = self->create_v8_qobject (gbox);
   }

   return scope.Close (result);
}

void
dmz::JsModuleUiV8QtBasic::_init_group_box () {

   v8::HandleScope scope;

   _groupBoxTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _groupBoxTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _groupBoxTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _groupBoxTemp->PrototypeTemplate ();
   proto->Set ("flat", v8::FunctionTemplate::New (_group_box_flat, _self));
   proto->Set ("title", v8::FunctionTemplate::New (_group_box_title, _self));
   proto->Set ("alignment", v8::FunctionTemplate::New (_group_box_alignment, _self));
   proto->Set ("checkable", v8::FunctionTemplate::New (_group_box_checkable, _self));

   _groupBoxApi.add_function ("create", _create_group_box, _self);
}
