#include <dmzJsV8UtilHelpers.h>
#include <dmzTypesHashTableStringTemplate.h>

using namespace dmz;

namespace {

struct InterfaceStruct {

   const String Name;
   V8FunctionTemplatePersist funcTemplate;
   InterfaceStruct (const String &TheName) : Name (TheName) {;}
   ~InterfaceStruct () { funcTemplate.Dispose (); funcTemplate.Clear (); }
};

};


struct dmz::V8InterfaceHelper::State {

   HashTableStringTemplate<InterfaceStruct> table;
   V8ObjectPersist obj;

   ~State () { table.empty (); obj.Dispose (); obj.Clear (); }
};

dmz::V8InterfaceHelper::V8InterfaceHelper () : _state (*(new State)) {;}


dmz::V8InterfaceHelper::~V8InterfaceHelper () { reset (); delete &_state; }


void
dmz::V8InterfaceHelper::clear () {

   if (_state.obj.IsEmpty () == false) {

      v8::HandleScope scope;

      HashTableStringIterator it;
      InterfaceStruct *current (0);

      while (_state.table.get_next (it, current)) {

         _state.obj->Set (
            v8::String::NewSymbol (current->Name.get_buffer ()),
            v8::Undefined ());
      }

      _state.obj.Dispose (); _state.obj.Clear ();
   }
}


void
dmz::V8InterfaceHelper::reset () {

   clear ();
   _state.table.empty ();
}


dmz::Boolean
dmz::V8InterfaceHelper::add_function (
      const String &Name,
      v8::InvocationCallback cb,
      V8Value data) {

   v8::HandleScope scope;
   Boolean result (False);

   InterfaceStruct *is = new InterfaceStruct (Name);

   if (_state.table.store (Name, is)) {

      is->funcTemplate = V8FunctionTemplatePersist::New (
         v8::FunctionTemplate::New (cb, data));

      result = True;
   }
   else if (is) { delete is; is = 0; }

   return result;
}


dmz::V8ObjectPersist
dmz::V8InterfaceHelper::get_new_instance () {

   v8::HandleScope scope;

   clear ();

   _state.obj = V8ObjectPersist::New (v8::Object::New ());
 
   HashTableStringIterator it;
   InterfaceStruct *current (0);

   while (_state.table.get_next (it, current)) {

      _state.obj->Set (
         v8::String::NewSymbol (current->Name.get_buffer ()),
         current->funcTemplate->GetFunction ());
   }

   return _state.obj;
}
