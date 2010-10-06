#ifndef DMZ_V8_QT_STACKED_WIDGET_DOT_H
#define DMZ_V8_QT_STACKED_WIDGET_DOT_H

#include <dmzV8QtWidget.h>

namespace dmz {

   class V8QtStackedWidget : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtStackedWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtStackedWidget ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_currentChanged (int index);
         void on_widgetRemoved (int index);
   };
};


#endif // DMZ_V8_QT_STACKED_WIDGET_DOT_H
