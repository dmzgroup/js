#include "dmzJsModuleUiV8QtBasicWidgets.h"
#include <QtGui/QtGui>

#include <QtCore/QDebug>


dmz::V8QtObject::V8QtObject (QWidget *widget) :
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


dmz::V8QtWidget::V8QtWidget (QWidget *widget, QObject *parent) :
      QObject (parent),
      V8QtObject (widget) {;}


dmz::V8QtWidget::~V8QtWidget () {;}


dmz::V8QtButton::V8QtButton (QWidget *widget, QObject *parent) :
      QObject (parent),
         V8QtObject (widget) {;}


dmz::V8QtButton::~V8QtButton () {;}


dmz::Boolean
dmz::V8QtButton::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);
   
   if (sender) {
      
      if (Signal == "clicked") {

         connect (
            sender,
            SIGNAL (clicked ()),
            SLOT (on_clicked ()),
            Qt::UniqueConnection);
            
         results = True;
      }
   }
   
   return results;
}


void
dmz::V8QtButton::on_clicked () {

   v8::HandleScope scope;
   
   foreach (CallbackStruct *cbs, _cbList) {
      
      if (!cbs->func.IsEmpty () && !cbs->self.IsEmpty ()) {

         // V8Object localSelf = v8::Local<v8::Object>::New (cbs->self);
         // V8Function localFunc = v8::Local<v8::Function>::New (cbs->func);

         V8Value argv[] = { cbs->self };
         // V8Value argv[] = { localSelf };
         
         v8::TryCatch tc;
         
         cbs->func->Call (cbs->self, 1, argv);
         // localFunc->Call (localSelf, 1, argv);

         if (tc.HasCaught ()) {

            // if (_core) { _core->handle_v8_exception (tc); }
         }
      }
   }
}   

