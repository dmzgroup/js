#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzQtModuleMainWindow.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_central_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      QMainWindow *mainWindow = module ? module->get_qt_main_window () : 0;
      if (mainWindow) {

         if (Args.Length ()) {

            QWidget *widget = self->_to_qwidget (Args[0]);

            if (widget) {

               widget->setParent (mainWindow);
               mainWindow->setCentralWidget (widget);
            }
         }

         QWidget *centralWidget = mainWindow->centralWidget ();
         if (centralWidget) {

            result = self->create_v8_qwidget (centralWidget);
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

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      QMainWindow *mainWindow = module ? module->get_qt_main_window () : 0;
      if (mainWindow) {

         bool res = mainWindow->close ();
         result = v8::Boolean::New (res);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_create_dock_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      if (module) {

         String name = v8_to_string (Args[0]);
         QWidget *widget = Args.Length () > 1 ? self->_to_qwidget (Args[1]) : 0;

         if (name) {

            QDockWidget *dock = module->create_dock_widget (name, widget);
            if (dock) { result = self->create_v8_qwidget (dock); }
         }
      }
   }

   return scope.Close (result);
}



dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_add_dock_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      if (module) {

         QDockWidget *dock (0);

         V8Value arg = Args[0];
         if (arg->IsString ()) {

            String name = v8_to_string (arg);
            dock = module->lookup_dock_widget (name);
         }
         else {

            QWidget *widget = self->_to_qwidget (arg);
            dock = qobject_cast<QDockWidget *>(widget);
         }

         if (dock) {

            Qt::DockWidgetArea area = Qt::RightDockWidgetArea;
            if (Args.Length () > 1) { area = (Qt::DockWidgetArea)v8_to_uint32 (Args[1]); }

            Boolean res = module->add_dock_widget (dock, area);
            result = v8::Boolean::New (res);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_remove_dock_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

self->_log.warn << "_main_window_remove_dock_widget" << endl;

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      if (module) {

         QDockWidget *dock (0);

         V8Value arg = Args[0];
         if (arg->IsString ()) {

            String name = v8_to_string (arg);
            self->_log.warn << "lookup dock: " << name << endl;
            dock = module->lookup_dock_widget (name);
         }
         else {

            QWidget *widget = self->_to_qwidget (arg);
            dock = qobject_cast<QDockWidget *>(widget);
         }

         if (dock) {

            Boolean res = module->remove_dock_widget (dock);
            result = v8::Boolean::New (res);
         }
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
    _mainWindowApi.add_function ("createDock", _main_window_create_dock_widget, _self);
//    _mainWindowApi.add_function ("updateDock", _main_window_update_dock_widget, _self);
    _mainWindowApi.add_function ("addDock", _main_window_add_dock_widget, _self);
    _mainWindowApi.add_function ("removeDock", _main_window_remove_dock_widget, _self);

//    _mainWindowApi.add_function ("lookupMenu", _main_window_lookup_menu, _self);
//    _mainWindowApi.add_function ("addMenuAction", _main_window_add_menu_action, _self);
//    _mainWindowApi.add_function ("removeMenuAction", _main_window_remove_menu_action, _self);
}
