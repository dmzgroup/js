#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzQtModuleMainWindow.h>
#include "dmzV8QtObject.h"
#include <QtGui/QDockWidget>
#include <QtGui/QMainWindow>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_dock_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QtModuleMainWindow *module = self->_state.mainWindowModule;
      QMainWindow *mainWindow = module ? module->get_qt_main_window () : 0;

      QDockWidget *dock = new QDockWidget (mainWindow);

      String title;
      QWidget *widget;

      if (Args.Length () >= 2) {

         title = v8_to_string (Args[0]);
         widget = self->_to_qwidget (Args[1]);
      }
      else if (Args.Length ()){

         V8Value arg = Args[1];

         if (arg->IsString ()) { title = v8_to_string (arg); }
         else { widget = self->_to_qwidget (arg); }
      }

      if (title) {

         dock->setObjectName (title.get_buffer ());
         dock->setWindowTitle (title.get_buffer ());
      }

      if (widget) { dock->setWidget (widget); }

      result = self->create_v8_qwidget (dock);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_dock_widget () {

   v8::HandleScope scope;

   _dockWidgetApi.add_constant ("DockWidgetClosable", (UInt32)QDockWidget::DockWidgetClosable);
   _dockWidgetApi.add_constant ("DockWidgetMovable", (UInt32)QDockWidget::DockWidgetMovable);

   _dockWidgetApi.add_constant (
      "DockWidgetFloatable",
      (UInt32)QDockWidget::DockWidgetFloatable);

   _dockWidgetApi.add_constant (
      "DockWidgetVerticalTitleBar",
      (UInt32)QDockWidget::DockWidgetVerticalTitleBar);

   _dockWidgetApi.add_constant (
      "AllDockWidgetFeatures",
      (UInt32)QDockWidget::AllDockWidgetFeatures);

   _dockWidgetApi.add_constant (
      "NoDockWidgetFeatures",
      (UInt32)QDockWidget::NoDockWidgetFeatures);

   _dockWidgetApi.add_function ("create", _create_dock_widget, _self);

   _dockWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _dockWidgetTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _dockWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _dockWidgetTemp->PrototypeTemplate ();
//   proto->Set ("widget", v8::FunctionTemplate::New (_dock_widget_widget, _self));
}
