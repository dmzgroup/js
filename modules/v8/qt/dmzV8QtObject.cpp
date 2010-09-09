#include "dmzV8QtObject.h"
#include <QtGui/QWidget>

#include <QtCore/QDebug>


dmz::V8QtObject::V8QtObject (QWidget *widget, JsModuleUiV8QtBasic::State *state) :
      QObject (0),
      _widget (widget),
      _state (state) {;}


dmz::V8QtObject::~V8QtObject () {
   
   if (_widget) {
      
      if (!_widget->parentWidget ()) { delete _widget; }
      _widget = 0;
   }

   _obsTable.empty ();
}


QWidget *
dmz::V8QtObject::get_qt_widget () const {
   
   return _widget;
}


void
dmz::V8QtObject::add_callback (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   ObsStruct *os = _obsTable.lookup (Signal);

   if (Signal && !os) {
      
      os = new ObsStruct;

      if (os && !_obsTable.store (Signal, os)) { delete os; os = 0; }
   }
   
   if (os) {

      CallbackStruct *cs = new CallbackStruct;
      cs->self = V8ObjectPersist::New (Self);
      cs->func = V8FunctionPersist::New (Func);

      cs->next = os->list;
      os->list = cs;
   }
}


dmz::Boolean
dmz::V8QtObject::bind (QWidget *sender, const String &Signal) {

   return False;
}
