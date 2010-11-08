#ifndef DMZ_V8_QT_INPUT_DIALOG_DOT_H
#define DMZ_V8_QT_INPUT_DIALOG_DOT_H

#include <dmzV8QtDialog.h>


namespace dmz {

   class V8QtInputDialog :  public V8QtDialog {

      Q_OBJECT

      public:
         V8QtInputDialog (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtInputDialog ();

         virtual Boolean bind (
               const String &Signal,
               const V8Object &Self,
               const V8Function &Func);

      public Q_SLOTS:
         void on_finished (int);
         void on_doubleValueChanged (double);
         void on_doubleValueSelected (double);
         void on_intValueChanged (int);
         void on_intValueSelected (int);
         void on_textValueChanged (const QString &);
         void on_textValueSelected (const QString &);
   };
};


#endif // DMZ_V8_QT_INPUT_DIALOG_DOT_H
