#include <dmzFoundationBase64.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzJsPluginV8ConsoleQt.h"
#include <dmzQtUtil.h>
#include <dmzSystemFile.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeConfigWrite.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzRuntimeSession.h>
#include <QtGui/QAction>

// This header needs to go here in order to compile -rb
#include "dmzJsFunctions.h"


v8::Handle<v8::Value>
dmz::JsPluginV8ConsoleQt::_print (const v8::Arguments &args) {

   v8::HandleScope scope;

   JsPluginV8ConsoleQt *self = (JsPluginV8ConsoleQt *)v8::External::Unwrap (args.Data ());

   if (self) {

      const int Length = args.Length ();

      String out;

      for (int ix = 0; ix < Length; ix++) {

         if (ix > 0) { out << " "; }

         out << dmz::v8_to_string (args[ix]);
      }

      self->append (out);
   }

   return scope.Close (v8::Undefined());
}


dmz::Highlighter::Highlighter (QTextDocument *parent) : QSyntaxHighlighter (parent) {

   HighlightingRule rule;


   keywordFormat.setForeground (Qt::blue);
   keywordFormat.setFontWeight (QFont::Bold);
   QStringList keywordPatterns;

   keywordPatterns
      // JavaScript Reserved Words
      << "\\bbreak\\b"
      << "\\bcontinue\\b"
      << "\\bdo\\b"
      << "\\bfor\\b"
      << "\\bimport\\b"
      << "\\bnew\\b"
      << "\\bthis\\b"
      << "\\bself\\b"
      << "\\bvoid\\b"
      << "\\bcase\\b"
      << "\\bdefault\\b"
      << "\\belse\\b"
      << "\\bfunction\\b"
      << "\\bin\\b"
      << "\\breturn\\b"
      << "\\btypeof\\b"
      << "\\bnull\\b"
      << "\\bundefined\\b"
      << "\\bwhile\\b"
      << "\\bcomment\\b"
      << "\\bdelete\\b"
      << "\\bexport\\b"
      << "\\bif\\b"
      << "\\blabel\\b"
      << "\\bswitch\\b"
      << "\\bvar\\b"
      << "\\bwith\\b"
#if 0
      // reserved Java keywords in JavaScript
      << "\\babstract\\b"
      << "\\bimplements\\b"
      << "\\bprotected\\b"
      << "\\bboolean\\b"
      << "\\binstanceOf\\b"
      << "\\bpublic\\b"
      << "\\bbyte\\b"
      << "\\bint\\b"
      << "\\bshort\\b"
      << "\\bchar\\b"
      << "\\binterface\\b"
      << "\\bstatic\\b"
      << "\\bdouble\\b"
      << "\\blong\\b"
      << "\\bsynchronized\\b"
      << "\\bfalse\\b"
      << "\\bnative\\b"
      << "\\bthrows\\b"
      << "\\bfinal\\b"
      << "\\bnull\\b"
      << "\\btransient\\b"
      << "\\bfloat\\b"
      << "\\bpackage\\b"
      << "\\btrue\\b"
      << "\\bgoto\\b"
      << "\\bprivate\\b"
#endif // 0
      // ECMAScript Reserved Words
      << "\\bcatch\\b"
      << "\\benum\\b"
      << "\\bthrow\\b"
      << "\\bclass\\b"
      << "\\bextends\\b"
      << "\\btry\\b"
      << "\\bconst\\b"
      << "\\bfinally\\b"
      << "\\bdebugger\\b"
      << "\\bsuper\\b"
      ;

   foreach (QString pattern, keywordPatterns) {

      rule.pattern = QRegExp (pattern);
      rule.format = keywordFormat;
      highlightingRules.append (rule);
   }

   funcFormat.setForeground (Qt::darkGreen);
   funcFormat.setFontWeight (QFont::Bold);
   QStringList funcPatterns;

   set_js_functions (funcPatterns);

   foreach (QString pattern, funcPatterns) {

      rule.pattern = QRegExp (pattern);
      rule.format = funcFormat;
      highlightingRules.append (rule);
   }

   singleLineCommentFormat.setForeground (Qt::darkGray);
   rule.pattern = QRegExp ("//[^\n]*");
   rule.format = singleLineCommentFormat;
   highlightingRules.append (rule);

   quotationFormat.setForeground (Qt::red);
   rule.pattern = QRegExp ("\"[^\"]*\"");
   rule.format = quotationFormat;
   highlightingRules.append (rule);

   squotationFormat.setForeground (Qt::red);
   rule.pattern = QRegExp ("'[^']*'");
   rule.format = squotationFormat;
   highlightingRules.append (rule);

   numberFormat.setForeground (Qt::red);
   numberFormat.setFontWeight (QFont::Bold);

   rule.pattern = QRegExp ("[-]?\\d{1,}[\\.]?\\d*[eE]{1}[-]?\\d{1,}");
   rule.format = numberFormat;
   highlightingRules.append (rule);

   rule.pattern = QRegExp ("[-]?\\d{1,}[\\.]?\\d*");
   rule.format = numberFormat;
   highlightingRules.append (rule);

   commentStartExpression = QRegExp ("/\\*");
   multiLineCommentFormat.setForeground (Qt::darkGray);
}


