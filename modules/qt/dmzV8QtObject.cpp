#include <dmzJsModuleV8.h>
#include "dmzV8QtObject.h"
#include "dmzV8QtUtil.h"
#include <QtCore/QVariant>
#include <QtGui/QWidget>

#include <QtCore/QDebug>


dmz::V8QtObject::V8QtObject (
      const V8Object &Self,
      QObject *object,
      JsModuleUiV8QtBasicState *state) :
      QObject (0),
      _object (object),
      _state (state),
      _current (0) {

   if (!Self.IsEmpty ()) {

      Self->SetInternalField (0, v8::External::Wrap ((void *)this));
      self = V8ObjectPersist::New (Self);
   }
}


dmz::V8QtObject::~V8QtObject () {

   if (_object) {

      if (!_object->parent ()) { delete _object; }
      _object = 0;
   }

   _cbTable.empty ();

   self.Dispose (); self.Clear ();
}


QObject *
dmz::V8QtObject::get_qt_object () const {

   return _object;
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

   _do_callback (Signal, QVariant ());
}


void
dmz::V8QtObject::_do_callback (const String &Signal, const QVariant &Value) {

   if (_state) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;

      V8Value newValue = to_v8_value (Value);
      if (!newValue.IsEmpty ()) { args.append (newValue); }

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

            V8Value argv[Argc];

            for (int ix = 0; ix < (Argc - 2); ix++) {

               argv[ix] = ValueList.at (ix);
            }

            argv[Argc - 2] = _state->ui->create_v8_object (_object);
            argv[Argc - 1] = cs->self;

            v8::TryCatch tc;
            cs->func->Call (cs->self, Argc, argv);
            if (tc.HasCaught ()) { _handle_exception (Observer, tc); }
         }

         cs = _get_next_callback ();
      }
   }
}
