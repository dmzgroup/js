#ifndef DMZ_V8_QT_BUTTON_DOT_H
#define DMZ_V8_QT_BUTTON_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtButton :  public V8QtObject {
      
      Q_OBJECT
      
      public:
         V8QtButton (QWidget *widget, JsModuleUiV8QtBasic::State *state);
         virtual ~V8QtButton ();
         
         virtual Boolean bind (QWidget *sender, const String &Signal);
         
      public Q_SLOTS:
         void on_clicked ();
   };
};


#endif // DMZ_V8_QT_BUTTON_DOT_H
