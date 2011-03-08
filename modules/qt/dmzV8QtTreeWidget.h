#ifndef DMZ_V8_QT_TREE_WIDGET_DOT_H
#define DMZ_V8_QT_TREE_WIDGET_DOT_H

#include <dmzV8QtWidget.h>

class QTreeWidgetItem;


namespace dmz {

   class V8QtTreeWidget : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtTreeWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtTreeWidget ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_currentItemChanged (QTreeWidgetItem *current, QTreeWidgetItem *previous);
         void on_currentRowChanged (int value);
         void on_currentTextChanged (const QString &Value);
         void on_itemActivated (QTreeWidgetItem *item, int column);
         void on_itemExpanded (QTreeWidgetItem *item);
         void on_itemSelectionChanged (void);
   };
};


#endif // DMZ_V8_QT_TREE_WIDGET_DOT_H
