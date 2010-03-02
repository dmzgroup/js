#include <dmzJsV8UtilHelpers.h>
#include <dmzTypesHashTableStringTemplate.h>

using namespace dmz;

namespace {

struct InterfaceStruct {

   const String Name;
   V8FunctionTemplatePersist funcTemplate;
   InterfaceStruct (const String &TheName) : Name (TheName) {;}
   ~InterfaceStruct () { funcTemplate.Dispose (); }
};

};


struct dmz::V8InterfaceHelper::State {

   HashTableStringTemplate<InterfaceStruct> table;
   V8ObjectPersist obj;

   ~State () { table.empty (); obj.Dispose (); }
};

dmz::V8InterfaceHelper::V8InterfaceHelper () : _state (*(new State)) {;}


dmz::V8InterfaceHelper::~V8InterfaceHelper () { delete &_state; }


void
dmz::V8InterfaceHelper::clear () {

   _state.obj.Dispose ();
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

   _state.obj.Dispose (); _state.obj.Clear ();

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
