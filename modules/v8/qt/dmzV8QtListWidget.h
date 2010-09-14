#ifndef DMZ_V8_QT_LIST_WIDGET_DOT_H
#define DMZ_V8_QT_LIST_WIDGET_DOT_H

#include <dmzV8QtObject.h>

class QListWidgetItem;


namespace dmz {

   class V8QtListWidget : public V8QtObject {
      
      Q_OBJECT
      
      public:
         V8QtListWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtListWidget ();
         
         virtual Boolean bind (QWidget *sender, const String &Signal);

      public Q_SLOTS:
         void on_currentItemChanged (QListWidgetItem *current, QListWidgetItem *previous);
         void on_itemActivated (QListWidgetItem *item);
   };
};


#endif // DMZ_V8_QT_WIDGET_DOT_H
