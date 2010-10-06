#include <dmzJsModuleV8.h>
#include "dmzV8QtDockWidget.h"
#include <QtGui/QWidget>


namespace {

   static const dmz::String LocalSignalTopLevelChanged ("topLevelChanged");
   static const dmz::String LocalSignalVisibilityChanged ("visibilityChanged");
};


dmz::V8QtDockWidget::V8QtDockWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtDockWidget::~V8QtDockWidget () {;}


dmz::Boolean
dmz::V8QtDockWidget::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalTopLevelChanged) {

         connect (
            _widget,
            SIGNAL (topLEvelChanged (bool)),
            SLOT (on_topLevelChanged (bool)),
            Qt::UniqueConnection);

         results = True;
      }

      else if (Signal == LocalSignalVisibilityChanged) {

         connect (
            _widget,
            SIGNAL (visibilityChanged (bool)),
            SLOT (on_visibilityChanged (bool)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtDockWidget::on_topLevelChanged (bool topLevel) {

   _do_callback (LocalSignalTopLevelChanged, topLevel);
}


void
dmz::V8QtDockWidget::on_visibilityChanged (bool visible) {

   _do_callback (LocalSignalVisibilityChanged, visible);
}

