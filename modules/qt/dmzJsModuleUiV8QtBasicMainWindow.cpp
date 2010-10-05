#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzQtModuleMainWindow.h>
#include <dmzRuntimeConfigToTypesBase.h>
// #include "dmzV8QtObject.h"
#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_central_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QMainWindow *mainWindow = self->get_qt_main_window ();
      if (mainWindow) {

         if (Args.Length ()) {

            QWidget *widget = self->_to_qt_widget (Args[0]);

            if (widget) {

               mainWindow->setCentralWidget (widget);
            }
         }

         QWidget *centralWidget = mainWindow->centralWidget ();
         if (centralWidget) {

            result = self->create_v8_widget (centralWidget);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_close (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QMainWindow *mainWindow = self->get_qt_main_window ();
      if (mainWindow) {

         bool res = mainWindow->close ();
         result = v8::Boolean::New (res);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_dock_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QMainWindow *mainWindow = self->get_qt_main_window ();
      QWidget *widget = self->_to_qt_widget (Args[0]);
      QDockWidget *dock = qobject_cast<QDockWidget *>(widget);

      if (mainWindow && dock) {

         mainWindow->addDockWidget (Qt::LeftDockWidgetArea, dock);
         result = v8::Boolean::New (True);
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_main_window () {

   v8::HandleScope scope;

   // enum QMainWindow::DockOption
   _mainWindowApi.add_constant ("AllowNestedDocks", (UInt32)QMainWindow::AllowNestedDocks);
   _mainWindowApi.add_constant ("AllowTabbedDocks", (UInt32)QMainWindow::AllowTabbedDocks);
   _mainWindowApi.add_constant ("ForceTabbedDocks", (UInt32)QMainWindow::ForceTabbedDocks);
   _mainWindowApi.add_constant ("VerticalTabs", (UInt32)QMainWindow::VerticalTabs);

    _mainWindowApi.add_function ("centralWidget", _main_window_central_widget, _self);
    _mainWindowApi.add_function ("close", _main_window_close, _self);
    _mainWindowApi.add_function ("dockWidget", _main_window_dock_widget, _self);
//    _mainWindowApi.add_function ("lookupMenu", _main_window_lookup_menu, _self);
//    _mainWindowApi.add_function ("addMenuAction", _main_window_add_menu_action, _self);
//    _mainWindowApi.add_function ("removeMenuAction", _main_window_remove_menu_action, _self);
}
