#ifndef DMZ_V8_QT_WIDGET_DOT_H
#define DMZ_V8_QT_WIDGET_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtWidget : public V8QtObject {
      
      Q_OBJECT
      
      public:
         V8QtWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);
         
         virtual ~V8QtWidget ();
   };
};


#endif // DMZ_V8_QT_WIDGET_DOT_H
