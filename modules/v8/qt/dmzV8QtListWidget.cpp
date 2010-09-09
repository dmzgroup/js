#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtListWidget.h"
#include <QtGui/QListWidget>

#include <QtCore/QDebug>


namespace {
   
   static const dmz::String LocalSignalCurrentItemChanged ("currentItemChanged");
   static const dmz::String LocalSignalItemActivated ("itemActivated");
};


dmz::V8QtListWidget::V8QtListWidget (QWidget *widget, JsModuleUiV8QtBasic::State *state) :
      V8QtObject (widget, state) {;}


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

   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;
      
      ObsStruct *os = _obsTable.lookup (LocalSignalCurrentItemChanged);
      if (os) {
      
         CallbackStruct *cs (os->list);
         while (cs) {
      
            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {
      
               const int Argc (3);
               V8Value argv[Argc];
               argv[0] = _state->ui->create_v8_list_widget_item (current);
               argv[1] = _state->ui->create_v8_list_widget_item (previous);
               argv[2] = cs->self;
      
               v8::TryCatch tc;
      
               cs->func->Call (cs->self, Argc, argv);
      
               if (tc.HasCaught ()) {
      
                  if (_state->core) { _state->core->handle_v8_exception (tc); }
               }
            }
      
            cs = cs->next;
         }
      }
   }
}


void
dmz::V8QtListWidget::on_itemActivated (QListWidgetItem *item) {
   
   if (_state && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;
   
      ObsStruct *os = _obsTable.lookup (LocalSignalItemActivated);
      if (os) {
   
         CallbackStruct *cs (os->list);
         while (cs) {
   
            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {
   
               const int Argc (2);
               V8Value argv[Argc];
               argv[0] = _state->ui->create_v8_list_widget_item (item);
               argv[1] = cs->self;
   
               v8::TryCatch tc;
   
               cs->func->Call (cs->self, Argc, argv);
   
               if (tc.HasCaught ()) {
   
                  if (_state->core) { _state->core->handle_v8_exception (tc); }
               }
            }
   
            cs = cs->next;
         }
      }
   }
}
