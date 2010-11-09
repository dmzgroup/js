#ifndef DMZ_V8_QT_DATE_TIME_EDIT_DOT_H
#define DMZ_V8_QT_DATE_TIME_EDIT_DOT_H

#include <dmzV8QtWidget.h>

class QDate;
class QTime;
class QDateTime;

namespace dmz {

   class V8QtDateTimeEdit :  public V8QtWidget {

      Q_OBJECT

      public:
         V8QtDateTimeEdit (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtDateTimeEdit ();

         virtual Boolean bind (
               const String &Signal,
               const V8Object &Self,
               const V8Function &Func);


      public Q_SLOTS:
         void on_date_time_changed (const QDateTime &value);
   };
};


#endif // DMZ_V8_QT_DATE_TIME_EDIT_DOT_H
