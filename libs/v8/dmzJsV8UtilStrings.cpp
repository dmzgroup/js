#include <dmzJsV8UtilStrings.h>
#include <dmzSystemFile.h>

#include <string.h>

struct dmz::V8FileString::State {

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


dmz::V8FileString::V8FileString (const String &FileName) :
      _state (*(new State (FileName))) {;}


dmz::V8FileString::~V8FileString () { delete &_state; }

dmz::String
dmz::V8FileString::get_file_name () const { return _state.FileName; }


const char *
dmz::V8FileString::data () const { return _state.buffer; } 


size_t
dmz::V8FileString::length () const { return _state.length; }


struct dmz::V8EmbeddedString::State {

   const char *Buffer;
   const size_t Length;

   State (const char *TheBuffer, const size_t TheLength) :
         Buffer (TheBuffer),
         Length (TheLength ? TheLength : (TheBuffer ? strlen (TheBuffer) : 0)) {;}

   ~State () {;}
};


dmz::V8EmbeddedString::V8EmbeddedString (const char *Buffer, const size_t Length) :
      _state (*(new State (Buffer, Length))) {;}


dmz::V8EmbeddedString::~V8EmbeddedString () { delete &_state; }


const char *
dmz::V8EmbeddedString::data () const { return _state.Buffer; } 


size_t
dmz::V8EmbeddedString::length () const { return _state.Length; }

