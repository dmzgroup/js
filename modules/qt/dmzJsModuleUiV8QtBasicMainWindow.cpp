#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzQtModuleMainWindow.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include "dmzV8QtObject.h"
#include "dmzV8QtUtil.h"
#include <QtCore/QFileInfo>
#include <QtGui/QAction>
#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>

namespace {

   static const dmz::String LocalSignalTriggered ("triggered");
};


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


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_add_menu (const v8::Arguments &Args) {

#if 0
   mainWindow.addMenu (self, "File", "Open", "shortcut", function () {...});
   mainWindow.addMenu (self, "File", "New", "new.png", "shortcut", function () {...});
   mainWindow.addMenu (
      self,
      "File",
      {text:"New", icon:"new.png", shortcut: "Ctrl+X"},
      function () {...});

#endif

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      if (module) {

         const UInt32 Argc (Args.Length());
         const UInt32 ImageIndex (Argc > 5 ? 3 : 0);
         const UInt32 FuncIndex (Argc - 1);
         const UInt32 ShortcutIndex (Argc > 4 ? (Argc - 2) : 0);

         V8Object src = v8_to_object (Args[0]);
         V8Function func = v8_to_function (Args[FuncIndex]);

         if (!src.IsEmpty () && !func.IsEmpty ()) {

            QAction *action = new QAction (module->get_qt_main_window ());

            const QString Text = v8_to_qstring (Args[2]);
            if (!Text.isEmpty ()) { action->setText (Text); }

            const QString Shortcut = v8_to_qstring (Args[ShortcutIndex]);
            if (!Shortcut.isEmpty ()) { action->setShortcut (Shortcut); }

            if (ImageIndex) {

               const QString File = v8_to_qstring (Args[ImageIndex]);
               QFileInfo fi (File);

               if (fi.exists ()) {

                  QIcon icon (fi.filePath ());
                  action->setIcon (icon);
               }
            }

            result = self->create_v8_qobject (action);
            V8QtObject *jsObject = self->_to_v8_qt_object (result);

            const Handle Obs =
                  self->_state.core ? self->_state.core->get_instance_handle (src) : 0;

            if (Obs && jsObject) {

               if (jsObject->bind (LocalSignalTriggered, src, func)) {

                  ObsStruct *os = self->_obsTable.lookup (Obs);
                  if (!os) {

                     os = new ObsStruct (Obs);
                     if (!self->_obsTable.store (Obs, os)) { delete os; os = 0; }
                  }

                  if (os) { os->list.append (jsObject); }

                  const String Menu = v8_to_string (Args[1]);

                  module->add_menu_action (Menu, action);
               }
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_add_separator (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      if (module && Args.Length ()) {

         const String Menu = v8_to_string (Args[0]);
         module->add_menu_separator (Menu);
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
    _mainWindowApi.add_function ("addDock", _main_window_add_dock_widget, _self);
    _mainWindowApi.add_function ("removeDock", _main_window_remove_dock_widget, _self);

    _mainWindowApi.add_function ("addMenu", _main_window_add_menu, _self);
    _mainWindowApi.add_function ("addSeparator", _main_window_add_separator, _self);
//    _mainWindowApi.add_function ("lookupMenu", _main_window_lookup_menu, _self);
//    _mainWindowApi.add_function ("addMenuAction", _main_window_add_menu_action, _self);
//    _mainWindowApi.add_function ("removeMenuAction", _main_window_remove_menu_action, _self);
}