void
dmz::Highlighter::highlightBlock (const QString &text) {

   foreach (HighlightingRule rule, highlightingRules) {

      QRegExp expression (rule.pattern);
      int index = text.indexOf (expression);

      while (index >= 0) {

         int length = expression.matchedLength ();
         setFormat (index, length, rule.format);
         index = text.indexOf (expression, index + length);
      }
   }

   setCurrentBlockState (0);

   int startIndex = 0;

   if (previousBlockState () != 1) {

       startIndex = text.indexOf (commentStartExpression);
       QString comment ("\\*/");
       commentEndExpression = QRegExp (comment);
   }

   while (startIndex >= 0) {

      int endIndex = text.indexOf (commentEndExpression, startIndex);
      int commentLength;

      if (endIndex == -1) {

         setCurrentBlockState (1);
         commentLength = text.length () - startIndex;
      }
      else {

         commentLength = endIndex - startIndex
            + commentEndExpression.matchedLength ();
      }

      setFormat (startIndex, commentLength, multiLineCommentFormat);
      startIndex = text.indexOf (commentStartExpression, startIndex + commentLength);
   }
}


dmz::JsPluginV8ConsoleQt::JsPluginV8ConsoleQt (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      LogObserver (Info.get_context ()),
      MessageObserver (Info),
      JsExtV8 (Info),
      _log (Info),
      _hl (0),
      _historyCount (0),
      _historyCurrent (0),
      _historyHead (0),
      _historyTail (0),
      _core (0) {

   detach_log_observer ();

   _console.setupUi (this);

   _hl.setDocument (_console.inputBox->document ());

   QAction *executeAct = new QAction(this);
   executeAct->setShortcut(Qt::Key_Return + Qt::CTRL);

   QAction *prevAct = new QAction(this);
   prevAct->setShortcut(Qt::Key_K + Qt::CTRL);

   QAction *nextAct = new QAction(this);
   nextAct->setShortcut(Qt::Key_J + Qt::CTRL);

   connect(executeAct, SIGNAL (triggered ()), this, SLOT (on_executeButton_clicked ()));
   connect(prevAct, SIGNAL (triggered ()), this, SLOT (trigger_prev_history ()));
   connect(nextAct, SIGNAL (triggered ()), this, SLOT (trigger_next_history ()));

   _console.inputBox->addAction (executeAct);
   _console.inputBox->addAction (prevAct);
   _console.inputBox->addAction (nextAct);

   _defaultColor = _console.displayBox->textColor ();
   _init (local);
   
   adjustSize ();
   //show (); activateWindow ();
}


dmz::JsPluginV8ConsoleQt::~JsPluginV8ConsoleQt () {

   if (_historyHead) {

      delete _historyHead; _historyHead = _historyTail = _historyCurrent = 0;
   }
}


// Plugin Interface
void
dmz::JsPluginV8ConsoleQt::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateShutdown) {

      _printTemplate.Dispose (); _printTemplate.Clear ();

      RuntimeContext *context (get_plugin_runtime_context ());

      if (context) {

         String data;

         Config session (get_plugin_name ());

         session.add_config (qbytearray_to_config ("geometry", saveGeometry ()));

         session.add_config (
            qbytearray_to_config ("splitter", _console.splitter->saveState ()));

         if (isVisible ()) {

            session.add_config (boolean_to_config ("window", "visible", True));
         }

         HistoryStruct *current = _historyHead;

         Int32 count = 0;

         while (current && (count < 100)) {

            Config history ("history");

            history.store_attribute (
               "value",
                encode_base64 (qPrintable (current->Value)));

            session.add_config (history);
            current = current->next;
            count++;
         }

         set_session_config (context, session);
      }
   }
}


