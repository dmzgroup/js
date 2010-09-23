#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtLabel.h"
#include <QtGui/QLabel>
#include <QtCore/QDebug>


dmz::V8QtLabel::V8QtLabel (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtLabel::~V8QtLabel () {;}


dmz::Boolean
dmz::V8QtLabel::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   return False;
}
