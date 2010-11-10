#ifndef DMZ_V8_QT_TABLE_WIDGET_DOT_H
#define DMZ_V8_QT_TABLE_WIDGET_DOT_H

#include <dmzV8QtWidget.h>

class QTableWidgetItem;


namespace dmz {

   class V8QtTableWidget : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtTableWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtTableWidget ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_cellClicked(int row, int col);
         void on_cellDoubleClicked (int row, int col);
         void on_currentItemChanged (
            QTableWidgetItem *current,
            QTableWidgetItem *previous);
         void on_itemSelectionChanged ();
   };
};


#endif // DMZ_V8_QT_TABLE_WIDGET_DOT_H
