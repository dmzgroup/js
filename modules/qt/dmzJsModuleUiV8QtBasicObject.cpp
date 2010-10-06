#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtObject.h"
#include "dmzV8QtUtil.h"
#include <QtCore/QObject>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_object_class_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QObject *object = self->_to_qt_object (Args.This ());
      if (object) {

         result = v8::String::New (object->metaObject ()->className ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_object_lookup (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);

   if (self) {

      QObject *object = self->_to_qt_object (Args.This ());
      const String ObjectName = v8_to_string (Args[0]);

      if (object && ObjectName) {

         QObject *child = object->findChild<QObject *>(ObjectName.get_buffer ());

         if (child) {

            result = self->create_v8_object (child);
         }
         else {

            self->_log.warn << "lookup failed to find: " << ObjectName << endl;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_object_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QObject *object = self->_to_qt_object (Args.This ());
      if (object) {

         if (Args.Length ()) {

            const String Value = v8_to_string (Args[0]);
            object->setObjectName (Value.get_buffer ());
         }

         result = v8::String::New (qPrintable (object->objectName ()));
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_object_observe (const v8::Arguments &Args) {

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

            const String ObjectName = v8_to_string (Args[WidgetIndex]);
            QObject *object = jsObject->get_qt_object ();
            if (object) {

               QObject *child = object->findChild<QObject *>(ObjectName.get_buffer ());
               if (child) {

                  V8Value newObj = self->create_v8_object (child);
                  jsObject = self->_to_js_qt_object (newObj);
               }
               else { self->_log.warn << "lookup failed to find: " << ObjectName << endl; }
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
dmz::JsModuleUiV8QtBasic::_object_property (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QObject *object = self->_to_qt_object (Args.This ());
      if (object && (Args.Length () >= 1)) {

         const String Name = v8_to_string (Args[0]);

         if (Name && (Args.Length () >= 2)) {

            QVariant inValue = to_qt_variant (Args[1]);
            if (inValue.isValid ()) { object->setProperty (Name.get_buffer (), inValue); }
         }

         QVariant prop = object->property (Name.get_buffer ());
         V8Value outValue = to_v8_value (prop);
         if (!outValue.IsEmpty ()) { result = outValue; }
      }
   }

   return scope.Close (result);
}



void
dmz::JsModuleUiV8QtBasic::_init_object () {

   v8::HandleScope scope;

   _objectTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _objectTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _objectTemp->PrototypeTemplate ();
   proto->Set ("className", v8::FunctionTemplate::New (_object_class_name, _self));
   proto->Set ("lookup", v8::FunctionTemplate::New (_object_lookup, _self));
   proto->Set ("objectName", v8::FunctionTemplate::New (_object_name, _self));
   proto->Set ("observe", v8::FunctionTemplate::New (_object_observe, _self));
   proto->Set ("property", v8::FunctionTemplate::New (_object_property, _self));
}

