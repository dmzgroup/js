#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtListWidget.h"
#include <QtGui/QListWidget>

#include <QtCore/QDebug>


namespace {
   
   static const dmz::String LocalSignalCurrentItemChanged ("currentItemChanged");
   static const dmz::String LocalSignalItemActivated ("itemActivated");
};


dmz::V8QtListWidget::V8QtListWidget (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtListWidget::~V8QtListWidget () {;}


dmz::Boolean
dmz::V8QtListWidget::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);
   
   if (sender) {
      
      if (Signal == LocalSignalCurrentItemChanged) {

         connect (
            sender,
            SIGNAL (currentItemChanged (QListWidgetItem *, QListWidgetItem *)),
            this,
            SLOT (on_currentItemChanged (QListWidgetItem *, QListWidgetItem *)),
            Qt::UniqueConnection);
            
         results = True;
      }
      else if (Signal == LocalSignalItemActivated) {
         
         connect (
            sender,
            SIGNAL (itemActivated (QListWidgetItem *)),
            this,
            SLOT (on_itemActivated (QListWidgetItem *)),
            Qt::UniqueConnection);
            
         results = True;
      }
   }
   
   if (!results) { results = V8QtObject::bind (sender, Signal); }
   
   return results;
}


void
dmz::V8QtListWidget::on_currentItemChanged (
      QListWidgetItem *current,
      QListWidgetItem *previous) {

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;
      
      CallbackTable *ct = _cbTable.lookup (LocalSignalCurrentItemChanged);
      if (ct) {
         
         HashTableHandleIterator it;
         CallbackStruct *cs (0);
         
         while (ct->table.get_next (it, cs)) {
      
            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {
      
               const Handle Observer = cs->Observer;
      
               const int Argc (3);
               V8Value argv[Argc];
               argv[0] = _state->ui->create_v8_list_widget_item (current);
               argv[1] = _state->ui->create_v8_list_widget_item (previous);
               argv[2] = cs->self;
      
               v8::TryCatch tc;
      
               cs->func->Call (cs->self, Argc, argv);
      
               if (tc.HasCaught ()) {
      
                  _state->core->handle_v8_exception (Observer, tc);
                  
                  cs = ct->table.remove (Observer);
                  
                  if (cs) { delete cs; cs = 0; }
               }
            }
         }
      }
   }
}


void
dmz::V8QtListWidget::on_itemActivated (QListWidgetItem *item) {
   
   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;
   
      CallbackTable *ct = _cbTable.lookup (LocalSignalItemActivated);
      if (ct) {
         
         HashTableHandleIterator it;
         CallbackStruct *cs (0);
         
         while (ct->table.get_next (it, cs)) {
   
            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {
   
               const Handle Observer = cs->Observer;
   
               const int Argc (2);
               V8Value argv[Argc];
               argv[0] = _state->ui->create_v8_list_widget_item (item);
               argv[1] = cs->self;
   
               v8::TryCatch tc;
   
               cs->func->Call (cs->self, Argc, argv);
   
               if (tc.HasCaught ()) {
   
                  _state->core->handle_v8_exception (Observer, tc);
                  
                  cs = ct->table.remove (Observer);
                  
                  if (cs) { delete cs; cs = 0; }
               }
            }
         }
      }
   }
}
