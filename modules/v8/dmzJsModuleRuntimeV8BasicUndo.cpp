#include "dmzJsModuleRuntimeV8Basic.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>

#include <qdb.h>
static dmz::qdb out;

namespace {

static const char LocalState[] = "state";
static const char LocalNames[] = "names";

};

struct dmz::JsModuleRuntimeV8Basic::UndoStruct {

   V8FunctionPersist func;
   V8ObjectPersist self;

   ~UndoStruct () { func.Dispose (); func.Clear (); self.Dispose (); self.Clear (); }
};

// UndoObserver Interface
void
dmz::JsModuleRuntimeV8Basic::update_recording_state (
      const UndoRecordingStateEnum RecordingState,
      const UndoRecordingTypeEnum RecordingType,
      const UndoTypeEnum UndoType) {

}


void
dmz::JsModuleRuntimeV8Basic::update_current_undo_names (
      const String *NextUndoName,
      const String *NextRedoName) {

}


// Undo bindings
dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_is_nested_handle (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result;

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      const Handle UndoHandle = v8_to_handle (Args[0]);

      result = v8::Boolean::New (self->_undo.is_nested_handle (UndoHandle));
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


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self && self->_core && (Args.Length () > 2)) {

      V8Object obj = v8_to_object (Args[0]);
      const String Type = v8_to_string (Args[1]);
      V8Function func = v8_to_function (Args[2]);
      const Handle Instance = self->_core->get_instance_handle (obj);

      HashTableHandleTemplate<UndoStruct> &table =
         (Type == LocalNames ? self->_undoNamesTable : self->_undoStateTable);

      if (Instance && (func.IsEmpty () == false)) {

         UndoStruct *us = table.lookup (Instance);

         if (!us) {

            us = new UndoStruct;

            if (!table.store (Instance, us)) { delete us; us = 0; }
         }

         if (us) {

            us->self.Dispose (); us->self.Clear ();
            us->func.Dispose (); us->func.Clear ();

            us->self = V8ObjectPersist::New (obj);
            us->func = V8FunctionPersist::New (func);

            result = func;
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleRuntimeV8Basic::_undo_release (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleRuntimeV8Basic *self = to_self (Args);

   if (self) {

      
   }

   return scope.Close (result);
}


void
dmz::JsModuleRuntimeV8Basic::_init_undo () {

   v8::HandleScope scope;

   _undoApi.add_constant ("State", LocalState);
   _undoApi.add_constant ("Names", LocalNames);
   _undoApi.add_constant ("Undo", UInt32 (UndoTypeUndo));
	_undoApi.add_constant ("Redo", UInt32 (UndoTypeRedo));
	_undoApi.add_constant ("Start", UInt32 (UndoRecordingStateStart));
   _undoApi.add_constant ("Stop", UInt32 (UndoRecordingStateStop));
	_undoApi.add_constant ("Explicit", UInt32 (UndoRecordingTypeExplicit));
   _undoApi.add_constant ("Auto", UInt32 (UndoRecordingTypeAuto));
   _undoApi.add_function ("isNestedHandle", _undo_is_nested_handle, _self);
   _undoApi.add_function ("isInUndo", _undo_is_in_undo, _self);
   _undoApi.add_function ("isRecording", _undo_is_recording, _self);
   _undoApi.add_function ("getType", _undo_is_recording, _self);
   _undoApi.add_function ("doNext", _undo_do_next, _self);
   _undoApi.add_function ("startRecord", _undo_start_record, _self);
   _undoApi.add_function ("stopRecord", _undo_stop_record, _self);
   _undoApi.add_function ("abortRecord", _undo_abort_record, _self);
   _undoApi.add_function ("storeAction", _undo_store_action, _self);
   _undoApi.add_function ("observe", _undo_observe, _self);
   _undoApi.add_function ("release", _undo_release, _self);
}


void
dmz::JsModuleRuntimeV8Basic::_reset_undo () {

   _undoStateTable.empty ();
   _undoNamesTable.empty ();
}
