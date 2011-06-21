#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtMediaObject.h"

#include <qdb.h>

namespace {

   static const dmz::String FinishedSignal ("finished");
   static const dmz::String TickSignal ("tick");
   static const dmz::String HasVideoChangedSignal ("hasVideoChanged");
   static const dmz::String StateChanged ("stateChanged");
};


dmz::V8QtMediaObject::V8QtMediaObject (
      const V8Object &Self,
      QObject *object,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, object, state) {;}


dmz::V8QtMediaObject::~V8QtMediaObject () {;}


dmz::Boolean
dmz::V8QtMediaObject::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_object) {

      if (Signal == FinishedSignal) {

         connect (
            _object,
            SIGNAL (finished ()),
            SLOT (on_finished ()),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == TickSignal) {

         connect (
            _object,
            SIGNAL (tick ()),
            SLOT (on_tick ()),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == HasVideoChangedSignal) {

         connect (
            _object,
            SIGNAL (hasVideoChanged (bool)),
            SLOT (on_has_video_changed (bool)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == StateChanged) {

         connect (
            _object,
            SIGNAL (stateChanged (Phonon::State, Phonon::State)),
            SLOT (on_state_changed (Phonon::State, Phonon::State)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtMediaObject::on_tick () {

   _do_callback (TickSignal);
}


void
dmz::V8QtMediaObject::on_finished () {

   _do_callback (FinishedSignal);
}

void
dmz::V8QtMediaObject::on_has_video_changed (bool b) {

   _do_callback (HasVideoChangedSignal, b);
}

void
dmz::V8QtMediaObject::on_state_changed (Phonon::State newstate, Phonon::State oldstate) {

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      QList<V8Value> args;
      args.append (v8::Number::New ((UInt32)newstate));
      args.append (v8::Number::New ((UInt32)oldstate));

      _do_callback (StateChanged, args);
   }
}
