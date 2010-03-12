#include <dmzJsV8UtilHelpers.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <dmzTypesStringTokenizer.h>

using namespace dmz;

#include <qdb.h>
static qdb out;

namespace {

static const char ScopeChar = '.';

struct InterfaceStruct {

   const String Name;
   V8FunctionTemplatePersist funcTemplate;
   HashTableStringTemplate<InterfaceStruct> table;
   InterfaceStruct (const String &TheName) : Name (TheName) {;}
   ~InterfaceStruct () {

      funcTemplate.Dispose ();
      funcTemplate.Clear ();
      table.empty ();
   }
};

static void
local_get_scope (const String &Path, String &scope, String &name) {

   const Int32 Length (Path.get_length ());
   Boolean done (False);
   Int32 place = Length - 1;

   while (!done) {

      if (place < 0) { done = True; }
      else if (Path.get_char (place) == ScopeChar) {

         done = True;
      }
      else { place--; }
   }

   if (place >= 0) {

      scope = Path.get_sub (0, place - 1);
      name = Path.get_sub (place + 1);
   }
   else { name = Path; scope.flush (); }
}


static V8Value
local_get_number (v8::Local<v8::String> property, const v8::AccessorInfo &Info) {

   v8::HandleScope scope;
   V8Value result;

   Float64 *ptr = (Float64 *)v8::External::Unwrap (Info.Data ());

   if (ptr) { result = v8::Number::New (*ptr); }

   return result.IsEmpty () ? result : scope.Close (result);
}


static V8Value
local_get_string (v8::Local<v8::String> property, const v8::AccessorInfo &Info) {

   v8::HandleScope scope;
   V8Value result;

   String *ptr = (String *)v8::External::Unwrap (Info.Data ());

   if (ptr && *ptr) { result = v8::String::New (ptr->get_buffer ()); }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
local_set (
      v8::Local<v8::String> property,
      v8::Local<v8::Value> value,
      const v8::AccessorInfo &Info) {

   v8::HandleScope scope;
   String str ("Unable to set constant value: ");
   str << *(v8::String::AsciiValue (property));
   v8::ThrowException (v8::Exception::Error (v8::String::New (str.get_buffer ())));
}

};


struct dmz::V8InterfaceHelper::State {

   HashTableStringTemplate<InterfaceStruct> table;
   HashTableStringTemplate<String> strTable;
   HashTableStringTemplate<Float64> numTable;
   V8ObjectTemplatePersist objTemplate;
   V8ObjectPersist obj;

   State () {

      v8::HandleScope scope; 
      objTemplate = V8ObjectTemplatePersist::New (v8::ObjectTemplate::New ());
   }

   ~State () {

      table.empty (); 
      strTable.empty ();
      numTable.empty ();
      objTemplate.Dispose (); objTemplate.Clear ();
      obj.Dispose (); obj.Clear ();
   }
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
      const String &Path,
      v8::InvocationCallback cb,
      V8Value data) {

   v8::HandleScope v8Scope;
   Boolean result (False);

   String scope, name;

   local_get_scope (Path, scope, name);

   HashTableStringTemplate<InterfaceStruct> *table (&(_state.table));

   InterfaceStruct *foundIs (0);

   if (scope) {

      StringTokenizer stz (scope, ScopeChar);

      String token;

      while (table && stz.get_next (token)) {

         foundIs = table->lookup (token);
         if (foundIs) { table = &(foundIs->table); }
      }
   }

   InterfaceStruct *is = new InterfaceStruct (name);

   if (name && table && table->store (name, is)) {

      is->funcTemplate = V8FunctionTemplatePersist::New (
         v8::FunctionTemplate::New (cb, data));

      if (foundIs) {

         if (foundIs->funcTemplate.IsEmpty () == false) {

            foundIs->funcTemplate->Set (
               v8::String::NewSymbol (name.get_buffer ()),
               is->funcTemplate);

            result = True;
         }
      }
      else {

         if (_state.objTemplate.IsEmpty () == false) {

            _state.objTemplate->Set (
               v8::String::NewSymbol (name.get_buffer ()),
               is->funcTemplate);

            result = True;
         }
      }
   }
   else if (is) { delete is; is = 0; }

   return result;
}


dmz::Boolean
dmz::V8InterfaceHelper::add_constant (const String &Name, const Float64 Value) {

   v8::HandleScope scope;

   Boolean result (False);

   if (Name && !_state.numTable.lookup (Name)) {

      Float64 *ptr = new Float64 (Value);

      if (_state.numTable.store (Name, ptr)) {

         _state.objTemplate->SetAccessor (
            v8::String::NewSymbol (Name.get_buffer ()),
            local_get_number,
            local_set,
            v8::External::Wrap ((void *)ptr));

         result = True;
      }
      else { delete ptr; ptr = 0; }
   }

   return result;
}


dmz::Boolean
dmz::V8InterfaceHelper::add_constant (const String &Name, const UInt32 Value) {

   return add_constant (Name, Float64 (Value));
}


dmz::Boolean
dmz::V8InterfaceHelper::add_constant (const String &Name, const String Value) {

   v8::HandleScope scope;

   Boolean result (False);

   if (Name && !_state.strTable.lookup (Name)) {

      String *ptr = new String (Value);

      if (_state.strTable.store (Name, ptr)) {

         _state.objTemplate->SetAccessor (
            v8::String::NewSymbol (Name.get_buffer ()),
            local_get_string,
            local_set,
            v8::External::Wrap ((void *)ptr));

         result = True;
      }
      else { delete ptr; ptr = 0; }
   }
   return result;
}


dmz::V8ObjectPersist
dmz::V8InterfaceHelper::get_new_instance () {

   v8::HandleScope scope;

   clear ();

   _state.obj = V8ObjectPersist::New (_state.objTemplate->NewInstance ());

   return _state.obj;
}
