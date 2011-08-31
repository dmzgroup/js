#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include "dmzV8QtWidget.h"
#include <QtGui/QWidget>
#include <QtGui/QLayout>

#include <QtGui/QPushButton>

dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_close (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         widget->close ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_enabled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length ()) {

            const Boolean Value = v8_to_boolean (Args[0]);
            widget->setEnabled (Value);
         }

         result = v8::Boolean::New (widget->isEnabled ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_hide (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) { widget->hide (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (Args.Length () > 0) {

         QLayout *layout = self->v8_to_qobject<QLayout> (Args[0]);
         if (layout) {

            if (v8_to_boolean (Args[1])) { // delete current layout

               QLayout *currentLayout = widget->layout ();
               if (currentLayout) { delete currentLayout; }
            }

            widget->setLayout (layout);
         }
      }

      result = self->create_v8_qobject (widget->layout ());
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_show (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) { widget->show (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_title (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length ()) {

            widget->setWindowTitle (v8_to_qstring (Args[0]));
         }

         result = qstring_to_v8 (widget->windowTitle ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_visible (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length ()) {

            widget->setVisible (v8_to_boolean (Args[0]));
         }

         result = v8::Boolean::New (widget->isVisible ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_window (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         QWidget *window = widget->window ();
         if (window) {

            result = self->create_v8_qwidget (window);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent (0);
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QWidget *widget = new QWidget (parent);
      result = self->create_v8_qobject (widget);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_rect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) { result = qrect_to_v8 (widget->rect()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_context_menu (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length ()) {

            Qt::ContextMenuPolicy policy = (Qt::ContextMenuPolicy)v8_to_uint32 (Args[0]);
            widget->setContextMenuPolicy (policy);
         }

         result = v8::Integer::New (widget->contextMenuPolicy ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_set_focus (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) { widget->setFocus (Qt::OtherFocusReason); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_size_policy (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget && (Args.Length () == 2)) {

         QSizePolicy::Policy horiz = (QSizePolicy::Policy)v8_to_uint32 (Args[0]);
         QSizePolicy::Policy vert = (QSizePolicy::Policy)v8_to_uint32 (Args[1]);
         widget->setSizePolicy (horiz, vert);
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_size (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length () == 2) {

            widget->resize (v8_to_uint32 (Args[0]), v8_to_uint32 (Args[1]));
         }
         QSize size = widget->size ();
         V8Array array = v8::Array::New (2);
         array->Set (v8::Integer::New (0), v8::Uint32::New (size.width ()));
         array->Set (v8::Integer::New (1), v8::Uint32::New (size.height ()));
         result = array;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_fixed_size (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length () == 2) {

            int width = v8_to_uint32 (Args[0]);
            int height = v8_to_uint32 (Args[1]);
            widget->resize (width, height);
            widget->setFixedSize (width, height);
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_fixed_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length () == 1) {

            widget->setFixedWidth (v8_to_uint32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_fixed_heigth (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length () == 1) {

            widget->setFixedHeight (v8_to_uint32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_style_sheet (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length () == 1) {

            widget->setStyleSheet (v8_to_qstring(Args[0]));
         }
         else if (Args.Length () == 0) {

            result = qstring_to_v8 (widget->styleSheet ());
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_width (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         result = v8::Integer::New (widget->width ());
      }
   }

   return scope.Close(result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_height (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         result = v8::Integer::New (widget->height ());
      }
   }

   return scope.Close(result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_palette (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length ()) {

            QPalette *pal = self->_to_qpalette (Args[0]);
            if (pal) { widget->setPalette (*pal); }
         }
         result = self->create_v8_palette (new QPalette (widget->palette ()));
      }
   }

   return scope.Close(result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_widget_fill_bg (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args.This ());
      if (widget) {

         if (Args.Length ()) { widget->setAutoFillBackground (v8_to_boolean (Args[0])); }
         result = v8::Boolean::New (widget->autoFillBackground ());
      }
   }

   return scope.Close(result);
}


void
dmz::JsModuleUiV8QtBasic::_init_widget () {

   v8::HandleScope scope;

   _widgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _widgetTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _widgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _widgetTemp->PrototypeTemplate ();
   proto->Set ("close", v8::FunctionTemplate::New (_widget_close, _self));
   proto->Set ("enabled", v8::FunctionTemplate::New (_widget_enabled, _self));
   proto->Set ("hide", v8::FunctionTemplate::New (_widget_hide, _self));
   proto->Set ("layout", v8::FunctionTemplate::New (_widget_layout, _self));
   proto->Set ("show", v8::FunctionTemplate::New (_widget_show, _self));
   proto->Set ("title", v8::FunctionTemplate::New (_widget_title, _self));
   proto->Set ("visible", v8::FunctionTemplate::New (_widget_visible, _self));
   proto->Set ("window", v8::FunctionTemplate::New (_widget_window, _self));
   proto->Set ("rect", v8::FunctionTemplate::New (_widget_rect, _self));
   proto->Set ("contextMenuPolicy", v8::FunctionTemplate::New (_widget_context_menu, _self));
   proto->Set ("setFocus", v8::FunctionTemplate::New (_widget_set_focus, _self));
   proto->Set ("sizePolicy", v8::FunctionTemplate::New (_widget_size_policy, _self));
   proto->Set ("size", v8::FunctionTemplate::New (_widget_size, _self));
   proto->Set ("fixedSize", v8::FunctionTemplate::New (_widget_fixed_size, _self));
   proto->Set ("fixedWidth", v8::FunctionTemplate::New (_widget_fixed_width, _self));
   proto->Set ("fixedHeigth", v8::FunctionTemplate::New (_widget_fixed_heigth, _self));
   proto->Set ("styleSheet", v8::FunctionTemplate::New (_widget_style_sheet, _self));
   proto->Set ("width", v8::FunctionTemplate::New (_widget_width, _self));
   proto->Set ("height", v8::FunctionTemplate::New (_widget_height, _self));
   proto->Set ("palette", v8::FunctionTemplate::New (_widget_palette, _self));
   proto->Set ("autoFillBackground", v8::FunctionTemplate::New (_widget_fill_bg, _self));

   _widgetApi.add_function ("create", _create_widget, _self);
}

