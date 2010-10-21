#include <dmzJsModuleV8.h>
#include "dmzV8QtDateTimeEdit.h"
#include <dmzV8QtUtil.h>
#include <QtGui/QDateTimeEdit>

namespace {

   static const dmz::String DateTimeChanged ("dateTimeChanged");
};


dmz::V8QtDateTimeEdit::V8QtDateTimeEdit (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtDateTimeEdit::~V8QtDateTimeEdit () {;}


dmz::Boolean
dmz::V8QtDateTimeEdit::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == DateTimeChanged) {

         connect (
            _widget,
            SIGNAL (dateTimeChanged (const QDateTime &)),
            SLOT (on_date_time_changed (const QDateTime &)),
            Qt::UniqueConnection);

         results = True;
      }

   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtDateTimeEdit::on_date_time_changed (const QDateTime &value) {

   if (_state) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;
      QList<V8Value> args;
      args.append (qdatetime_to_v8 (value));
      _do_callback (DateTimeChanged, args);
   }
}
