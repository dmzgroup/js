#ifndef DMZ_V8_QT_DOCK_WIDGET_DOT_H
#define DMZ_V8_QT_DOCK_WIDGET_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtDockWidget :  public V8QtObject {

      Q_OBJECT

      public:
         V8QtDockWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtDockWidget ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_topLevelChanged (bool topLevel);
         void on_visibilityChanged (bool visible);
   };
};


#endif // DMZ_V8_QT_DOCK_WIDGET_DOT_H
