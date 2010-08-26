#include "dmzV8QtObject.h"
#include <QtGui/QWidget>


dmz::V8QtObject::V8QtObject (QWidget *widget, QObject *parent) :
      QObject (parent),
      _widget (widget) {;}


dmz::V8QtObject::~V8QtObject () {
   
   if (_widget) {
      
      if (!_widget->parentWidget ()) { delete _widget; }
      _widget = 0;
   }
   
   qDeleteAll (_cbList.begin (), _cbList.end ());
   _cbList.clear ();
}


QWidget *
dmz::V8QtObject::get_qt_widget () const {
   
   return _widget;
}


void
dmz::V8QtObject::add_callback (const V8Object &Self, const V8Function &Func) {

   CallbackStruct *cbs = new CallbackStruct ();
   cbs->self = V8ObjectPersist::New (Self);
   cbs->func = V8FunctionPersist::New (Func);
   _cbList.append (cbs);
}


dmz::Boolean
dmz::V8QtObject::bind (QWidget *sender, const String &Signal) {

   return False;
}
