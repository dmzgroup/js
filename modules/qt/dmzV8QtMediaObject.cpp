#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtMediaObject.h"

#include <phonon/MediaObject>


namespace {

   static const dmz::String FinishedSignal ("finished");
   static const dmz::String TickSignal ("tick");
   static const dmz::String HasVideoChangedSignal ("hasVideoChanged");
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
      }
      else if (Signal == HasVideoChangedSignal) {

         connect (
            _object,
            SIGNAL (hasVideoChanged (bool)),
            SLOT (on_has_video_changed (bool)),
            Qt::UniqueConnection);
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
