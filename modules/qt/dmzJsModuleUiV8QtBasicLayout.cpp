#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include <QtGui/QBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QFormLayout>
#include <QtGui/QWidget>


//QLayout *
//dmz::JsModuleUiV8QtBasic::_to_qlayout (V8Value value) {

//   QLayout *result (0);
//   QObject *object = _to_qobject (value);
//   if (object) { result = qobject_cast<QLayout *>(object); }
//   return result;
//}


//QBoxLayout *
//dmz::JsModuleUiV8QtBasic::_to_qboxlayout (V8Value value) {

//   QBoxLayout *result (0);
//   QObject *object = _to_qobject (value);
//   if (object) { result = qobject_cast<QBoxLayout *>(object); }
//   return result;
//}


//QGridLayout *
//dmz::JsModuleUiV8QtBasic::_to_qgridlayout (V8Value value) {

//   QGridLayout *result (0);
//   QObject *object = _to_qobject (value);
//   if (object) { result = qobject_cast<QGridLayout *>(object); }
//   return result;
//}


//QFormLayout *
//dmz::JsModuleUiV8QtBasic::_to_qformlayout (V8Value value) {

//   QFormLayout *result (0);
//   QObject *object = _to_qobject (value);
//   if (object) { result = qobject_cast<QFormLayout *>(object); }
//   return result;
//}

//dmz::V8Value
//dmz::JsModuleUiV8QtBasic::create_v8_layout (QLayout *value) {

//   v8::Context::Scope cscope (_state.context);
//   v8::HandleScope scope;

//   V8Value result = v8::Undefined ();

//   if (value) {

//      V8Object obj;

//      if (value->inherits ("QHBoxLayout")) {

//         if (!_hBoxLayoutCtor.IsEmpty ()) {

//            obj = _hBoxLayoutCtor->NewInstance ();
//         }
//      }
//      else if (value->inherits ("QVBoxLayout")) {

//         if (!_vBoxLayoutCtor.IsEmpty ()) {

//            obj = _vBoxLayoutCtor->NewInstance ();
//         }
//      }
//      else if (value->inherits ("QBoxLayout")) {

//         if (!_boxLayoutCtor.IsEmpty ()) {

//            obj = _boxLayoutCtor->NewInstance ();
//         }
//      }
//      else if (value->inherits ("QGridLayout")) {

//         if (!_gridLayoutCtor.IsEmpty ()) {

//            obj = _gridLayoutCtor->NewInstance ();
//         }
//      }
//      else if (value->inherits ("QFormLayout")) {

//         if (!_formLayoutCtor.IsEmpty ()) {

//            obj = _formLayoutCtor->NewInstance ();
//         }
//      }
//      else if (value->inherits ("QLayout")) {

//         if (!_layoutCtor.IsEmpty ()) {

//            obj = _layoutCtor->NewInstance ();
//         }
//      }

//      if (!obj.IsEmpty ()) {

//         obj->SetInternalField (0, v8::External::Wrap ((void *)value));
//         result = obj;
//      }
//   }

