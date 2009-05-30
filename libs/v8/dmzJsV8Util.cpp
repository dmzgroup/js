#include <dmzJsV8Util.h>
#include <dmzSystemFile.h>

#include <string.h>

struct dmz::V8ScriptBuffer::State {

   const String FileName;
   char *buffer;
   size_t length;

   State (const String &TheFileName) :
         FileName (TheFileName),
         buffer (0),
         length (0) { init (); }

   ~State () { if (buffer) { delete buffer; buffer = 0; } }

   void init () {

      length = get_file_size (FileName);

      if (length > 0) {

         buffer = new char[length + 1];

         if (buffer) {

            buffer[length] = '\0';

            FILE *file = open_file (FileName, "rb");

            if (file) {

               Int32 place (0);

               while (place < length) {

                  place += read_file (file, length - place, &(buffer[place]));
               }

               close_file (file);
            }
         }
      }
   }
};


dmz::V8ScriptBuffer::V8ScriptBuffer (String &FileName) :
      _state (*(new State (FileName))) {;}


dmz::V8ScriptBuffer::~V8ScriptBuffer () { delete &_state; }

dmz::String
dmz::V8ScriptBuffer::get_file_name () const { return _state.FileName; }


const char *
dmz::V8ScriptBuffer::data () const { return _state.buffer; } 


size_t
dmz::V8ScriptBuffer::length () const { return _state.length; }


struct dmz::V8EmbeddedBuffer::State {

   const char *Buffer;
   size_t length;

   State (const char *TheBuffer) :
         Buffer (TheBuffer),
         length (TheBuffer ? strlen (TheBuffer) : 0) {;}

   ~State () {;}
};


dmz::V8EmbeddedBuffer::V8EmbeddedBuffer (const char *Buffer) :
      _state (*(new State (Buffer))) {;}


dmz::V8EmbeddedBuffer::~V8EmbeddedBuffer () { delete &_state; }


const char *
dmz::V8EmbeddedBuffer::data () const { return _state.Buffer; } 


size_t
dmz::V8EmbeddedBuffer::length () const { return _state.length; }

