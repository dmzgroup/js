#ifndef DMZ_V8_QT_LIST_WIDGET_DOT_H
#define DMZ_V8_QT_LIST_WIDGET_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtListWidget : public V8QtObject {
      
      Q_OBJECT
      
      public:
         V8QtListWidget (QWidget *widget, JsModuleUiV8QtBasic::State *state);
         virtual ~V8QtListWidget ();
         
         virtual Boolean bind (QWidget *sender, const String &Signal);

      public Q_SLOTS:
         // void on_clicked ();
   };
};


#endif // DMZ_V8_QT_WIDGET_DOT_H
