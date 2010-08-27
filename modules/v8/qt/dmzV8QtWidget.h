#ifndef DMZ_V8_QT_WIDGET_DOT_H
#define DMZ_V8_QT_WIDGET_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtWidget : public V8QtObject {
      
      Q_OBJECT
      
      public:
         V8QtWidget (QWidget *widget, JsModuleUiV8QtBasic::State *state);
         virtual ~V8QtWidget ();
   };
};


#endif // DMZ_V8_QT_WIDGET_DOT_H
