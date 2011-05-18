
#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtCore/QCryptographicHash>


namespace {

void local_crypto_hash_delete (v8::Persistent<v8::Value> object, void *param) {

   if (param) {

      QCryptographicHash *ptr = (QCryptographicHash *)param;
      delete ptr; ptr = 0;
   }

   object.Dispose (); object.Clear ();
}

};


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_crypto (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QCryptographicHash *hash (0);
      if (Args.Length () && Args[0]->IsString ()) {

         hash = new QCryptographicHash((QCryptographicHash::Algorithm)v8_to_uint32 (Args[0]));
      }
      result = self->create_v8_crypto_hash (hash);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_crypto_hash (QCryptographicHash *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj;
      if (!_cryptoCtor.IsEmpty ()) { obj = _cryptoCtor->NewInstance (); }
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;

         v8::Persistent<v8::Value> persist = v8::Persistent<v8::Value>::New (result);
         persist.MakeWeak ((void *)value, local_crypto_hash_delete);
      }
   }

   return scope.Close (result);
}


QCryptographicHash *
dmz::JsModuleUiV8QtBasic::_to_crypto_hash (V8Value value) {

   v8::HandleScope scope;
   QCryptographicHash *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_cryptoTemp->HasInstance (obj)) {

         result = (QCryptographicHash *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }
   return result;
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_crypto_add_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QCryptographicHash *hash = self->_to_crypto_hash (Args.This ());
      if (hash && (Args.Length () > 1) && Args[0]->IsString()) {

         QString data = v8_to_qstring (Args[0]);
         hash->addData (data.toLocal8Bit ());
      }
   }
   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_crypto_reset (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QCryptographicHash *hash = self->_to_crypto_hash (Args.This ());
      if (hash) { hash->reset (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_crypto_result (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QCryptographicHash *hash = self->_to_crypto_hash (Args.This ());
      if (hash) { result = v8::String::New (qPrintable(QString(hash->result ().toHex ()))); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_crypto_hash (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      if ((Args.Length () > 1) && Args[0]->IsString () && Args[1]->IsNumber ()) {

         QByteArray ba =
            QCryptographicHash::hash (
               v8_to_qstring (Args[0]).toLocal8Bit (),
               (QCryptographicHash::Algorithm)v8_to_uint32 (Args[1]));

         result = v8::String::New (qPrintable(QString(ba.toHex ())));
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_crypto () {

   v8::HandleScope scope;

   _cryptoTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _cryptoTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _cryptoTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _cryptoTemp->PrototypeTemplate ();
   proto->Set ("addData", v8::FunctionTemplate::New (_crypto_add_data, _self));
   proto->Set ("reset", v8::FunctionTemplate::New (_crypto_reset, _self));
   proto->Set ("result", v8::FunctionTemplate::New (_crypto_result, _self));

   _cryptoApi.add_function ("hash", _crypto_hash, _self);

   // enum QCryptographicHash::Algorithm
   _cryptoApi.add_constant ("Md4", (UInt32)QCryptographicHash::Md4);
   _cryptoApi.add_constant ("Md5", (UInt32)QCryptographicHash::Md5);
   _cryptoApi.add_constant ("Sha1", (UInt32)QCryptographicHash::Sha1);
}
