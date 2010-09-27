#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtLCDNumber.h"
#include <QtGui/QLCDNumber>
#include <QtCore/QDebug>


dmz::V8QtLCDNumber::V8QtLCDNumber (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtLCDNumber::~V8QtLCDNumber () {;}


dmz::Boolean
dmz::V8QtLCDNumber::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   return False;
}
