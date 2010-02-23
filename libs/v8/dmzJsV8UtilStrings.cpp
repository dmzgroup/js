#include <dmzJsV8UtilStrings.h>
#include <dmzSystemFile.h>

#include <string.h>

struct dmz::V8FileString::State {

   const String FileName;
   const String Header;
   const String Footer;
   char *buffer;
   size_t length;

   State (const String &TheFileName, const String TheHeader, const String &TheFooter) :
         FileName (TheFileName),
         Header (TheHeader),
         Footer (TheFooter),
         buffer (0),
         length (0) { init (); }

   ~State () { if (buffer) { delete buffer; buffer = 0; } }

   void init () {

      // size will be different than length if a Header and/or Footer is defined.
      size_t size  = get_file_size (FileName);
      length = size;

      if (Header) { length += Header.get_length (); size = length; }
      if (Footer) { length += Footer.get_length (); }

      if (length > 0) {

         buffer = new char[length + 1];

         if (buffer) {

            buffer[length] = '\0';

            Int32 place (0);

            if (Header) {

               strncpy (buffer, Header.get_buffer (), Header.get_length ());
               place += Header.get_length ();
            }

            FILE *file = open_file (FileName, "rb");

            if (file) {

               while (place < size) {

                  place += read_file (file, length - place, &(buffer[place]));
               }

               close_file (file);
            }

            if (Footer) {

               strncpy (&(buffer[place]), Footer.get_buffer (), Footer.get_length ());
            }
         }
      }
   }
};


dmz::V8FileString::V8FileString (const String &FileName) :
      _state (*(new State (FileName, "", ""))) {;}


dmz::V8FileString::V8FileString (
      const String &FileName,
      const String &Header,
      const String &Footer) :
      _state (*(new State (FileName, Header, Footer))) {;}


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
   String *ptr;

   State (String *thePtr) :
         Buffer (thePtr ? thePtr->get_buffer () : 0),
         Length (thePtr ? thePtr->get_length () : 0),
         ptr (thePtr) {;}

   State (const char *TheBuffer, const size_t TheLength) :
         Buffer (TheBuffer),
         Length (TheLength ? TheLength : (TheBuffer ? strlen (TheBuffer) : 0)),
         ptr (0) {;}

   ~State () { Buffer = 0; if (ptr) { delete ptr; ptr = 0; } }
};


dmz::V8EmbeddedString::V8EmbeddedString (String *ptr) :
      _state (*(new State (ptr))) {;}


dmz::V8EmbeddedString::V8EmbeddedString (const char *Buffer, const size_t Length) :
      _state (*(new State (Buffer, Length))) {;}


dmz::V8EmbeddedString::~V8EmbeddedString () { delete &_state; }


const char *
dmz::V8EmbeddedString::data () const { return _state.Buffer; } 


size_t
dmz::V8EmbeddedString::length () const { return _state.Length; }

