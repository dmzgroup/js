#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtGui/QFileDialog>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_file_dialog_get_existing_directory (
       const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      V8Object params = v8_to_object (Args[0]);

      if (Args.Length () >= 2) {

         parent = self->_to_qwidget (Args[1]);
      }

      if (!params.IsEmpty ()) {

         QString caption, dir, filter;
         Boolean allowMultiple (False);

         self->_get_file_dialog_params (params, caption, dir, filter, allowMultiple);

         QString dirName = QFileDialog::getExistingDirectory (parent, caption, dir);
         if (!dirName.isEmpty ()) {

            result = qstring_to_v8 (dirName);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_file_dialog_get_open_file_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      V8Object params = v8_to_object (Args[0]);

      if (Args.Length () >= 2) {

         parent = self->_to_qwidget (Args[1]);
      }

      if (!params.IsEmpty ()) {

         QString caption, dir, filter;
         Boolean allowMultiple (False);

         self->_get_file_dialog_params (params, caption, dir, filter, allowMultiple);

         QStringList fileList;

         if (allowMultiple) {

            QStringList fileNames =
               QFileDialog::getOpenFileNames (parent, caption, dir, filter);

            fileList = fileNames;
         }
         else {

            QString fileName =
               QFileDialog::getOpenFileName (parent, caption, dir, filter);

            if (!fileName.isEmpty ()) { fileList.append (fileName); }
         }

         result = qstringlist_to_v8 (fileList);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_file_dialog_get_save_file_name (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      V8Object params = v8_to_object (Args[0]);

      if (Args.Length () >= 2) {

         parent = self->_to_qwidget (Args[1]);
      }

      if (!params.IsEmpty ()) {

         QString caption, dir, filter;
         Boolean allowMultiple (False);

         self->_get_file_dialog_params (params, caption, dir, filter, allowMultiple);

         QString fileName = QFileDialog::getSaveFileName (parent, caption, dir, filter);

         result = qstring_to_v8 (fileName);
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_get_file_dialog_params (
      V8Object params,
      QString &caption,
      QString &dir,
      QString &filter,
      Boolean &allowMultiple) {

   if (!params.IsEmpty ()) {

      if (params->Has (_captionStr)) {

         caption = v8_to_qstring (params->Get (_captionStr));
      }

      if (params->Has (_dirStr)) {

         dir = v8_to_qstring (params->Get (_dirStr));
      }

      if (params->Has (_filterStr)) {

         filter = v8_to_qstring (params->Get (_filterStr));
      }

      if (params->Has (_allowMultipleStr)) {

         allowMultiple = v8_to_boolean (params->Get (_allowMultipleStr));
      }
   }
}


void
dmz::JsModuleUiV8QtBasic::_init_file_dialog () {

   v8::HandleScope scope;

   _fileDialogApi.add_function (
      "getExistingDirectory",
      _file_dialog_get_existing_directory, _self);

   _fileDialogApi.add_function (
      "getOpenFileName",
      _file_dialog_get_open_file_name, _self);

   _fileDialogApi.add_function (
      "getSaveFileName",
      _file_dialog_get_save_file_name, _self);
}
