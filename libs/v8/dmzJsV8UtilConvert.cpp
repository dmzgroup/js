#include <dmzJsV8UtilConvert.h>
#include <dmzTypesBase.h>
#include <dmzTypesHandleContainer.h>

dmz::V8Array
dmz::v8_to_array (HandleContainer &container) {

   v8::HandleScope scope;

   V8Array result = v8::Array::New ();

   HandleContainerIterator it;
   Handle value (0);
   int count (0);

   while (container.get_next (it, value)) {

      result->Set (v8::Integer::New (count), v8::Number::New ((double) value));
      count++;
   }

   return scope.Close (result);
}

