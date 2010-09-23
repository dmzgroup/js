#ifndef DMZ_V8_QT_OBJECT_DOT_H
#define DMZ_V8_QT_OBJECT_DOT_H

#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilTypes.h>
#include <dmzTypes.h>
#include <dmzTypesDeleteListTemplate.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <v8.h>

class QWidget;


namespace dmz {

   class V8QtObject : public QObject {

      Q_OBJECT

      public:
         V8QtObject (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtObject ();

         QWidget *get_qt_widget () const;

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func) = 0;

         void release_callback (const Handle Observer);

         V8ObjectPersist self;

      protected:
         struct CallbackStruct;

         struct CallbackTable {

            const String &Signal;
            HashTableHandleTemplate<CallbackStruct> table;

            CallbackTable (const String &TheSignal) : Signal (TheSignal) {;}
            ~CallbackTable () { table.empty (); }
         };

         struct CallbackStruct {

            const Handle Observer;
            V8ObjectPersist self;
            V8FunctionPersist func;

            CallbackStruct (const Handle TheObserver) : Observer (TheObserver) {;}

            ~CallbackStruct () {

               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };

         void _register_callback (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

         QPointer<QWidget> _widget;
         JsModuleUiV8QtBasicState *_state;
         HashTableStringTemplate<CallbackTable> _cbTable;
   };
};


#endif // DMZ_V8_QT_OBJECT_DOT_H
