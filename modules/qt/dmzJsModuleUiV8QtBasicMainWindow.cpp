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

               if (self->_centralWidget) {

                  mainWindow->setCentralWidget (0);
                  self->_centralWidget->setParent (0);
               }

               widget->setParent (mainWindow);
               mainWindow->setCentralWidget (widget);
               self->_centralWidget = widget;
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
         V8Object params;

         Qt::DockWidgetArea area (Qt::LeftDockWidgetArea);
         if (Args.Length () > 1) {

            if (v8_is_object (Args[1])) { params = v8_to_object (Args[1]); }
            else { area = (Qt::DockWidgetArea)v8_to_uint32 (Args[1]); }
         }

         QWidget *widget = Args.Length () > 2 ? self->_to_qwidget (Args[2]) : 0;

         if (name) {

            // we need the area out of the params for the create call
            if (!params.IsEmpty ()) {

               if (params->Has (self->_areaStr)) {

                  area = (Qt::DockWidgetArea)v8_to_uint32 (params->Get (self->_areaStr));
               }
            }

            QDockWidget *dock = module->create_dock_widget (name, area, widget);
            if (dock) {

               // setup dock based on passed in params

               if (!params.IsEmpty ()) {

                  if (params->Has (self->_floatingStr)) {

                     dock->setFloating (
                        v8_to_boolean (params->Get (self->_floatingStr)));
                  }

                  if (params->Has (self->_visibleStr)) {

                     dock->setVisible (v8_to_boolean (params->Get (self->_visibleStr)));
                  }

                  if (params->Has (self->_allowedAreasStr)) {

                     V8Array array = v8_to_array (params->Get (self->_allowedAreasStr));
                     if (!array.IsEmpty ()) {

                        Qt::DockWidgetAreas areas;
                        const uint32_t Length = array->Length ();
                        for (uint32_t ix = 0; ix < Length; ix++) {

                           const UInt32 Value = v8_to_uint32 (
                              array->Get (v8::Integer::NewFromUnsigned (ix)));

                           areas |= (Qt::DockWidgetArea)Value;
                        }

                        dock->setAllowedAreas (areas);
                     }
                  }

                  if (params->Has (self->_featuresStr)) {

                     V8Array array = v8_to_array (params->Get (self->_featuresStr));
                     if (!array.IsEmpty ()) {

                        QDockWidget::DockWidgetFeatures features;
                        const uint32_t Length = array->Length ();
                        for (uint32_t ix = 0; ix < Length; ix++) {

                           const UInt32 Value = v8_to_uint32 (
                              array->Get (v8::Integer::NewFromUnsigned (ix)));

                           features |= (QDockWidget::DockWidgetFeature)Value;
                        }

                        dock->setFeatures (features);
                     }
                  }
               }

               // _mainWindowState was saved on the last JsExtV8::Shutdown
               // restoring state now will make sure dock window shows in the right
               // place
               // NOTE: this might have some side effects depending on when the dock is
               // created if the dock is created from some kind of user interaction then
               // the state of the main window could be out of sync
               QMainWindow *mainWindow = module->get_qt_main_window ();
               if (!self->_mainWindowState.isEmpty () && mainWindow) {

                  mainWindow->restoreState (self->_mainWindowState);
               }

               self->_dockList.append (name);
               result = self->create_v8_qwidget (dock);
            }
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

         String name = v8_to_string (Args[0]);
         if (name) {

            self->_dockList.removeAll (name);
            Boolean res = module->remove_dock_widget (name);
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

         V8Object src = v8_to_object (Args[0]);
         const QString Menu = v8_to_qstring (Args[1]);
         const QString Text = v8_to_qstring (Args[2]);
         QKeySequence shortcut;

         V8Object params;
         if (Args.Length () > 4) {

            params = v8_to_object (Args[3]);

            if (v8_is_object (Args[3])) { params = v8_to_object (Args[3]); }
            else { shortcut = v8_to_qkeysequence (Args[3]); }
         }

         V8Function func = v8_to_function (Args[Args.Length () - 1]);

         if (!src.IsEmpty () && !func.IsEmpty ()) {

            QAction *action = 0;
            const QString Key = Menu + Text;

            if (self->_menuActionMap.contains (Key)) {

               action = self->_menuActionMap[Key];
            }

            if (!action) { action = new QAction (Text, module->get_qt_main_window ()); }

            if (!params.IsEmpty ()) {

               if (params->Has (self->_iconStr)) {

                  const QString File = v8_to_qstring (params->Get (self->_iconStr));
                  QFileInfo fi (File);

                  if (fi.exists ()) {

                     QIcon icon (fi.filePath ());
                     action->setIcon (icon);
                  }
               }

               if (params->Has (self->_shortcutStr)) {

                  shortcut = v8_to_qkeysequence (params->Get (self->_shortcutStr));
               }
            }

            if (!shortcut.isEmpty ()) { action->setShortcut (shortcut); }

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

                  if (!self->_menuActionMap.contains (Key)) {

                     module->add_menu_action (qPrintable (Menu), action);
                     self->_menuActionMap[Key] = action;
                  }
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


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_main_window_window (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QtModuleMainWindow *module (self->_state.mainWindowModule);
      QMainWindow *mainWindow = module ? module->get_qt_main_window () : 0;
      if (mainWindow) {

         result = self->create_v8_qwidget (mainWindow->window ());
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_main_window () {

   v8::HandleScope scope;

   // enum QMainWindow::DockOption
   _mainWindowApi.add_constant (
      "AllowNestedDocks",
      (UInt32)QMainWindow::AllowNestedDocks);

   _mainWindowApi.add_constant (
      "AllowTabbedDocks",
      (UInt32)QMainWindow::AllowTabbedDocks);

   _mainWindowApi.add_constant (
      "ForceTabbedDocks",
      (UInt32)QMainWindow::ForceTabbedDocks);

   _mainWindowApi.add_constant ("VerticalTabs", (UInt32)QMainWindow::VerticalTabs);

   _mainWindowApi.add_function ("centralWidget", _main_window_central_widget, _self);
   _mainWindowApi.add_function ("close", _main_window_close, _self);
   _mainWindowApi.add_function ("createDock", _main_window_create_dock_widget, _self);
   _mainWindowApi.add_function ("removeDock", _main_window_remove_dock_widget, _self);

   _mainWindowApi.add_function ("addMenu", _main_window_add_menu, _self);
   _mainWindowApi.add_function ("addSeparator", _main_window_add_separator, _self);
//    _mainWindowApi.add_function ("lookupMenu", _main_window_lookup_menu, _self);
//    _mainWindowApi.add_function (
//       "addMenuAction",
//       _main_window_add_menu_action,
//       _self);
//    _mainWindowApi.add_function (
//       "removeMenuAction",
//       _main_window_remove_menu_action,
//       _self);

    _mainWindowApi.add_function ("window", _main_window_window, _self);
}