// MessageObserver Interface
void
dmz::JsPluginV8ConsoleQt::receive_message (
      const Message &Type,
      const Handle MessageSendHandle,
      const Handle TargetObserverHandle,
      const Data *InData,
      Data *outData) {

   if (Type == _showMsg) { show (); activateWindow (); }
}


// LogObserver Interface
void
dmz::JsPluginV8ConsoleQt::store_log_message (
      const String &LogName,
      const LogLevelEnum Level,
      const String &Message) {

   String out;
   if (LogName && (LogName != get_plugin_name ())) { out << LogName << ":"; }
   out << Message;

   if (Level == LogLevelError) {

      _console.displayBox->setTextColor (QColor ("red"));
   }
   else if (Level == LogLevelWarn) {

      _console.displayBox->setTextColor (QColor ("orange"));
   }

   append (out);

   _console.displayBox->setTextColor (_defaultColor);
}


// JsExtV8 Interface
void
dmz::JsPluginV8ConsoleQt::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }

}


void
dmz::JsPluginV8ConsoleQt::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsPluginV8ConsoleQt::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

   }
   else if (State == JsExtV8::Init) {

      _self = V8ObjectPersist::New (v8::Object::New ());
      _dmzObj = V8ObjectPersist::New (v8::Object::New ());
      _printFunc = V8FunctionPersist::New (_printTemplate->GetFunction ());

      if (_core) {

         _core->set_external_instance_handle_and_name (
            get_plugin_handle (),
            "Console",
            _self);

         StringContainer list;
         _core->get_require_list (list);

         StringContainerIterator it;
         String value;

         while (list.get_next (it, value)) {

            V8Object obj = _core->require (value);

            if (obj.IsEmpty ()) {

            }
            else {

               String path, name, ext;
               split_path_file_ext (value, path, name, ext);

               if (name) {
               
                  _log.info << "Mapping: " << value << " to: " << name << endl;
                  _dmzObj->Set (v8::String::NewSymbol (name.get_buffer ()), obj);
               }
            }
         }
      }
   }
   else if (State == JsExtV8::Shutdown) {

      _self.Dispose (); _self.Clear ();
      _dmzObj.Dispose (); _dmzObj.Clear ();
      _printFunc.Dispose (); _printFunc.Clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsPluginV8ConsoleQt::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

}


// JsPluginV8ConsoleQt Inteface
void
dmz::JsPluginV8ConsoleQt::append (const String &Str) {

   if (!Str.is_null ()) { _console.displayBox->append (Str.get_buffer ()); }
}

void
dmz::JsPluginV8ConsoleQt::on_resetButton_clicked () {

   if (_core) { _core->reset_v8 (); }
}


void
dmz::JsPluginV8ConsoleQt::on_executeButton_clicked () {

   QString str (_console.inputBox->toPlainText ());

   if (str.trimmed ().isEmpty ()) { _console.inputBox->clear (); }
   else if (_v8Context.IsEmpty () == false){

      _console.displayBox->setTextColor (QColor ("blue"));
      _console.displayBox->append (str);
      _console.displayBox->setTextColor (_defaultColor);

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      String scriptStr ("(function (self, dmz, print) {\n");
      scriptStr << qPrintable (str) << "\n});";

      attach_log_observer ();

      v8::TryCatch tc;

      v8::Handle<v8::Script> script = v8::Script::Compile (
         v8::String::New (scriptStr.get_buffer ()),
         v8::String::New ("Console Function"));

      if (tc.HasCaught ()) {

         if (_core) { _core->handle_v8_exception (tc); }
      }
      else {

         v8::Handle<v8::Value> value = script->Run ();

         if (tc.HasCaught ()) { if (_core) { _core->handle_v8_exception (tc); } }
         else {
 
            V8Function func = v8_to_function (value); 
            if (func.IsEmpty ()) { 
               // Error! no function returned. 
               _log.error << "No function generated by console input." << endl; 
            } 
            else { 

               V8Value argv[] = { _self, _dmzObj, _printFunc };
               func->Call (_self, 3, argv);

               if (tc.HasCaught ()) {

                  if (_core) { _core->handle_v8_exception (tc); }
               }
               else {

                  _console.inputBox->clear ();
                  _currentBuffer.clear ();
                  _add_history (str);
               }
            } 
         }
      }

      detach_log_observer ();
   }
}


