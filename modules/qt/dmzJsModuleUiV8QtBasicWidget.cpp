#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtObject.h"
#include "dmzV8QtUtil.h"
#include <QtGui/QWidget>

#include <QtCore/QDebug>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_class_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         result = v8::String::New (widget->metaObject ()->className ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_object_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         if (Args.Length ()) {

            const String Value = v8_to_string (Args[0]);
            widget->setObjectName (Value.get_buffer ());
         }

         result = v8::String::New (qPrintable (widget->objectName ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_close (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         widget->close ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_enabled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         if (Args.Length ()) {

            const Boolean Value = v8_to_boolean (Args[0]);
            widget->setEnabled (Value);
         }

         result = v8::Boolean::New (widget->isEnabled ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_hide (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) { widget->hide (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_parent (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) {

         QWidget *parent = widget->parentWidget ();
         result = self->create_v8_widget (parent);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_lookup (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      const String WidgetName = v8_to_string (Args[0]);

      if (widget && WidgetName) {

         QWidget *child = widget->findChild<QWidget *>(WidgetName.get_buffer ());

         if (child) {

            result = self->create_v8_widget (child);
         }
         else {

            self->_log.warn << "lookup failed to find: " << WidgetName << endl;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self && self->_state.core && v8_is_object (Args[0])) {

      V8QtObject *jsObject = self->_to_js_qt_object (Args.This ());
      if (jsObject) {

         const Int32 Argc (Args.Length());
         const Int32 WidgetIndex (Argc > 3 ? 1 : 0);
         const Int32 SignalIndex (Argc > 3 ? 2 : 1);
         const Int32 FuncIndex (Argc > 3 ? 3 : 2);

         V8Object src = v8_to_object (Args[0]);
         V8Function func = v8_to_function (Args[FuncIndex]);
         const String Signal = v8_to_string (Args[SignalIndex]);

         if (WidgetIndex) {

            const String WidgetName = v8_to_string (Args[WidgetIndex]);
            QWidget *widget = jsObject->get_qt_widget ();
            if (widget) {

               QWidget *child = widget->findChild<QWidget *>(WidgetName.get_buffer ());
               if (child) {

                  V8Value newObj = self->create_v8_widget (child);
                  jsObject = self->_to_js_qt_object (newObj);
               }
               else { self->_log.warn << "lookup failed to find: " << WidgetName << endl; }
            }
         }

         const Handle Obs = self->_state.core->get_instance_handle (src);

         if (Obs && jsObject) {

            if (jsObject->bind (Signal, src, func)) {

               ObsStruct *os = self->_obsTable.lookup (Obs);
               if (!os) {

                  os = new ObsStruct (Obs);
                  if (!self->_obsTable.store (Obs, os)) { delete os; os = 0; }
               }

               if (os) { os->list.append (jsObject); }
            }
         }

         result = func;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_property (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget && (Args.Length () >= 1)) {

         const String Name = v8_to_string (Args[0]);

         if (Name && (Args.Length () >= 2)) {

            QVariant inValue = to_qt_variant (Args[1]);
            if (inValue.isValid ()) { widget->setProperty (Name.get_buffer (), inValue); }
         }

         QVariant prop = widget->property (Name.get_buffer ());
         V8Value outValue = to_v8_value (prop);
         if (!outValue.IsEmpty ()) { result = outValue; }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_show (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (widget) { widget->show (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qt_widget (Args.This ());
      if (Args.Length () > 0) {

         QLayout *layout = self->_to_qt_layout (Args[0]);
         if (layout) {

            widget->setLayout (layout);
         }
      }
      else {

         result = self->create_v8_layout (widget->layout ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_widget () {

   v8::HandleScope scope;

   _widgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _widgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _widgetTemp->PrototypeTemplate ();
   proto->Set ("className", v8::FunctionTemplate::New (_widget_class_name, _self));
   proto->Set ("objectName", v8::FunctionTemplate::New (_widget_object_name, _self));
   proto->Set ("close", v8::FunctionTemplate::New (_widget_close, _self));
   proto->Set ("enabled", v8::FunctionTemplate::New (_widget_enabled, _self));
   proto->Set ("lookup", v8::FunctionTemplate::New (_widget_lookup, _self));
   proto->Set ("show", v8::FunctionTemplate::New (_widget_show, _self));
   proto->Set ("hide", v8::FunctionTemplate::New (_widget_hide, _self));
   proto->Set ("parent", v8::FunctionTemplate::New (_widget_parent, _self));
   proto->Set ("observe", v8::FunctionTemplate::New (_widget_observe, _self));
   proto->Set ("layout", v8::FunctionTemplate::New (_widget_layout, _self));
   proto->Set ("property", v8::FunctionTemplate::New (_widget_property, _self));
}

