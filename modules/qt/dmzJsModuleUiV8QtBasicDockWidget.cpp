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


/*
QDockWidget *
dmz::JsModuleUiV8QtBasic::_create_dock_widget (V8Object params, QWidget *widget) {

   QDockWidget *dock (0);

   if (!params.IsEmpty ()) {

      QString title;
      if (params->Has (_titleStr)) {

         title = v8_to_string (params->Get (_textStr)).get_buffer ();
      }

      dock = new QDockWidget (title, widget);

      if (params->Has (_allowedAreasStr)) {

         V8Array areaArray = v8_to_array (params->Get (_allowedAreasStr));
         if (!areaArray.IsEmpty ()) {

            Qt::DockWidgetAreas areas;

            const uint32_t Length = areaArray->Length ();
            for (uint32_t ix = 0; ix < Length; ix++) {

               const UInt32 AreaValue =
                  v8_to_uint32 (areaArray->Get (v8::Integer::NewFromUnsigned (ix)));

               areas |= (Qt::DockWidgetArea)AreaValue;
            }

            dock->setAllowedAreas (areas);
         }
      }

      if (params->Has (_featuresStr)) {

         V8Array featureArray = v8_to_array (params->Get (_featuresStr));
         if (!featureArray.IsEmpty ()) {

            QDockWidget::DockWidgetFeatures features;

            const uint32_t Length = featureArray->Length ();
            for (uint32_t ix = 0; ix < Length; ix++) {

               const UInt32 FeatureValue =
                  v8_to_uint32 (featureArray->Get (v8::Integer::NewFromUnsigned (ix)));

               features |= (QDockWidget::DockWidgetFeature)FeatureValue;
            }

            dock->setFeatures (features);
         }
      }

      if (params->Has (_textStr)) {

         String text = v8_to_string (params->Get (_textStr));
         if (text) { dock->setText (text.get_buffer ()); }
      }

      if (params->Has (_infoTextStr)) {

         String infoText = v8_to_string (params->Get (_infoTextStr));
         if (infoText) { dock->setInformativeText (infoText.get_buffer ()); }
      }

      if (params->Has (_standardButtonsStr)) {

         V8Array buttonArray = v8_to_array (params->Get (_standardButtonsStr));
         if (!buttonArray.IsEmpty ()) {

            QMessageBox::StandardButtons buttons;
            const uint32_t Length = buttonArray->Length ();
            for (uint32_t ix = 0; ix < Length; ix++) {

               const UInt32 ButtonValue =
                  v8_to_uint32 (buttonArray->Get (v8::Integer::NewFromUnsigned (ix)));

               buttons |= (QMessageBox::StandardButton)ButtonValue;
            }

            dock->setStandardButtons (buttons);
         }
      }

      if (params->Has (_defaultButtonStr)) {

         const UInt32 ButtonValue = v8_to_uint32 (params->Get (_defaultButtonStr));
         dock->setDefaultButton ((QMessageBox::StandardButton)ButtonValue);
      }
   }

   return dock;
}
*/


void
dmz::JsModuleUiV8QtBasic::_init_dock_widget () {

   v8::HandleScope scope;

   _dockWidgetApi.add_constant ("DockWidgetClosable", (UInt32)QDockWidget::DockWidgetClosable);
   _dockWidgetApi.add_constant ("DockWidgetMovable", (UInt32)QDockWidget::DockWidgetMovable);
   _dockWidgetApi.add_constant ("DockWidgetFloatable", (UInt32)QDockWidget::DockWidgetFloatable);
   _dockWidgetApi.add_constant ("DockWidgetVerticalTitleBar", (UInt32)QDockWidget::DockWidgetVerticalTitleBar);
   _dockWidgetApi.add_constant ("AllDockWidgetFeatures", (UInt32)QDockWidget::AllDockWidgetFeatures);
   _dockWidgetApi.add_constant ("NoDockWidgetFeatures", (UInt32)QDockWidget::NoDockWidgetFeatures);

   _dockWidgetApi.add_function ("create", _create_dock_widget, _self);

   _dockWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _dockWidgetTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _dockWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _dockWidgetTemp->PrototypeTemplate ();
//   proto->Set ("widget", v8::FunctionTemplate::New (_dock_widget_widget, _self));
}
