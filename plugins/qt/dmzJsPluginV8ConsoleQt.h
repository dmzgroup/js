#ifndef DMZ_JS_PLUGIN_V8_CONSOLE_QT_DOT_H
#define DMZ_JS_PLUGIN_V8_CONSOLE_QT_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimeMessaging.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <QtCore/QHash>
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextCharFormat>
#include <QtGui/QWidget>

#include <ui_dmzJsConsole.h>

class QTextDocument;

namespace dmz {

   class JsModuleV8;

   class Highlighter : public QSyntaxHighlighter {

      Q_OBJECT

   public:
      Highlighter(QTextDocument *parent = 0);

   protected:
      void highlightBlock(const QString &text);

   private:
      struct HighlightingRule {

         QRegExp pattern;
         QTextCharFormat format;
      };

      QVector<HighlightingRule> highlightingRules;

      QRegExp commentStartExpression;
      QRegExp commentEndExpression;

      QTextCharFormat keywordFormat;
      QTextCharFormat funcFormat;
      QTextCharFormat singleLineCommentFormat;
      QTextCharFormat multiLineCommentFormat;
      QTextCharFormat quotationFormat;
      QTextCharFormat squotationFormat;
      QTextCharFormat numberFormat;
   };

   class JsPluginV8ConsoleQt :
         public QWidget,
         public Plugin,
         public MessageObserver,
         public LogObserver,
         public JsExtV8 {

      Q_OBJECT

      public:
         JsPluginV8ConsoleQt (const PluginInfo &Info, Config &local);
         ~JsPluginV8ConsoleQt ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr) {;}

         // MessageObserver Interface
         virtual void receive_message (
            const Message &Type,
            const Handle MessageSendHandle,
            const Handle TargetObserverHandle,
            const Data *InData,
            Data *outData);

         // LogObserver Interface
         virtual void store_log_message (
            const String &LogName,
            const LogLevelEnum Level,
            const String &Message);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         // JsPluginV8ConsoleQt Inteface
         void append (const String &Str);

      protected slots:
         void on_resetButton_clicked ();
         void on_executeButton_clicked ();
         void on_clearSelfButton_clicked ();
         void trigger_prev_history ();
         void trigger_next_history ();

      protected:
         struct HistoryStruct {

            HistoryStruct *prev;
            HistoryStruct *next;
            const QString Value;

            HistoryStruct (const QString &TheValue) :
                  Value (TheValue),
                  prev (0),
                  next (0) {;}

            ~HistoryStruct () { if (next) { delete next; next = 0; } }
         };

         void _add_history (const QString &Value);
         void _init (Config &local);

         Log _log;
         StreamLog _consoleLog;

         QColor _defaultColor;
         QString _currentBuffer;
         Message _showMsg;
         Ui::JsConsole _console;
         Highlighter _hl;
         Int32 _historyCount;
         HistoryStruct *_historyCurrent;
         HistoryStruct *_historyHead;
         HistoryStruct *_historyTail;

         HashTableStringTemplate<String> _requireMap;

         v8::Handle<v8::Context> _v8Context;
         JsModuleV8 *_core;

         V8ObjectPersist _self;
         V8ObjectPersist _dmzObj;
         V8FunctionTemplatePersist _printTemplate;
         V8FunctionPersist _printFunc;

      private:
         JsPluginV8ConsoleQt ();
         JsPluginV8ConsoleQt (const JsPluginV8ConsoleQt &);
         JsPluginV8ConsoleQt &operator= (const JsPluginV8ConsoleQt &);

   };
};

#endif // DMZ_JS_PLUGIN_V8_CONSOLE_QT_DOT_H
