#include <dmzJsExtV8.h>
#include "dmzJsModuleV8Basic.h"
#include <dmzJsKernelEmbed.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilStrings.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeConfigToStringContainer.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzV8IntenralEmbed.h>

#include <strings.h>
#include <stdio.h>

namespace {

static const char LocalDMZName[] = "DMZ";

const char *
to_c_string (const v8::String::Utf8Value& value) {

   return *value ? *value : "<string conversion failed>";
}


v8::Handle<v8::Value>
local_print (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::StreamLog *out = (dmz::StreamLog *)v8::External::Unwrap (args.Data ());

   if (out) {

      const int Length = args.Length ();

      for (int ix = 0; ix < Length; ix++) {

         if (ix > 0) { *out << " "; }

         v8::String::Utf8Value str (args[ix]);
         const char *cstr = to_c_string (str);
         *out << cstr;
      }

      *out << dmz::endl;
   }

   return scope.Close (v8::Undefined());
}


v8::Handle<v8::Value>
local_add_stack_trace (const v8::Arguments &args) {

   v8::HandleScope scope;

   dmz::JsModuleV8Basic *module =
      (dmz::JsModuleV8Basic *)v8::External::Unwrap (args.Data ());

   if (module) {

      module->add_stack_trace (
         args[0]->Int32Value (), // Level
         *(v8::String::Utf8Value (args[1])), // Script
         args[2]->Int32Value () + 1, // Line, Add one since line count starts at zero.
         args[3]->Int32Value (), // Column.
         *(v8::String::Utf8Value (args[4]))); // Code
   }

   return scope.Close (v8::Undefined());
}

};


dmz::JsModuleV8Basic::JsModuleV8Basic (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsModuleV8 (Info),
      _log (Info),
      _out ("", LogLevelOut, Info.get_context ()),
      _rc (Info),
      _kernelList (0),
      _scriptList (0),
      _stHead (0),
      _stTail (0) {

   _init (local);
}


dmz::JsModuleV8Basic::~JsModuleV8Basic () {

   _extTable.clear ();

   if (_stHead) { delete _stHead; }
   _stTail = 0;
   if (_scriptList) { delete _scriptList; _scriptList = 0; }
   if (_kernelList) { delete _kernelList; _kernelList = 0; }

   _root.Dispose ();
   _context.Dispose ();
}


// Plugin Interface
void
dmz::JsModuleV8Basic::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

      _init_context ();
      _load_kernel ();
      _run_scripts (_kernelList);
      _init_ext ();
      _load_scripts ();
      _run_scripts (_scriptList);
   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

   }
}


void
dmz::JsModuleV8Basic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      JsExtV8 *ext = JsExtV8::cast (PluginPtr);

      if (ext && _extTable.store (ext->get_js_ext_v8_handle (), ext)) {

         ext->store_js_module_v8 (*this);
         if (!_context.IsEmpty ()) { ext->open_js_v8_extension (_context, _root); }
      }
   }
   else if (Mode == PluginDiscoverRemove) {

      JsExtV8 *ext = JsExtV8::cast (PluginPtr);

      if (ext && _extTable.remove (ext->get_js_ext_v8_handle ())) {

         if (!_context.IsEmpty ()) { ext->close_js_v8_extension (_context, _root); }
         ext->remove_js_module_v8 (*this);
      }
   }
}


// JsModuleV8Basic Interface
void
dmz::JsModuleV8Basic::add_stack_trace (
      const Int32 Level,
      const String &Script,
      const Int32 Line,
      const Int32 Column,
      const String &Code) {

   if ((Level == 0) && _stHead) { delete _stHead; _stHead = _stTail = 0; }

   StackTraceStruct *next = new StackTraceStruct (Level, Script, Line, Column, Code);

   if (_stTail) { _stTail->next = next; _stTail = next; }
   else { _stHead = _stTail = next; }
}


void
dmz::JsModuleV8Basic::_init_context () {

   if (!_root.IsEmpty ()) { _root.Dispose (); _root.Clear (); }
   if (!_context.IsEmpty ()) { _context.Dispose (); _context.Clear (); }

   char flags[] = "--expose_debug_as V8";
   v8::V8::SetFlagsFromString (flags, strlen (flags));

   _context = v8::Context::New ();

   v8::Context::Scope scope (_context);
   v8::HandleScope hscope;

   v8::Handle<v8::Object> global = _context->Global ();

   if (!global.IsEmpty ()) {

      _root = v8::Persistent<v8::Object>::New (v8::Object::New ());

      v8::Handle<v8::Object> debug = v8::Object::New ();
      _root->Set (v8::String::New ("Debug"), debug);

      global->Set (v8::String::New (LocalDMZName), _root);

      global->Set (
         v8::String::New ("print"),
         v8::FunctionTemplate::New (
            local_print,
            v8::External::Wrap (&_out))->GetFunction ());

      debug->Set (
         v8::String::New ("addStackTrace"),
         v8::FunctionTemplate::New (
            local_add_stack_trace,
            v8::External::Wrap (this))->GetFunction ());
   }
   else { _log.error << "No global object." << endl; }
}


void
dmz::JsModuleV8Basic::_init_ext () {

   HashTableHandleIterator it;
   JsExtV8 *ext (0);

   while (_extTable.get_next (it, ext)) {

      ext->open_js_v8_extension (_context, _root);
   }
}


