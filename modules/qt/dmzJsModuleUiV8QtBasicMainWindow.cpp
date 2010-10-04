#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzQtModuleMainWindow.h>
#include <dmzRuntimeConfigToTypesBase.h>
// #include "dmzV8QtObject.h"
#include <QtGui/QMainWindow>


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


void
dmz::JsModuleUiV8QtBasic::_init_main_window () {

   v8::HandleScope scope;

    _mainWindowApi.add_function ("centralWidget", _main_window_central_widget, _self);
    _mainWindowApi.add_function ("close", _main_window_close, _self);
}
