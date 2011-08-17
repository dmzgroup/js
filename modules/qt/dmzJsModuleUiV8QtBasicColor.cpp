#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"

dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_palette (QPalette *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object obj = _paletteCtor->NewInstance ();
      if (!obj.IsEmpty ()) {

         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
         result = obj;
      }
   }

   return scope.Close (result);
}

QPalette *
dmz::JsModuleUiV8QtBasic::_to_qpalette (V8Value value) {

   v8::HandleScope scope;
   QPalette *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_paletteTemp->HasInstance (obj)) {

         result = (QPalette *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}

dmz::V8Value
dmz::JsModuleUiV8QtBasic::_palette_brush (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPalette *pal = self->_to_qpalette (Args.This ());
      if (pal) {

         QBrush *brush (0);
         if (Args.Length () == 1) {

            brush = new QBrush (pal->brush ((QPalette::ColorRole) v8_to_number (Args[0])));
         }
         else if (Args.Length () == 2) {

            if (Args[1]->IsNumber ()) {

               brush =
                  new QBrush (
                     pal->brush (
                        (QPalette::ColorGroup) v8_to_uint32 (Args[0]),
                        (QPalette::ColorRole) v8_to_uint32 (Args[1])));
            }
            else {

               brush = self->_to_gbrush (Args[1]);
               if (brush) {

                  pal->setBrush ((QPalette::ColorRole) v8_to_uint32 (Args[0]), *brush);
               }
               brush = new QBrush (pal->brush ((QPalette::ColorRole) v8_to_number (Args[0])));
            }
         }
         else if (Args.Length () == 3) {

            brush = self->_to_gbrush (Args[2]);
            if (brush) {

               pal->setBrush (
                  (QPalette::ColorGroup) v8_to_uint32 (Args[0]),
                  (QPalette::ColorRole) v8_to_uint32 (Args[1]),
                  *brush);
            }

            brush =
               new QBrush (
                  pal->brush (
                     (QPalette::ColorGroup) v8_to_uint32 (Args[0]),
                     (QPalette::ColorRole) v8_to_uint32 (Args[1])));
         }

         if (brush) { result = self->create_v8_gbrush (brush); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_palette_color (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QPalette *pal = self->_to_qpalette (Args.This ());
      if (pal) {

         int length = Args.Length ();
         if (length == 1) {

            result = qcolor_to_v8 (pal->color ((QPalette::ColorRole) v8_to_number (Args[0])));
         }
         else if (length == 2) {

            if (Args[1]->IsNumber ()) {

               result =
                  qcolor_to_v8 (
                     pal->color (
                        (QPalette::ColorGroup) v8_to_uint32 (Args[0]),
                        (QPalette::ColorRole) v8_to_uint32 (Args[1])));
            }
            else {

               pal->setColor (
                  (QPalette::ColorRole) v8_to_uint32 (Args[0]),
                  v8_to_qcolor (Args[1]));
               result = qcolor_to_v8 (pal->color ((QPalette::ColorRole) v8_to_number (Args[0])));
            }
         }
         else if (length == 3) {

            pal->setColor (
               (QPalette::ColorGroup) v8_to_uint32 (Args[0]),
               (QPalette::ColorRole) v8_to_uint32 (Args[1]),
               v8_to_qcolor (Args[2]));

            result =
               qcolor_to_v8 (
                  pal->color (
                     (QPalette::ColorGroup) v8_to_uint32 (Args[0]),
                     (QPalette::ColorRole) v8_to_uint32 (Args[1])));
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_palette () {

   v8::HandleScope scope;

   _paletteTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());

   V8ObjectTemplate instance = _paletteTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _paletteTemp->PrototypeTemplate ();
   proto->Set ("color", v8::FunctionTemplate::New (_palette_color, _self));
   proto->Set ("brush", v8::FunctionTemplate::New (_palette_brush, _self));

   // ColorGroup
   _paletteApi.add_constant ("Disabled", (UInt32)QPalette::Disabled);
   _paletteApi.add_constant ("Active", (UInt32)QPalette::Active);
   _paletteApi.add_constant ("Inactive", (UInt32)QPalette::Inactive);

   // ColorRole
   _paletteApi.add_constant ("Window", (UInt32)QPalette::Window);
   _paletteApi.add_constant ("WindowText", (UInt32)QPalette::WindowText);
   _paletteApi.add_constant ("Base", (UInt32)QPalette::Base);
   _paletteApi.add_constant ("AlternateBase", (UInt32)QPalette::AlternateBase);
   _paletteApi.add_constant ("ToolTipBase", (UInt32)QPalette::ToolTipBase);
   _paletteApi.add_constant ("ToolTipText", (UInt32)QPalette::ToolTipText);
   _paletteApi.add_constant ("Text", (UInt32)QPalette::Text);
   _paletteApi.add_constant ("Button", (UInt32)QPalette::Button);
   _paletteApi.add_constant ("ButtonText", (UInt32)QPalette::ButtonText);
   _paletteApi.add_constant ("BrightText", (UInt32)QPalette::BrightText);
}
