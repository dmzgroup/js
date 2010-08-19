#include "dmzJsModuleUiV8QtBasicWidgets.h"
#include <QtGui/QtGui>

#include <QtCore/QDebug>


dmz::V8QtObserver::V8QtObserver (
      const V8Object &Self,
      const V8Function &Func,
      QWidget *widget,
      QObject *parent) :
      QObject (parent),
      _widget (widget),
      _self (Self),
      _func (Func) {;}


dmz::V8QtObserver::~V8QtObserver () {
   
   if (!_func.IsEmpty ()) { _func.Dispose (); _func.Clear (); }
   if (!_self.IsEmpty ()) { _self.Dispose (); _self.Clear (); }
}


void
dmz::V8QtObserver::observe () {

   if (!_func.IsEmpty ()) {
      
      v8::HandleScope scope;
      
      V8Value argv[] = { _self };
      
      v8::TryCatch tc;

      qDebug () << "!!!!!!!!!!dmz::V8QtObserver::observe!!!!!!!!!!!";

      // _func->Call (_self, 1, argv);
      
      if (tc.HasCaught ()) {
         
         if (_core) { _core->handle_v8_exception (tc); }
         
         // Handle exception here
         // handle_v8_exception (tc);
      }
   }
}   


dmz::V8QtBaseWidget::V8QtBaseWidget (QWidget *widget) :
      _widget (widget) {;}


dmz::V8QtBaseWidget::~V8QtBaseWidget () {;}


dmz::V8QtButton::V8QtButton (QWidget *widget, QObject *parent) :
      QObject (parent),
      V8QtBaseWidget (widget) {;}


dmz::V8QtButton::~V8QtButton () {;}


void
dmz::V8QtButton::slot_clicked () {

   qDebug () << "dmz::V8QtButton::slot_clicked!!!!!!!!!!!";
}   

