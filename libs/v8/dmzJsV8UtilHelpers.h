#ifndef DMZ_JS_V8_UTIL_HELPERS_DOT_H
#define DMZ_JS_V8_UTIL_HELPERS_DOT_H

#include <dmzJsV8UtilExport.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzTypesBase.h>

#include <v8.h>

namespace dmz {

class DMZ_JS_V8_UTIL_LINK_SYMBOL V8InterfaceHelper {

   public:
      V8InterfaceHelper ();
      ~V8InterfaceHelper ();

      void clear ();
      void reset ();
      Boolean add_function (const String &Name, v8::InvocationCallback cb, V8Value data);
      Boolean add_constant (const String &Name, const Float64 Value);
      Boolean add_constant (const String &Name, const UInt32 Value);
      Boolean add_constant (const String &Name, const String Value);
      V8ObjectPersist get_new_instance ();

   protected:
      struct State;
      State &_state;
};

};

#endif // DMZ_JS_V8_UTIL_HELPERS_DOT_H
