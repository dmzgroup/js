#include <dmzJsModuleV8.h>
#include "dmzV8QtObject.h"
#include <QtGui/QWidget>


dmz::V8QtObject::V8QtObject (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      QObject (0),
      _widget (widget),
      _state (state) {
         
   if (!Self.IsEmpty ()) {
   
      Self->SetInternalField (0, v8::External::Wrap ((void *)this));
      self = V8ObjectPersist::New (Self);
   }
}


dmz::V8QtObject::~V8QtObject () {
   
   if (_widget) {
      
      if (!_widget->parentWidget ()) { delete _widget; }
      _widget = 0;
   }

   _cbTable.empty ();
   
   self.Dispose (); self.Clear ();
}


QWidget *
dmz::V8QtObject::get_qt_widget () const {
   
   return _widget;
}


void
dmz::V8QtObject::register_callback (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   if (_state && _state->core) {
      
      CallbackTable *ct = _cbTable.lookup (Signal);

      if (Signal && !ct) { 
      
         ct = new CallbackTable (Signal);
         
         if (!_cbTable.store (Signal, ct)) { delete ct; ct = 0; }
      }

      const Handle ObsHandle = _state->core->get_instance_handle (Self);
      
      if (ct && ObsHandle) {
         
         CallbackStruct *cs = ct->table.lookup (ObsHandle);
         if (cs) {
         
            if (!(cs->func.IsEmpty ())) { cs->func.Dispose (); cs->func.Clear (); }
            cs->func = V8FunctionPersist::New (Func);
         }
         else {
            
            cs = new CallbackStruct (ObsHandle);
            cs->self = V8ObjectPersist::New (Self);
            cs->func = V8FunctionPersist::New (Func);
            
            if (!ct->table.store (ObsHandle, cs)) { delete cs; cs = 0; }
         }
      }
   }
}


void
dmz::V8QtObject::release_callback (const Handle Observer) {
   
}


dmz::Boolean
dmz::V8QtObject::bind (QWidget *sender, const String &Signal) {

   return False;
}

