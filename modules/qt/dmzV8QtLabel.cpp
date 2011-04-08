#include <dmzJsModuleV8.h>
#include "dmzV8QtLabel.h"
#include <QtGui/QWidget>


namespace {

   static const dmz::String LocalSignalLinkActivated("linkActivated");
};


dmz::V8QtLabel::V8QtLabel (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtLabel::~V8QtLabel () {;}


dmz::Boolean
dmz::V8QtLabel::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalLinkActivated) {

         connect (
            _widget,
            SIGNAL (linkActivated (const QString &)),
            SLOT (on_linkActivated (const QString &)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtLabel::on_linkActivated (const QString &Link) {

   _do_callback (LocalSignalLinkActivated, Link);
}
