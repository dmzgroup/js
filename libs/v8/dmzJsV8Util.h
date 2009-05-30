#ifndef DMZ_JS_V8_UTIL_DOT_H
#define DMZ_JS_V8_UTIL_DOT_H

#include <dmzJsV8UtilExport.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

#include <v8.h>

namespace dmz {

class DMZ_JS_V8_UTIL_LINK_SYMBOL V8ScriptBuffer :
      public v8::String::ExternalAsciiStringResource {

   public:
      V8ScriptBuffer (String &FileName);
      virtual ~V8ScriptBuffer ();

      String get_file_name () const;
      // ExternalAsciiStringResource Interface.
      virtual const char *data () const;
      virtual size_t length () const;

   protected:
      struct State;
      State &_state;

   private:
      V8ScriptBuffer ();
      V8ScriptBuffer (const V8ScriptBuffer &);
      V8ScriptBuffer &operator= (const V8ScriptBuffer &);
};

class DMZ_JS_V8_UTIL_LINK_SYMBOL V8EmbeddedBuffer :
      public v8::String::ExternalAsciiStringResource {

   public:
      V8EmbeddedBuffer (const char *Buffer);
      virtual ~V8EmbeddedBuffer ();

      // ExternalAsciiStringResource Interface.
      virtual const char *data () const;
      virtual size_t length () const;

   protected:
      struct State;
      State &_state;

   private:
      V8EmbeddedBuffer ();
      V8EmbeddedBuffer (const V8EmbeddedBuffer &);
      V8EmbeddedBuffer &operator= (const V8EmbeddedBuffer &);
};

};

#endif // DMZ_JS_V8_UTIL_DOT_H
