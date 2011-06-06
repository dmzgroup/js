#include <dmzJsModuleV8.h>
#include "dmzV8QtObject.h"
#include "dmzV8QtUtil.h"
#include <QtCore/QDebug>

dmz::V8QtObject::V8QtObject (
      const V8Object &Self,
      QObject *object,
      JsModuleUiV8QtBasicState *state) :
      QObject (0),
      _object (object),
      _state (state),
      _current (0),
      _deleteObject (True) {

   setObjectName ("V8QtObject");
   if (!Self.IsEmpty ()) {

      Self->SetInternalField (0, v8::External::Wrap ((void *)this));
      self = V8ObjectPersist::New (Self);
   }
}


dmz::V8QtObject::~V8QtObject () {

   if (_object) {

      if (!_object->parent ()) {

         if (_deleteObject) { delete _object; }
      }
   }

   _object = 0;

   _cbTable.empty ();

   self.Dispose (); self.Clear ();
}


QObject *
dmz::V8QtObject::get_qobject () const {

   return _object;
}


dmz::Boolean
dmz::V8QtObject::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   return False;
}


void
dmz::V8QtObject::bind_event (const V8Object &Self, const V8Function &Func) {

   _register_event_callback (Self, Func);
}


void
dmz::V8QtObject::release_callback (const Handle Observer) {

   if (Observer && _state && _state->core) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      HashTableStringIterator it;
      CallbackTable *ct (0);

      while (_cbTable.get_next (it, ct)) {

         CallbackStruct *cs = ct->table.remove (Observer);
         if (cs) {

            delete cs; cs = 0;
         }
      }
   }
}


void
dmz::V8QtObject::set_delete_object (const Boolean Value) { _deleteObject = Value; }


void
dmz::V8QtObject::_register_callback (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   if (_state && _state->core) {

      CallbackTable *ct = _cbTable.lookup (Signal);

      if (Signal && !ct) {

         ct = new CallbackTable (Signal);

         if (!_cbTable.store (Signal, ct)) { delete ct; ct = 0; }
      }

      const Handle ObsHandle = _state->core->get_instance_handle (Self);

      if (ct && ObsHandle) {

         CallbackStruct *cs = ct->table.lookup (ObsHandle);
         if (cs) {

            if (!(cs->func.IsEmpty ())) { cs->func.Dispose (); cs->func.Clear (); }
            cs->func = V8FunctionPersist::New (Func);
         }
         else {

            cs = new CallbackStruct (ObsHandle);
            cs->self = V8ObjectPersist::New (Self);
            cs->func = V8FunctionPersist::New (Func);

            if (!ct->table.store (ObsHandle, cs)) { delete cs; cs = 0; }
         }
      }
   }
}


void
dmz::V8QtObject::_register_event_callback (
      const V8Object &Self,
      const V8Function &Func) {

   if (_state && _state->core) {

      const Handle ObsHandle = _state->core->get_instance_handle (Self);

      if (ObsHandle) {

//         CallbackStruct *cs = ct->table.lookup (ObsHandle);
         CallbackStruct *cs = _eventCallbackTable.lookup (ObsHandle);
         if (cs) {

            if (!(cs->func.IsEmpty ())) { cs->func.Dispose (); cs->func.Clear (); }
            cs->func = V8FunctionPersist::New (Func);
         }
         else {

            cs = new CallbackStruct (ObsHandle);
            cs->self = V8ObjectPersist::New (Self);
            cs->func = V8FunctionPersist::New (Func);

            if (!_eventCallbackTable.store (ObsHandle, cs)) { delete cs; cs = 0; }
         }
      }
   }
}


bool
dmz::V8QtObject::eventFilter (QObject *object, QEvent *event) {

   bool result = false;

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      HashTableHandleIterator it;
      CallbackStruct *cs = _eventCallbackTable.get_first (it);
      while (cs) {

         if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

            const Handle Observer = cs->Observer;

            const int Argc (2);

            V8Value *argv = new V8Value[Argc];

            argv[0] = _state->ui->create_v8_qobject (object);
            argv[1] = _state->ui->create_v8_qevent (event);

            v8::TryCatch tc;
            cs->func->Call (cs->self, Argc, argv);
            if (tc.HasCaught ()) { _handle_exception (Observer, tc); }

            delete []argv; argv = 0;
         }

         cs = _eventCallbackTable.get_next (it);
      }
   }

   return result;
}


dmz::V8QtObject::CallbackStruct *
dmz::V8QtObject::_get_first_callback (const String &Signal) {

   CallbackStruct *cs (0);
   _current = _cbTable.lookup (Signal);
   if (_current) { cs = _current->table.get_first (_it); }
   return cs;
}


dmz::V8QtObject::CallbackStruct *
dmz::V8QtObject::_get_next_callback () {

   CallbackStruct *cs (0);
   if (_current) { cs = _current->table.get_next (_it); }
   return cs;
}


void
dmz::V8QtObject::_handle_exception (const Handle Source, v8::TryCatch &tc) {

   if (_current && Source) {

      if (_state && _state->core) { _state->core->handle_v8_exception (Source, tc); }

      CallbackStruct *cs = _current->table.remove (Source);
      if (cs) { delete cs; cs = 0; }
   }
}


void
dmz::V8QtObject::_do_callback (const String &Signal) {

   QVariantList args;
   _do_callback (Signal, args);
}


void
dmz::V8QtObject::_do_callback (const String &Signal, const QVariant &Value) {

   QVariantList args;
   args.append (Value);
   _do_callback (Signal, args);
}


void
dmz::V8QtObject::_do_callback (const String &Signal, const QVariantList &ValueList) {

   if (_state) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;

      foreach (QVariant value, ValueList) {

         V8Value newValue = qvariant_to_v8 (value);
         if (!newValue.IsEmpty ()) { args.append (newValue); }
      }

      _do_callback (Signal, args);
   }
}


void
dmz::V8QtObject::_do_callback (const String &Signal, const QList<V8Value> &ValueList) {

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackStruct *cs = _get_first_callback (Signal);
      while (cs) {

         if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

            const Handle Observer = cs->Observer;

            const int Argc (ValueList.size () + 2);

            V8Value *argv = new V8Value[Argc];

            for (int ix = 0; ix < (Argc - 2); ix++) {

               argv[ix] = ValueList.at (ix);
            }

            argv[Argc - 2] = _state->ui->create_v8_qobject (_object);
            argv[Argc - 1] = cs->self;

            v8::TryCatch tc;
            cs->func->Call (cs->self, Argc, argv);
            if (tc.HasCaught ()) { _handle_exception (Observer, tc); }

            delete []argv; argv = 0;
         }

         cs = _get_next_callback ();
      }
   }
}


dmz::V8Value
dmz::V8QtObject::find_callback (const V8Object &Self, const String &Signal) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   if (_state && _state->core) {

      CallbackTable *ct = _cbTable.lookup (Signal);
      const Handle ObsHandle = _state->core->get_instance_handle (Self);

      if (Signal && ct && ObsHandle) {

         CallbackStruct *cs = ct->table.lookup (ObsHandle);
         if (cs) { result = cs->func; }
      }
   }

   return scope.Close (result);
}
