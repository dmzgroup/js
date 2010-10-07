#ifndef DMZ_V8_QT_TAB_WIDGET_DOT_H
#define DMZ_V8_QT_TAB_WIDGET_DOT_H

#include <dmzV8QtWidget.h>

namespace dmz {

   class V8QtTabWidget : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtTabWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtTabWidget ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_currentChanged (int index);
   };
};


#endif // DMZ_V8_QT_TAB_WIDGET_DOT_H