void
dmz::JsModuleV8Basic::_handle_exception (v8::TryCatch &tc) {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   v8::String::Utf8Value exception (tc.Exception ());
   const String ExStr = to_c_string (exception);

   v8::Handle<v8::Message> message = tc.Message ();

   if (message.IsEmpty()) {

      _log.error << "Unhandled exception: " << ExStr << endl;
   }
   else {

      v8::String::Utf8Value fptr (message->GetScriptResourceName ());
      const String FileName = to_c_string (fptr);
      const Int32 Line = message->GetLineNumber ();
      _log.error << FileName << ":" << Line << ": " << ExStr << endl;

      if (!_stHead) {

         // Print line of source code.
         v8::String::Utf8Value sptr (message->GetSourceLine ());
         _log.error << to_c_string (sptr) << endl;

         // Print wavy underline (GetUnderline is deprecated).
         int start = message->GetStartColumn ();
         int end = message->GetEndColumn ();

         String space;
         space.repeat ("-", start - 1);
         String line;
         line.repeat ("^", end - start);

         _log.error << " " << space << line << endl;
      }
   }

   if (_stHead) {

      _log.error << "Stack Trace:" << endl;

      StackTraceStruct *current (_stHead);

      while (current) {

         _log.error << "-=-=-=-=-=-=-=-=-=-=- Level: " << current->Level
            << " -=-=-=-=-=-=-=-=-=-=-" << endl;
         _log.error << "Line:" << current->Line << ":" << current->Script << endl << endl;
         _log.error << current->Code << endl;
         String space;
         space.repeat ("-", current->Column);
         _log.error << space << "^" << endl;

         current = current->next;
      }

      delete _stHead; _stHead = _stTail = 0;
   }
}


void
dmz::JsModuleV8Basic::_load_kernel () {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   if (_kernelList) { delete _kernelList; _kernelList = 0; }

   ScriptStruct *last (0);

   const int Count = get_embedded_js_count ();

   for (int ix = 0; ix < Count; ix++) {

      V8EmbeddedString *buffer = new V8EmbeddedString (
         get_embedded_js_text (ix),
         get_embedded_js_length (ix));

      ScriptStruct *ss = new ScriptStruct (get_embedded_js_file_name (ix));

      if (ss) {

         v8::TryCatch tc;

         v8::Handle<v8::Script> script = v8::Script::Compile (
            v8::String::NewExternal (buffer),
            v8::String::New (get_embedded_js_file_name (ix)));

         if (script.IsEmpty ()) { _handle_exception (tc); }
         else {

            ss->script = v8::Persistent<v8::Script>::New (script);
            if (last) { last->next = ss; last = ss; }
            else { _kernelList = last = ss; }
         }
      }
   }
 
   V8EmbeddedString *buffer = new V8EmbeddedString (
      get_v8_internal_text (0),
      get_v8_internal_length (0));

   ScriptStruct *ss = new ScriptStruct (get_v8_internal_file_name (0));

   if (ss) {

      v8::TryCatch tc;

      v8::Handle<v8::Script> script = v8::Script::Compile (
         v8::String::NewExternal (buffer),
         v8::String::New (get_v8_internal_file_name (0)));

      if (script.IsEmpty ()) { _handle_exception (tc); }
      else {

         ss->script = v8::Persistent<v8::Script>::New (script);
         if (last) { last->next = ss; last = ss; }
         else { _kernelList = last = ss; }
      }
   }  
}


void
dmz::JsModuleV8Basic::_load_scripts () {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   ScriptStruct *current (_scriptList);

   while (current) {

      current->clear ();
      
      v8::TryCatch tc;
      V8FileString *sptr = new V8FileString (current->FileName);

      v8::Handle<v8::Script> script = v8::Script::Compile (
         v8::String::NewExternal (sptr),
         v8::String::New (current->FileName.get_buffer ()));

      if (script.IsEmpty ()) { _handle_exception (tc); }
      else { current->script = v8::Persistent<v8::Script>::New (script); }

      current = current->next;
   }
}


void
dmz::JsModuleV8Basic::_run_scripts (ScriptStruct *list) {

   v8::Context::Scope cscope (_context);
   v8::HandleScope hscope;

   ScriptStruct *current (list);

   while (current) {

      if (!current->script.IsEmpty ()) {

         v8::TryCatch tc;

         v8::Handle<v8::Value> value = current->script->Run ();

         if (value.IsEmpty ()) {

            _handle_exception (tc);
            current->clear ();
         }
      }

      current = current->next;
   }
}


void
dmz::JsModuleV8Basic::_init (Config &local) {

   _localPaths = config_to_path_string_container (local);

   Config scriptList;

   if (local.lookup_all_config ("script", scriptList)) {

      ScriptStruct *last (0);
      ConfigIterator it;
      Config script;

      while (scriptList.get_next_config (it, script)) {

         const String Name = config_to_string ("name", script);

         if (Name) {

            String scriptPath = _rc.find_file (Name);

            if (!scriptPath && (_localPaths.get_count () > 0)) {

               find_file (_localPaths, Name + ".js", scriptPath);
            }

            if (scriptPath) {

               ScriptStruct *ss = new ScriptStruct (scriptPath);

               if (last) { last->next = ss; ss = last; }
               else { _scriptList = last = ss; }
            }
            else {

               _log.error << "Failed to find script resource: " << Name << endl;
            }
         }
      }
   }

   _log.info << "Using V8 v" << v8::V8::GetVersion () << endl;
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleV8Basic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleV8Basic (Info, local);
}

};
