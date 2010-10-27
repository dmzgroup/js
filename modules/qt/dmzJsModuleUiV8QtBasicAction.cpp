#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtObject.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QAction>


#if 0
dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_action (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAction *action = new QAction;

      String text;
      String file;

      if (Args.Length () > 2) {

      }

      result = self->create_v8_qobject (action);
   }

   return scope.Close (result);
}
#endif


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_action_enabled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAction *action = self->v8_to_qobject<QAction> (Args.This ());;
      if (action) {

         if (Args.Length ()) {

            const Boolean Value = v8_to_boolean (Args[0]);
            action->setEnabled (Value);
         }

         result = v8::Boolean::New (action->isEnabled ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_action_shortcut (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAction *action = self->v8_to_qobject<QAction> (Args.This ());;
      if (action) {

         if (Args.Length ()) {

            action->setShortcut (v8_to_qkeysequence (Args[0]));
         }

         result = qstring_to_v8 (action->shortcut ().toString ());
      }
   }

   return scope.Close (result);
}



dmz::V8Value
dmz::JsModuleUiV8QtBasic::_action_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAction *action = self->v8_to_qobject<QAction> (Args.This ());;
      if (action) {

         if (Args.Length ()) {

            V8Value arg = Args[0];

            if (arg->IsString ()) {

               action->setText (v8_to_qstring (arg));
            }
            else if (v8_is_object(arg)) {

               V8Object obj = v8_to_object (arg);

               if (!obj.IsEmpty ()) {

                  if (obj->Has (self->_textStr)) {

                     action->setText (v8_to_qstring (obj->Get (self->_textStr)));
                  }

                  if (obj->Has (self->_statusTipStr)) {

                     action->setStatusTip (v8_to_qstring (obj->Get (self->_statusTipStr)));
                  }

                  if (obj->Has (self->_toolTipStr)) {

                     action->setToolTip (v8_to_qstring (obj->Get (self->_toolTipStr)));
                  }
               }
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_action_trigger (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QAction *action = self->v8_to_qobject<QAction> (Args.This ());;
      if (action) {

         action->trigger ();
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_action () {

   v8::HandleScope scope;

   _actionTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _actionTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _actionTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _actionTemp->PrototypeTemplate ();
   proto->Set ("enabled", v8::FunctionTemplate::New (_action_enabled, _self));
   proto->Set ("shortcut", v8::FunctionTemplate::New (_action_shortcut, _self));
   proto->Set ("text", v8::FunctionTemplate::New (_action_text, _self));
   proto->Set ("trigger", v8::FunctionTemplate::New (_action_trigger, _self));
}

