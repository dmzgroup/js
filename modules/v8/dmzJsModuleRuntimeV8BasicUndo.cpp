#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsV8UtilConvert.h>

#include <qdb.h>
static dmz::qdb out;


// Undo bindings
dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_is_nested_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Handle UndoHandle = v8_to_handle (Args[0]);

      result = v8::Boolean::New ((double)self->_undo.is_nested_handle (UndoHandle));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_is_in_undo (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      result = v8::Number::New ((double)self->_undo.is_in_undo ());
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_is_recording (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      result = v8::Number::New ((double)self->_undo.is_recording ());
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_get_type (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      result = v8::Number::New ((double)self->_undo.get_type ());
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_do_next (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const UInt32 Type = v8_to_uint32 (Args[0]);

      result = v8::Boolean::New (self->_undo.do_next (
         Type == UndoTypeUndo ? UndoTypeUndo : UndoTypeRedo));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_start_record (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const String Name = v8_to_string (Args[0]);

      result = v8::Number::New ((double)self->_undo.start_record (Name));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_stop_record (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Handle UndoHandle = v8_to_handle (Args[0]);

      result = v8::Boolean::New (self->_undo.stop_record (UndoHandle));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_abort_record (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Handle UndoHandle = v8_to_handle (Args[0]);

      result = v8::Boolean::New (self->_undo.abort_record (UndoHandle));
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_store_action (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Int32 Length = Args.Length ();

      Message *msg = self->_to_message_ptr (Args[0]);
      const UInt32 ObsHandle = (Length > 2) ? v8_to_handle (Args[1]) : 0;
      Data *data = self->_to_data_ptr (Args[(Length > 2) ? 2 : 1]);

      if (msg) {

         result = v8::Boolean::New (self->_undo.store_action (*msg, ObsHandle, data));
      }
   }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsModuleRuntimeV8Basic::_init_undo () {

   v8::HandleScope scope;

   _undoApi.add_constant ("UndoType", UInt32 (UndoTypeUndo));
   _undoApi.add_constant ("RedoType", UInt32 (UndoTypeRedo));
   _undoApi.add_function ("isNestedHandle", _undo_is_nested_handle, _self);
   _undoApi.add_function ("isInUndo", _undo_is_in_undo, _self);
   _undoApi.add_function ("isRecording", _undo_is_recording, _self);
   _undoApi.add_function ("getType", _undo_is_recording, _self);
   _undoApi.add_function ("doNext", _undo_do_next, _self);
   _undoApi.add_function ("startRecord", _undo_start_record, _self);
   _undoApi.add_function ("stopRecord", _undo_stop_record, _self);
   _undoApi.add_function ("abortRecord", _undo_abort_record, _self);
   _undoApi.add_function ("storeAction", _undo_store_action, _self);
}