//   return scope.Close (result);
//}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_layout_index_of (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLayout *layout = self->v8_to_qobject<QLayout> (Args.This ());
      if (layout) {

         QWidget *widget = self->_to_qwidget (Args[0]);
         if (widget) {

            result = v8::Number::New (layout->indexOf (widget));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_layout_enabled (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLayout *layout = self->v8_to_qobject<QLayout> (Args.This ());
      if (layout) {

         if (Args.Length () > 0) {

            bool val = v8_to_boolean (Args[0]);
            layout->setEnabled (val);
         }

         result = v8::Boolean::New (layout->isEnabled ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_layout_at (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLayout *layout = self->v8_to_qobject<QLayout> (Args.This ());
      if (layout) {

         if (Args.Length () > 0) {

            QWidget *widget = layout->itemAt (v8_to_int32 (Args[0]))->widget ();
            if (widget) {

               result = self->create_v8_qwidget (widget);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_layout_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QLayout *layout = self->v8_to_qobject<QLayout> (Args.This ());
      if (layout) {

         result = v8::Number::New (layout->count ());
      }
   }

   return scope.Close (result);
}


//dmz::V8Value
//dmz::JsModuleUiV8QtBasic::_layout_parent (const v8::Arguments &Args) {

//   v8::HandleScope scope;
//   V8Value result = v8::Undefined ();

//   JsModuleUiV8QtBasic *self = _to_self (Args);
//   if (self) {

//      QLayout *layout = self->_to_qt_layout (Args.This ());
//      if (layout) {

//         QObject *obj = layout->parent ();
//         QWidget *parent_widget = qobject_cast<QWidget *>(obj);
//         QLayout *parent_layout = qobject_cast<QLayout *>(obj);

//         if (parent_widget) { result = self->create_v8_qwidget (parent_widget); }
//         else if (parent_layout) { result = self->create_v8_layout (parent_layout); }
//      }
//   }

//   return scope.Close (result);
//}


//dmz::V8Value
//dmz::JsModuleUiV8QtBasic::_layout_parent_widget (const v8::Arguments &Args) {

//   v8::HandleScope scope;
//   V8Value result = v8::Undefined ();

//   JsModuleUiV8QtBasic *self = _to_self (Args);

//   if (self) {

//      QLayout *layout = self->_to_qt_layout (Args.This ());

//      if (layout) {

//         QWidget *widget = layout->parentWidget ();
//         if (widget) {

//            result = self->create_v8_qwidget (widget);
//         }
//      }
//   }

//   return scope.Close (result);
//}


void
dmz::JsModuleUiV8QtBasic::_init_layout () {

   v8::HandleScope scope;

   _layoutTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _layoutTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _layoutTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _layoutTemp->PrototypeTemplate ();
   proto->Set ("indexOf", v8::FunctionTemplate::New (_layout_index_of, _self));
   proto->Set ("enabled", v8::FunctionTemplate::New (_layout_enabled, _self));
   proto->Set ("at", v8::FunctionTemplate::New (_layout_at, _self));
   proto->Set ("count", v8::FunctionTemplate::New (_layout_count, _self));
//   proto->Set ("parent", v8::FunctionTemplate::New (_layout_parent, _self));
//   proto->Set ("parentWidget", v8::FunctionTemplate::New (_layout_parent_widget, _self));
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_box_layout_add_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QBoxLayout *layout = self->v8_to_qobject<QBoxLayout> (Args.This ());
      if (layout) {

         if (Args.Length () > 0) {

            QLayout *newLayout = self->v8_to_qobject<QLayout> (Args[0]);
            if (newLayout) {

               layout->addLayout (newLayout);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_box_layout_add_stretch (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QBoxLayout *layout = self->v8_to_qobject<QBoxLayout> (Args.This ());
      if (layout) {

         if (Args.Length () > 0) {

            layout->addStretch (v8_to_int32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_box_layout_add_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QBoxLayout *layout = self->v8_to_qobject<QBoxLayout> (Args.This ());
      if (layout) {

         if (Args.Length () > 0) {

            QWidget *widget = self->_to_qwidget (Args[0]);
            if (widget) { layout->addWidget (widget); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_box_layout_direction (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QBoxLayout *layout = self->v8_to_qobject<QBoxLayout> (Args.This ());
      if (layout) {

         if (Args.Length () > 0) {

            int direction = v8_to_int32 (Args[0]);
            if ((direction < 0) || (direction > 3)) {

               direction = 0;
            }

            layout->setDirection ((QBoxLayout::Direction)direction);
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_box_layout () {

   v8::HandleScope scope;

   _boxLayoutTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _boxLayoutTemp->Inherit (_layoutTemp);

   V8ObjectTemplate instance = _boxLayoutTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _boxLayoutTemp->PrototypeTemplate ();
   proto->Set ("addLayout", v8::FunctionTemplate::New (_box_layout_add_layout, _self));
   proto->Set ("addStretch", v8::FunctionTemplate::New (_box_layout_add_stretch, _self));
   proto->Set ("addWidget", v8::FunctionTemplate::New (_box_layout_add_widget, _self));
   proto->Set ("direction", v8::FunctionTemplate::New (_box_layout_direction, _self));

   _layoutApi.add_constant ("LeftToRight", (UInt32)QBoxLayout::LeftToRight);
   _layoutApi.add_constant ("RightToLeft", (UInt32)QBoxLayout::RightToLeft);
   _layoutApi.add_constant ("TopToBottom", (UInt32)QBoxLayout::TopToBottom);
   _layoutApi.add_constant ("BottomToTop", (UInt32)QBoxLayout::BottomToTop);
   _layoutApi.add_function ("createBoxLayout", _create_box_layout, _self);
}


void
dmz::JsModuleUiV8QtBasic::_init_hbox_layout () {

   v8::HandleScope scope;

   _hBoxLayoutTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _hBoxLayoutTemp->Inherit (_boxLayoutTemp);

   V8ObjectTemplate instance = _hBoxLayoutTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   _layoutApi.add_function ("createHBoxLayout", _create_hbox_layout, _self);
}


void
dmz::JsModuleUiV8QtBasic::_init_vbox_layout () {

   v8::HandleScope scope;

   _vBoxLayoutTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _vBoxLayoutTemp->Inherit (_boxLayoutTemp);

   V8ObjectTemplate instance = _vBoxLayoutTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   _layoutApi.add_function ("createVBoxLayout", _create_vbox_layout, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_box_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent (0);
      int direction = 0;
      if (Args.Length () > 0) {

         direction = v8_to_int32 (Args[0]);
         if ((direction < 0) || (direction > 3)) {

            direction = 0;
         }
      }

      if (Args.Length () > 1) {

         parent = self->_to_qwidget (Args[1]);
      }

      QBoxLayout *layout = new QBoxLayout((QBoxLayout::Direction)direction, parent);
      result = self->create_v8_qobject (layout);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_hbox_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = self->_to_qwidget (Args[0]);

      QHBoxLayout *layout = new QHBoxLayout(parent);
      result = self->create_v8_qobject (layout);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_vbox_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = self->_to_qwidget (Args[0]);

      QVBoxLayout *layout = new QVBoxLayout(parent);
      result = self->create_v8_qobject (layout);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_grid_layout_add_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGridLayout *grid = self->v8_to_qobject<QGridLayout> (Args.This ());
      if (grid) {

         QLayout *layout = self->v8_to_qobject<QLayout> (Args[0]);
         if (grid && layout) {

            int row = v8_to_int32 (Args[1]);
            int column = v8_to_int32 (Args[2]);
            int rowSpan = -1;
            int columnSpan = -1;

            if (Args.Length () > 3) {

               rowSpan = v8_to_int32 (Args[3]);
               columnSpan = v8_to_int32 (Args[4]);
            }

            if ((row >= 0) && (column >= 0)) {

               if ((rowSpan < 0) || (columnSpan < 0)) {

                  grid->addLayout (layout, row, column);
               }
               else { grid->addLayout (layout, row, column, rowSpan, columnSpan); }
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_grid_layout_add_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGridLayout *grid = self->v8_to_qobject<QGridLayout> (Args.This ());
      QWidget *widget = self->_to_qwidget (Args[0]);

      if (grid && widget) {

         int row = v8_to_int32 (Args[1]);
         int column = v8_to_int32 (Args[2]);
         int rowSpan = -1;
         int columnSpan = -1;

         if (Args.Length () > 3) {

            rowSpan = v8_to_int32 (Args[3]);
            columnSpan = v8_to_int32 (Args[4]);
         }

         if ((row >= 0) && (column >= 0)) {

            if ((rowSpan < 0) || (columnSpan < 0)) {

               grid->addWidget (widget, row, column, 0);
            }
            else {

               grid->addWidget (widget, row, column, rowSpan, columnSpan);
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_grid_layout_column_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGridLayout *grid = self->v8_to_qobject<QGridLayout> (Args.This ());
      if (grid) {

         result = v8::Number::New (grid->columnCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_grid_layout_row_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGridLayout *grid = self->v8_to_qobject<QGridLayout> (Args.This ());
      if (grid) {

         result = v8::Number::New (grid->rowCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_grid_layout_column_stretch (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGridLayout *grid = self->v8_to_qobject<QGridLayout> (Args.This ());
      if (grid) {

         if (Args.Length () > 1) {

            int column = v8_to_int32 (Args[0]);
            int stretch = v8_to_int32 (Args[1]);
            grid->setColumnStretch (column, stretch);
         }
         else if (Args.Length () == 1){

            result = v8::Number::New (grid->columnStretch (v8_to_int32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_grid_layout_row_stretch (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QGridLayout *grid = self->v8_to_qobject<QGridLayout> (Args.This ());
      if (grid) {

         if (Args.Length () > 1) {

            int row = v8_to_int32 (Args[0]);
            int stretch = v8_to_int32 (Args[1]);
            grid->setColumnStretch (row, stretch);
         }
         else if (Args.Length () == 1) {

            result = v8::Number::New (grid->rowStretch (v8_to_int32 (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_grid_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = self->_to_qwidget (Args[0]);

      QGridLayout *layout = new QGridLayout(parent);
      result = self->create_v8_qobject (layout);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_grid_layout () {

   v8::HandleScope scope;

   _gridLayoutTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _gridLayoutTemp->Inherit (_layoutTemp);

   V8ObjectTemplate instance = _gridLayoutTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _gridLayoutTemp->PrototypeTemplate ();
   proto->Set ("addLayout", v8::FunctionTemplate::New (_grid_layout_add_layout, _self));
   proto->Set ("addWidget", v8::FunctionTemplate::New (_grid_layout_add_widget, _self));
   proto->Set ("columnCount", v8::FunctionTemplate::New (_grid_layout_column_count, _self));
   proto->Set ("rowCount", v8::FunctionTemplate::New (_grid_layout_row_count, _self));
   proto->Set ("columnStretch", v8::FunctionTemplate::New (_grid_layout_column_stretch, _self));
   proto->Set ("rowStretch", v8::FunctionTemplate::New (_grid_layout_row_stretch, _self));

   _layoutApi.add_function ("createGridLayout", _create_grid_layout, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_form_layout_add_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QFormLayout *form = self->v8_to_qobject<QFormLayout> (Args.This ());
      if (form) {

         QLayout *layout = 0;
         QWidget *widget = 0;

         if (Args.Length () == 1) {

            layout = self->v8_to_qobject<QLayout> (Args[0]);
            widget = self->_to_qwidget (Args[0]);

            if (layout) { form->addRow (layout); }
            else if (widget) { form->addRow (widget); }
         }
         else if (Args.Length () == 2) {

            widget = self->_to_qwidget (Args[0]);
            QWidget *field = self->_to_qwidget (Args[1]);
            layout = self->v8_to_qobject<QLayout> (Args[1]);

            String text = v8_to_string (Args[0]);

            if (widget) {

               if (field) { form->addRow (widget, field); }
               else if (layout) { form->addRow (widget, layout); }
            }
            else if (text) {

               if (field) { form->addRow (text.get_buffer (), field); }
               else if (layout) { form->addRow (text.get_buffer (), layout); }
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_form_layout_insert_row (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QFormLayout *form = self->v8_to_qobject<QFormLayout> (Args.This ());
      if (form) {

         QLayout *layout = 0;
         QWidget *widget = 0;

         int row = v8_to_int32 (Args[0]);
         if (Args.Length () == 2) {

            layout = self->v8_to_qobject<QLayout> (Args[1]);
            widget = self->_to_qwidget (Args[1]);

            if (layout) { form->addRow (layout); }
            else if (widget) { form->addRow (widget); }
         }
         else if (Args.Length () == 3) {

            String text = v8_to_string (Args[1]);
            widget = self->_to_qwidget (Args[1]);
            QWidget *field = self->_to_qwidget (Args[2]);
            layout = self->v8_to_qobject<QLayout> (Args[2]);

            if (widget) {

               if (field) { form->insertRow (row, widget, field); }
               else if (layout) { form->insertRow (row, widget, layout); }
            }
            else if (text) {

               if (field) { form->insertRow (row, text.get_buffer (), field); }
               else if (layout) {

                  form->insertRow (row, text.get_buffer (), layout);
               }
            }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_form_layout_row_count (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *widget = self->_to_qwidget (Args[0]);

      QFormLayout *layout = new QFormLayout (widget);
      if (layout) {

         result = v8::Number::New (layout->rowCount ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_form_layout_vertical_spacing (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QFormLayout *form = self->v8_to_qobject<QFormLayout> (Args.This ());
      if (form) {

         if (Args.Length () == 0) {

            result = v8::Number::New (form->verticalSpacing ());
         }
         else if (Args.Length () == 1) {

            form->setVerticalSpacing (v8_to_int32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_form_layout_spacing (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QFormLayout *form = self->v8_to_qobject<QFormLayout> (Args.This ());
      if (form) {

         if (Args.Length () == 0) {

            result = v8::Number::New (form->spacing ());
         }
         else if (Args.Length () == 1) {

            form->setSpacing (v8_to_int32 (Args[0]));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_form_layout (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent = self->_to_qwidget (Args[0]);

      QFormLayout *layout = new QFormLayout(parent);
      result = self->create_v8_qobject (layout);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_form_layout () {

   v8::HandleScope scope;

   _formLayoutTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _formLayoutTemp->Inherit (_layoutTemp);

   V8ObjectTemplate instance = _formLayoutTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _formLayoutTemp->PrototypeTemplate ();
   proto->Set ("addRow", v8::FunctionTemplate::New (_form_layout_add_row, _self));
   proto->Set ("insertRow", v8::FunctionTemplate::New (_form_layout_insert_row, _self));
   proto->Set ("rowCount", v8::FunctionTemplate::New (_form_layout_row_count, _self));
   proto->Set ("verticalSpacing", v8::FunctionTemplate::New (_form_layout_vertical_spacing, _self));
   proto->Set ("spacing", v8::FunctionTemplate::New (_form_layout_spacing, _self));

   _layoutApi.add_function ("createFormLayout", _create_form_layout, _self);
}
