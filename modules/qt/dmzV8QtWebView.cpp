#include <dmzJsModuleV8.h>
#include "dmzV8QtWebView.h"
#include <QtWebKit/QWebView>


namespace {

   static const dmz::String LocalSignalLinkClicked("linkClicked");
   static const dmz::String LocalSignalLoadFinished("loadFinished");
};


dmz::V8QtWebView::V8QtWebView (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtWidget (Self, widget, state) {;}


dmz::V8QtWebView::~V8QtWebView () {;}


dmz::Boolean
dmz::V8QtWebView::bind (
      const String &Signal,
      const V8Object &Self,
      const V8Function &Func) {

   Boolean results (False);

   if (_widget) {

      if (Signal == LocalSignalLinkClicked) {

         connect (
            _widget,
            SIGNAL (linkClicked (const QUrl &)),
            SLOT (on_linkClicked (const QUrl &)),
            Qt::UniqueConnection);

         results = True;
      }
      else if (Signal == LocalSignalLoadFinished) {

         connect (
            _widget,
            SIGNAL (loadFinished (bool)),
            SLOT (on_load_finished (bool)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (results) { _register_callback (Signal, Self, Func); }

   return results;
}


void
dmz::V8QtWebView::on_linkClicked (const QUrl &Link) {

   _do_callback (LocalSignalLinkClicked, Link.toString ());
}

void
dmz::V8QtWebView::on_load_finished (bool ok) {

   _do_callback (LocalSignalLoadFinished, ok);
}
