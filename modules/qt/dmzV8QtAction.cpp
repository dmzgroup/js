#include <dmzJsModuleV8.h>
#include "dmzV8QtAction.h"
//#include <QtGui/QWidget>


namespace {

   static const dmz::String LocalSignalToggled ("toggled");
   static const dmz::String LocalSignalTriggered ("triggered");
};


dmz::V8QtAction::V8QtAction (
      const V8Object &Self,
      QObject *object,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, object, state) {;}


dmz::V8QtAction::~V8QtAction () {;}


dmz::Boolean
dmz::V8QtAction::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_object) {

      if (Signal == LocalSignalToggled) {

         connect (
            _object,
            SIGNAL (toggled (bool)),
            SLOT (on_toggled (bool)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalTriggered) {

         connect (
            _object,
            SIGNAL (triggered (bool)),
            SLOT (on_triggered (bool)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtAction::on_toggled (bool checked) {

   _do_callback (LocalSignalToggled, checked);
}


void
dmz::V8QtAction::on_triggered (bool checked) {

//   _do_callback (LocalSignalTriggered, checked);
   _do_callback (LocalSignalTriggered);
}
