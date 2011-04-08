#ifndef DMZ_V8_QT_LABEL_DOT_H
#define DMZ_V8_QT_LABEL_DOT_H

#include <dmzV8QtWidget.h>


namespace dmz {

   class V8QtLabel :  public V8QtWidget {

      Q_OBJECT

      public:
         V8QtLabel (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtLabel ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_linkActivated (const QString &Link);
   };
};


#endif // DMZ_V8_QT_LABEL_DOT_H
