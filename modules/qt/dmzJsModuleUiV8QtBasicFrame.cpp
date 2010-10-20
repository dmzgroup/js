#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QFrame>


//dmz::V8Value
//dmz::JsModuleUiV8QtBasic::_frame_create (const v8::Arguments &Args) {

//   v8::HandleScope scope;
//   V8Value result = v8::Undefined ();

//   JsModuleUiV8QtBasic *self = _to_self (Args);

//   if (self) {

//      QWidget *parent = self->_to_qwidget (Args[0]);
//      result = self->create_v8_qwidget (new QFrame (parent ? parent : 0));
//   }

//   return scope.Close (result);
//}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_frame_shadow (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QFrame *frame = self->v8_to_qobject<QFrame> (Args.This ());
      if (Args.Length ()) {

         UInt32 val = v8_to_uint32 (Args[0]);
         frame->setFrameShadow ((QFrame::Shadow)val);
      }

      result = v8::Number::New (frame->frameShadow ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_frame_shape (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QFrame *frame = self->v8_to_qobject<QFrame> (Args.This ());
      if (Args.Length ()) {

         UInt32 val = v8_to_uint32 (Args[0]);
         frame->setFrameShape ((QFrame::Shape)val);
      }

      result = v8::Number::New (frame->frameShape ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_frame_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QFrame *frame = self->v8_to_qobject<QFrame> (Args.This ());

      result = v8::Number::New (frame->frameWidth ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_frame_line_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QFrame *frame = self->v8_to_qobject<QFrame> (Args.This ());
      if (Args.Length ()) {

         UInt32 val = v8_to_uint32 (Args[0]);
         frame->setLineWidth (val);
      }

      result = v8::Number::New (frame->lineWidth ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_frame_mid_line_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QFrame *frame = self->v8_to_qobject<QFrame> (Args.This ());
      if (Args.Length ()) {

         UInt32 val = v8_to_uint32 (Args[0]);
         frame->setMidLineWidth (val);
      }

      result = v8::Number::New (frame->midLineWidth ());
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_frame () {

   v8::HandleScope scope;

   _frameTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _frameTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _frameTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _frameTemp->PrototypeTemplate ();
   proto->Set ("frameShadow", v8::FunctionTemplate::New (_frame_shadow, _self));
   proto->Set ("frameShape", v8::FunctionTemplate::New (_frame_shape, _self));
   proto->Set ("frameWidth", v8::FunctionTemplate::New (_frame_width, _self));
   proto->Set ("lineWidth", v8::FunctionTemplate::New (_frame_line_width, _self));
   proto->Set ("midLineWidth", v8::FunctionTemplate::New (_frame_mid_line_width, _self));

   _frameApi.add_constant ("ShadowPlain", (UInt32)QFrame::Plain);
   _frameApi.add_constant ("ShadowRaised", (UInt32)QFrame::Raised);
   _frameApi.add_constant ("ShadowSunken", (UInt32)QFrame::Sunken);
   _frameApi.add_constant ("ShapeNoFrame", (UInt32)QFrame::NoFrame);
   _frameApi.add_constant ("ShapeBox", (UInt32)QFrame::Box);
   _frameApi.add_constant ("ShapePanel", (UInt32)QFrame::Panel);
   _frameApi.add_constant ("ShapeStyledPanel", (UInt32)QFrame::StyledPanel);
   _frameApi.add_constant ("ShapeHLine", (UInt32)QFrame::HLine);
   _frameApi.add_constant ("ShapeVLine", (UInt32)QFrame::VLine);
   _frameApi.add_constant ("ShapeWinPanel", (UInt32)QFrame::WinPanel);

//   _frameApi.add_function ("createFrame", _frame_create, _self);
}
