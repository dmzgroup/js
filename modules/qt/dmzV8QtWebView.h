#ifndef DMZ_V8_QT_WEB_VIEW_DOT_H
#define DMZ_V8_QT_WEB_VIEW_DOT_H

#include <dmzV8QtWidget.h>

class QLineEdit;

namespace dmz {

   class V8QtWebView : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtWebView (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtWebView ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_linkClicked (const QUrl &);
   };
};

#endif
