#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QFileDialog>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_file_dialog_get_existing_directory (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = 0;
      V8Object params;

      if (Args.Length () >= 2) {

         parent = self->_to_qwidget (Args[0]);
         params = v8_to_object (Args[1]);
      }
      else {

         params = v8_to_object (Args[0]);
      }

      if (!params.IsEmpty ()) {

         QString caption, dir, filter;
         Boolean allowMultiple (False);

         self->_get_file_dialog_params (params, caption, dir, filter, allowMultiple);

         QString dirName = QFileDialog::getExistingDirectory (parent, caption, dir);
         if (!dirName.isEmpty ()) {

            result = v8::String::New (qPrintable(dirName));
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
      V8Object params;

      if (Args.Length () >= 2) {

         parent = self->_to_qwidget (Args[0]);
         params = v8_to_object (Args[1]);
      }
      else {

         params = v8_to_object (Args[0]);
      }

      if (!params.IsEmpty ()) {

         QString caption, dir, filter;
         Boolean allowMultiple (False);

         self->_get_file_dialog_params (params, caption, dir, filter, allowMultiple);

         QStringList fileList;

         if (allowMultiple) {

            QStringList fileNames = QFileDialog::getOpenFileNames (parent, caption, dir, filter);
            fileList = fileNames;
         }
         else {

            QString fileName = QFileDialog::getOpenFileName (parent, caption, dir, filter);
            if (!fileName.isEmpty ()) { fileList.append (fileName); }
         }

         V8Array array = v8::Array::New (fileList.count ());
         Int32 count = 0;

         QStringList::Iterator it = fileList.begin ();
         while (it != fileList.end ()) {

            array->Set (v8::Integer::New (count++), v8::String::New (qPrintable(*it)));
             ++it;
         }

         result = array;
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
      V8Object params;

      if (Args.Length () >= 2) {

         parent = self->_to_qwidget (Args[0]);
         params = v8_to_object (Args[1]);
      }
      else {

         params = v8_to_object (Args[0]);
      }

      if (!params.IsEmpty ()) {

         QString caption, dir, filter;
         Boolean allowMultiple (False);

         self->_get_file_dialog_params (params, caption, dir, filter, allowMultiple);

         QString fileName = QFileDialog::getSaveFileName (parent, caption, dir, filter);

         result = v8::String::New (qPrintable (fileName));
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

         caption = v8_to_string (params->Get (_captionStr)).get_buffer ();
      }

      if (params->Has (_dirStr)) {

         dir = v8_to_string (params->Get (_dirStr)).get_buffer ();
      }

      if (params->Has (_filterStr)) {

         filter = v8_to_string (params->Get (_filterStr)).get_buffer ();
      }

      if (params->Has (_allowMultipleStr)) {

         allowMultiple = v8_to_boolean (params->Get (_allowMultipleStr));
      }
   }
}


void
dmz::JsModuleUiV8QtBasic::_init_file_dialog () {

   v8::HandleScope scope;

   _fileDialogApi.add_function ("getExistingDirectory", _file_dialog_get_existing_directory, _self);
   _fileDialogApi.add_function ("getOpenFileName", _file_dialog_get_open_file_name, _self);
   _fileDialogApi.add_function ("getSaveFileName", _file_dialog_get_save_file_name, _self);
}
