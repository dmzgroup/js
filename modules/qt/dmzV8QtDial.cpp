#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include "dmzV8QtDial.h"
#include <QtGui/QDial>
#include <QtCore/QDebug>


namespace {

   static const dmz::String SliderPressed ("sliderPressed");
   static const dmz::String SliderReleased ("sliderReleased");
   static const dmz::String ValueChanged ("valueChanged");
};


dmz::V8QtDial::V8QtDial (
      const V8Object &Self,
      QWidget *widget,
      JsModuleUiV8QtBasicState *state) :
      V8QtObject (Self, widget, state) {;}


dmz::V8QtDial::~V8QtDial () {;}


dmz::Boolean
dmz::V8QtDial::bind (QWidget *sender, const String &Signal) {

   Boolean results (False);

   if (sender) {

      if (Signal == SliderPressed) {

         connect (
            sender,
            SIGNAL (sliderPressed ()),
            this,
            SLOT (on_sliderPressed ()),
            Qt::UniqueConnection);

         results = True;
      }

      if (Signal == SliderReleased) {

         connect (
            sender,
            SIGNAL (sliderReleased ()),
            this,
            SLOT (on_sliderReleased ()),
            Qt::UniqueConnection);

         results = True;
      }

      if (Signal == ValueChanged) {

         connect (
            sender,
            SIGNAL (valueChanged (int)),
            this,
            SLOT (on_valueChanged (int)),
            Qt::UniqueConnection);

         results = True;
      }
   }

   if (!results) { results = V8QtObject::bind (sender, Signal); }

   return results;
}

void
dmz::V8QtDial::on_sliderPressed () {

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (SliderPressed);
      if (ct) {

         HashTableHandleIterator it;
         CallbackStruct *cs (0);

         while (ct->table.get_next (it, cs)) {

            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

               const Handle Observer = cs->Observer;

               const int Argc (1);
               V8Value argv[Argc];
               argv[0] = cs->self;

               v8::TryCatch tc;

               cs->func->Call (cs->self, Argc, argv);

               if (tc.HasCaught ()) {

                  _state->core->handle_v8_exception (Observer, tc);

                  cs = ct->table.remove (Observer);

                  if (cs) { delete cs; cs = 0; }
               }
            }
         }
      }
   }
}


void
dmz::V8QtDial::on_sliderReleased () {

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (SliderReleased);
      if (ct) {

         HashTableHandleIterator it;
         CallbackStruct *cs (0);

         while (ct->table.get_next (it, cs)) {

            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

               const Handle Observer = cs->Observer;

               const int Argc (1);
               V8Value argv[Argc];
               argv[0] = cs->self;

               v8::TryCatch tc;

               cs->func->Call (cs->self, Argc, argv);

               if (tc.HasCaught ()) {

                  _state->core->handle_v8_exception (Observer, tc);

                  cs = ct->table.remove (Observer);

                  if (cs) { delete cs; cs = 0; }
               }
            }
         }
      }
   }
}


void
dmz::V8QtDial::on_valueChanged (int val) {

   if (_state && _state->core && _state->ui) {

      v8::Context::Scope cscope (_state->context);
      v8::HandleScope scope;

      CallbackTable *ct = _cbTable.lookup (ValueChanged);
      if (ct) {

         HashTableHandleIterator it;
         CallbackStruct *cs (0);

         while (ct->table.get_next (it, cs)) {

            if (!(cs->func.IsEmpty ()) && !(cs->self.IsEmpty ())) {

               const Handle Observer = cs->Observer;

               const int Argc (2);
               V8Value argv[Argc];
               argv[0] = v8::Number::New (val);
               argv[1] = cs->self;

               v8::TryCatch tc;

               cs->func->Call (cs->self, Argc, argv);

               if (tc.HasCaught ()) {

                  _state->core->handle_v8_exception (Observer, tc);

                  cs = ct->table.remove (Observer);

                  if (cs) { delete cs; cs = 0; }
               }
            }
         }
      }
   }
}