void
dmz::JsPluginV8ConsoleQt::on_clearSelfButton_clicked () {

   if (_v8Context.IsEmpty () == false) {

      v8::Context::Scope cscope(_v8Context);
      v8::HandleScope scope;

      if (_self.IsEmpty () == false) {

         V8Array array = _self->GetPropertyNames ();

         if (array.IsEmpty () == false) {

            const uint32_t Length = array->Length ();

            for (uint32_t ix = 0; ix < Length; ix++) {

               V8Value value = array->Get (v8::Integer::New (ix));

               if ((value.IsEmpty () == false) && value->IsString ()) {

                  _self->Delete (value->ToString ());
               }
            }
         }
      }
   }
}


void
dmz::JsPluginV8ConsoleQt::trigger_prev_history () {

   Boolean set (False);

   if (!_historyCurrent) {

      _currentBuffer = _console.inputBox->toPlainText ();
      _historyCurrent = _historyHead; set = True;
   }
   else if (_historyCurrent->next) {

      _historyCurrent = _historyCurrent->next;
      set = True;
   }

   if (set && _historyCurrent) {

      _console.inputBox->setPlainText (_historyCurrent->Value);
   }
}


void
dmz::JsPluginV8ConsoleQt::trigger_next_history () {

   Boolean wasCurrent = False;

   if (_historyCurrent) { _historyCurrent = _historyCurrent->prev; wasCurrent = True; }

   if (_historyCurrent) {

      _console.inputBox->setPlainText (_historyCurrent->Value);
   }
   else if (!_currentBuffer.isEmpty () || wasCurrent) {

      _console.inputBox->setPlainText (_currentBuffer);
      _currentBuffer.clear ();
   }
}


void
dmz::JsPluginV8ConsoleQt::_add_history (const QString &Value) {

   const QString Trimmed = Value.trimmed ();

   if (!Trimmed.isEmpty ()) {

      _historyCurrent = 0;

      if (!_historyHead) {

         _historyHead = _historyTail = new HistoryStruct (Value);
         if (_historyHead) { _historyCount = 1; }
      }
      else if (_historyHead->Value != Value) {

         HistoryStruct *hs (new HistoryStruct (Value));

         if (hs) {

            hs->next = _historyHead;
            _historyHead->prev = hs;
            _historyHead = hs;
            _historyCount++;
         }
      }
   }
}


void
dmz::JsPluginV8ConsoleQt::_init (Config &local) {

   v8::HandleScope scope;

   RuntimeContext *context (get_plugin_runtime_context ());

   _showMsg = config_create_message (
      "show.name",
      local,
      "DMZ_Show_Java_Script_Console",
      context);

   subscribe_to_message (_showMsg);

   if (context) {

      Config session (get_session_config (get_plugin_name (), context));

      QByteArray geometry (config_to_qbytearray ("geometry", session, saveGeometry ()));
      restoreGeometry (geometry);

      geometry = config_to_qbytearray ("splitter", session);

      if (!geometry.isEmpty ()) { _console.splitter->restoreState (geometry); }

      if (config_to_boolean ("window.visible", session, False)) { show (); }

      Config historyList;

      if (session.lookup_all_config ("history", historyList)) {

         ConfigIterator it;
         Config history;

         Boolean found (historyList.get_last_config (it, history));

         while (found) {

            const String Value = decode_base64 (config_to_string ("value", history));
            _add_history (Value.get_buffer ());
            found = historyList.get_prev_config (it, history);
         }
      }
   }

   _printTemplate = v8::Persistent<v8::FunctionTemplate>::New (
      v8::FunctionTemplate::New (_print, v8::External::Wrap (this)));
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsPluginV8ConsoleQt (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsPluginV8ConsoleQt (Info, local);
}

};
