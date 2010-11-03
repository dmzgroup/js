#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <dmzV8QtUtil.h>
#include <QtGui/QToolBox>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_add_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length () > 1) {

            QWidget *widget = self->v8_to_qobject<QWidget>(Args[0]);
            QString text = v8_to_qstring (Args[1]);
            tbox->addItem (widget, text);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         result = v8::Number::New (tbox->count ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_curr_index (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length ()) { tbox->setCurrentIndex (v8_to_uint32 (Args[0])); }
         result = v8::Number::New (tbox->currentIndex ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_curr_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length ()) {

            tbox->setCurrentWidget (self->v8_to_qobject<QWidget>(Args[0]));
         }
         result = self->create_v8_qwidget (tbox->currentWidget ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_index_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length ()) {

            QWidget *widget = self->v8_to_qobject<QWidget>(Args[0]);
            result = v8::Number::New (tbox->indexOf (widget));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_insert_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length () > 2) {

            QWidget *widget = self->v8_to_qobject<QWidget>(Args[0]);
            QString text = v8_to_qstring (Args[1]);
            int index = v8_to_uint32 (Args[2]);
            tbox->insertItem (index, widget, text);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_item_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length ()) {

            int index = v8_to_uint32 (Args[0]);
            if (Args.Length () > 1) {

               tbox->setItemText (index, v8_to_qstring (Args[1]));
            }
            result = v8::String::New (qPrintable (tbox->itemText (index)));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length ()) {

            QWidget *widget = tbox->widget (v8_to_uint32 (Args[0]));
            result = self->create_v8_qwidget (widget);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_toolbox_remove_item (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QToolBox *tbox = self->v8_to_qobject<QToolBox>(Args.This ());
      if (tbox) {

         if (Args.Length ()) { tbox->removeItem (v8_to_uint32 (Args[0])); }
      }
   }

   return scope.Close (result);
}

void
dmz::JsModuleUiV8QtBasic::_init_toolbox () {

   v8::HandleScope scope;

   _toolboxTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _toolboxTemp->Inherit (_frameTemp);

   V8ObjectTemplate instance = _toolboxTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _toolboxTemp->PrototypeTemplate ();
   proto->Set ("addItem", v8::FunctionTemplate::New (_toolbox_add_item, _self));
   proto->Set ("count", v8::FunctionTemplate::New (_toolbox_count, _self));
   proto->Set ("currentIndex", v8::FunctionTemplate::New (_toolbox_curr_index, _self));
   proto->Set ("currentWidget", v8::FunctionTemplate::New (_toolbox_curr_widget, _self));
   proto->Set ("indexOf", v8::FunctionTemplate::New (_toolbox_index_of, _self));
   proto->Set ("insertItem", v8::FunctionTemplate::New (_toolbox_insert_item, _self));
   proto->Set ("itemText", v8::FunctionTemplate::New (_toolbox_item_text, _self));
   proto->Set ("removeItem", v8::FunctionTemplate::New (_toolbox_remove_item, _self));
   proto->Set ("widget", v8::FunctionTemplate::New (_toolbox_widget, _self));
}
