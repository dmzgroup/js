#ifndef DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
#define DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsModuleUiV8Qt.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeTimeSlice.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesStringContainer.h>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsItem>
#include <QtCore/QCryptographicHash>
#include <v8.h>

class QInputDialog;
class QMessageBox;

namespace dmz {

   class JsModuleUiV8QtBasic;
   class JsModuleV8;
   class QtModuleMainWindow;
   class V8QtObject;
   class V8QtDialog;
   class V8QtWidget;
   struct V8ValueRef;


   struct JsModuleUiV8QtBasicState {

      JsModuleV8 *core;
      JsModuleUiV8QtBasic *ui;
      QtModuleMainWindow *mainWindowModule;

      v8::Handle<v8::Context> context;

      JsModuleUiV8QtBasicState () :
            core (0),
            ui (0),
            mainWindowModule (0) {;}
   };


   class JsModuleUiV8QtBasic :
         public QObject,
         public Plugin,
         public TimeSlice,
         public JsModuleUiV8Qt,
         public JsExtV8 {

      Q_OBJECT

      public:
         JsModuleUiV8QtBasic (const PluginInfo &Info, Config &local);
         ~JsModuleUiV8QtBasic ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // TimeSlice Interface
         virtual void update_time_slice (const Float64 DeltaTime);

         // JsModuleUiV8Qt Interface
         virtual v8::Handle<v8::Value> create_v8_qobject (QObject *value);
         virtual v8::Handle<v8::Value> create_v8_qwidget (QWidget *value);

         virtual v8::Handle<v8::Value> create_v8_qlistwidgetitem (
            QListWidgetItem *value);

         virtual v8::Handle<v8::Value> create_v8_qtreewidgetitem (
            QTreeWidgetItem *value);

         virtual v8::Handle<v8::Value> create_v8_qtablewidgetitem (
            QTableWidgetItem *value);

         virtual v8::Handle<v8::Value> create_v8_qtablewidgetselectionrange (
            QTableWidgetSelectionRange *value);

         virtual v8::Handle<v8::Value> create_v8_graphics_item (QGraphicsItem *value);
         virtual v8::Handle<v8::Value> create_v8_gbrush (QBrush *value);
         virtual v8::Handle<v8::Value> create_v8_gpen (QPen *value);
         virtual v8::Handle<v8::Value> create_v8_gpainter_path (QPainterPath *value);
         virtual v8::Handle<v8::Value> create_v8_gpainter (QPainter *value);
         virtual v8::Handle<v8::Value> create_v8_gimage (QImage *value);
         virtual v8::Handle<v8::Value> create_v8_gpixmap (QPixmap *value);
         virtual v8::Handle<v8::Value> create_v8_gpaint_device (QPaintDevice *value);
         virtual v8::Handle<v8::Value> create_v8_crypto_hash (QCryptographicHash *value);

         virtual v8::Handle<v8::Value> create_v8_qevent (QEvent *value);



         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

         template <class T> T *
         v8_to_qobject (V8Value value) {

            T *result (0);
            QObject *object = _to_qobject (value);
            if (object) { result = qobject_cast<T *>(object); }
            return result;
         }

         void v8_qt_widget_destroyed (V8QtWidget *widget);

      protected:
         struct ObsStruct {

            const Handle Observer;
            QList<V8QtObject *>list;

            ObsStruct (const Handle TheObserver) : Observer (TheObserver) {;}
            ~ObsStruct () { list.clear (); }
         };

         // Static Functions
         static JsModuleUiV8QtBasic *_to_self (const v8::Arguments &Args);

         // QUiLoader bindings
         static V8Value _uiloader_load (const v8::Arguments &Args);

         // QObject bindings implemented in JsModuleUiV8QtBasicObject.cpp
         static V8Value _object_lookup (const v8::Arguments &Args);
         static V8Value _object_name (const v8::Arguments &Args);
         static V8Value _object_observe (const v8::Arguments &Args);
         static V8Value _object_event_filter (const v8::Arguments &Args);
         static V8Value _object_delete_event_filter (const v8::Arguments &Args);
         static V8Value _object_parent (const v8::Arguments &Args);
         static V8Value _object_property (const v8::Arguments &Args);
         static V8Value _object_callback (const v8::Arguments &Args);
         static V8Value _object_inherits (const v8::Arguments &Args);
         static V8Value _object_to_string (const v8::Arguments &Args);

         // QWidget bindings implemented in JsModuleUiV8QtBasicWidget.cpp
         static V8Value _widget_close (const v8::Arguments &Args);
         static V8Value _widget_enabled (const v8::Arguments &Args);
         static V8Value _widget_hide (const v8::Arguments &Args);
         static V8Value _widget_layout (const v8::Arguments &Args);
         static V8Value _widget_show (const v8::Arguments &Args);
         static V8Value _widget_title (const v8::Arguments &Args);
         static V8Value _widget_visible (const v8::Arguments &Args);
         static V8Value _widget_window (const v8::Arguments &Args);
         static V8Value _widget_rect (const v8::Arguments &Args);
         static V8Value _widget_context_menu (const v8::Arguments &Args);
         static V8Value _widget_set_focus (const v8::Arguments &Args);
         static V8Value _widget_size_policy (const v8::Arguments &Args);
         static V8Value _widget_size (const v8::Arguments &Args);
         static V8Value _widget_fixed_size (const v8::Arguments &Args);
         static V8Value _widget_set_style_sheet (const v8::Arguments &Args);
         static V8Value _create_widget (const v8::Arguments &Args);

         // QAbstractButton bindings implemented in JsModuleUiV8QtBasicButton.cpp
         static V8Value _button_text (const v8::Arguments &Args);
         static V8Value _button_is_checked (const v8::Arguments &Args);
         static V8Value _button_set_checked (const v8::Arguments &Args);
         static V8Value _button_click (const v8::Arguments &Args);
         static V8Value _button_std_icon (const v8::Arguments &Args);
         static V8Value _create_push_button (const v8::Arguments &Args);
         static V8Value _create_radio_button (const v8::Arguments &Args);
         static V8Value _create_check_box (const v8::Arguments &Args);
//         static V8Value _create_tool_button (const v8::Arguments &Args);

         // QDialog bindings implemented in JsModuleUiV8QtBasicDialog.cpp
         static V8Value _dialog_open (const v8::Arguments &Args);
         static V8Value _dialog_accept (const v8::Arguments &Args);
         static V8Value _dialog_reject (const v8::Arguments &Args);

         // QSpinBox bindings implemented in JsModuleUiV8QtBasicSpinBox.cpp
         static V8Value _spinbox_maximum (const v8::Arguments &Args);
         static V8Value _spinbox_minimum (const v8::Arguments &Args);
         static V8Value _spinbox_value (const v8::Arguments &Args);
         static V8Value _spinbox_text (const v8::Arguments &Args);
         static V8Value _spinbox_set_single_step (const v8::Arguments &Args);
         static V8Value _spinbox_set_suffix (const v8::Arguments &Args);
         static V8Value _create_spinbox (const v8::Arguments &Args);
         static V8Value _create_dspinbox (const v8::Arguments &Args);

         // QComboBox bindings implemented in JsModuleUiV8QtBasicComboBox.cpp
         static V8Value _combobox_count (const v8::Arguments &Args);
         static V8Value _combobox_current_index (const v8::Arguments &Args);
         static V8Value _combobox_current_text (const v8::Arguments &Args);
         static V8Value _combobox_add_item (const v8::Arguments &Args);
         static V8Value _combobox_add_items (const v8::Arguments &Args);
         static V8Value _combobox_find_text (const v8::Arguments &Args);
         static V8Value _combobox_remove_item (const v8::Arguments &Args);
         static V8Value _combobox_clear (const v8::Arguments &Args);
         static V8Value _combobox_item_text (const v8::Arguments &Args);
         static V8Value _combobox_insert_item (const v8::Arguments &Args);
         static V8Value _combobox_item_data (const v8::Arguments &Args);
         static V8Value _create_comboBox (const v8::Arguments &Args);

         // QSlider bindings implemented in JsModuleUiV8QtBasicSlider.cpp
         static V8Value _slider_maximum (const v8::Arguments &Args);
         static V8Value _slider_minimum (const v8::Arguments &Args);
         static V8Value _slider_value (const v8::Arguments &Args);
         static V8Value _slider_is_down (const v8::Arguments &Args);
         static V8Value _create_slider (const v8::Arguments &Args);

         // QDial bindings implemented in JsModuleUiV8QtBasicSlider.cpp
         static V8Value _dial_wrapping (const v8::Arguments &Args);
         static V8Value _dial_notches_visible (const v8::Arguments &Args);
         static V8Value _create_dial (const v8::Arguments &Args);

         // QLineEdit bindings implemented in JsModuleUiV8QtBasicLineEdit.cpp
         static V8Value _lineEdit_text (const v8::Arguments &Args);
         static V8Value _lineEdit_clear (const v8::Arguments &Args);
         static V8Value _lineEdit_undo (const v8::Arguments &Args);
         static V8Value _lineEdit_redo (const v8::Arguments &Args);
         static V8Value _lineEdit_readonly (const v8::Arguments &Args);
         static V8Value _create_line_edit (const v8::Arguments &Args);

         // QTextEdit bindings implemented in JsModuleUiV8QtBasicTextEdit.cpp
         static V8Value _textEdit_append (const v8::Arguments &Args);
         static V8Value _textEdit_clear (const v8::Arguments &Args);
         static V8Value _textEdit_redo (const v8::Arguments &Args);
         static V8Value _textEdit_undo (const v8::Arguments &Args);
         static V8Value _textEdit_text (const v8::Arguments &Args);
         static V8Value _textEdit_allow_undo (const v8::Arguments &Args);
         static V8Value _textEdit_read_only (const v8::Arguments &Args);
         static V8Value _create_text_edit (const v8::Arguments &Args);

         // QLabel bindings implemented in JsModuleUiV8QtLabel.cpp
         static V8Value _label_text (const v8::Arguments &Args);
         static V8Value _label_word_wrap (const v8::Arguments &Args);
         static V8Value _label_clear (const v8::Arguments &Args);
         static V8Value _label_pixmap (const v8::Arguments &Args);
         static V8Value _create_label (const v8::Arguments &Args);

         // QProgressBar bindings implemented in JsModuleUiV8QtLabel.cpp
         static V8Value _progress_bar_maximum (const v8::Arguments &Args);
         static V8Value _progress_bar_minimum (const v8::Arguments &Args);
         static V8Value _progress_bar_text (const v8::Arguments &Args);
         static V8Value _progress_bar_value (const v8::Arguments &Args);
         static V8Value _progress_bar_reset (const v8::Arguments &Args);
         static V8Value _create_pbar (const v8::Arguments &Args);

         // QLCDNumber bindings implemented in JsModuleUiV8QtLCDNumber.cpp
         static V8Value _lcd_number_value (const v8::Arguments &Args);
         static V8Value _create_lcd (const v8::Arguments &Args);

         // QListWidgetItem bindings implemented in JsModuleUiV8QtBasicListWidget.cpp
         static V8Value _list_widget_item_text (const v8::Arguments &Args);
         static V8Value _list_widget_item_data (const v8::Arguments &Args);
         static V8Value _list_widget_item_hidden (const v8::Arguments &Args);
         static V8Value _list_widget_item_bg_brush (const v8::Arguments &Args);

         // QListWidget bindings implemented in JsModuleUiV8QtBasicListWidget.cpp
         static V8Value _list_widget_add_item (const v8::Arguments &Args);
         static V8Value _list_widget_current_item (const v8::Arguments &Args);
         static V8Value _list_widget_clear (const v8::Arguments &Args);
         static V8Value _list_widget_count (const v8::Arguments &Args);
         static V8Value _list_widget_item (const v8::Arguments &Args);
         static V8Value _list_widget_row (const v8::Arguments &Args);
         static V8Value _list_widget_take_item (const v8::Arguments &Args);
         static V8Value _list_widget_remove_item (const v8::Arguments &Args);
         static V8Value _list_widget_find_items (const v8::Arguments &Args);
         static V8Value _create_list (const v8::Arguments &Args);

         // QScrollArea bindings implemented in JsModuleUiV8QtBasicScrollArea.cpp
         static V8Value _scroll_area_widget (const v8::Arguments &Args);
         static V8Value _scroll_area_ensure_visible (const v8::Arguments &Args);
         static V8Value _scroll_area_widget_resizable (const v8::Arguments &Args);
         static V8Value _create_scroll_area (const v8::Arguments &Args);

         // QStackedWidget bindings implemented in JsModuleUiV8QtBasicStackedWidget.cpp
         static V8Value _stacked_widget_add (const v8::Arguments &Args);
         static V8Value _stacked_widget_remove (const v8::Arguments &Args);
         static V8Value _stacked_widget_count (const v8::Arguments &Args);
         static V8Value _stacked_widget_current_index (const v8::Arguments &Args);
         static V8Value _stacked_widget_current_widget (const v8::Arguments &Args);
         static V8Value _stacked_widget_at (const v8::Arguments &Args);
         static V8Value _stacked_widget_index_of (const v8::Arguments &Args);
         static V8Value _create_stacked_widget (const v8::Arguments &Args);

         // QTabWidget bindings implemented in JsModuleUiV8QtBasicTabWidget.cpp
         static V8Value _tab_widget_add (const v8::Arguments &Args);
         static V8Value _tab_widget_remove (const v8::Arguments &Args);
         static V8Value _tab_widget_current_widget (const v8::Arguments &Args);
         static V8Value _tab_widget_current_index (const v8::Arguments &Args);
         static V8Value _tab_widget_count (const v8::Arguments &Args);
         static V8Value _tab_widget_index_of (const v8::Arguments &Args);
         static V8Value _tab_widget_tab_text (const v8::Arguments &Args);
         static V8Value _tab_widget_at (const v8::Arguments &Args);
         static V8Value _tab_widget_clear (const v8::Arguments &Args);
         static V8Value _create_tab_widget (const v8::Arguments &Args);

         // QMessageBox bindings implemented in JsModuleUiV8QtBasicMessageBox.cpp
         static V8Value _create_message_box (const v8::Arguments &Args);
         QMessageBox *_create_message_box (V8Object params, QWidget *parent);

         // QLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _layout_index_of (const v8::Arguments &Args);
         static V8Value _layout_enabled (const v8::Arguments &Args);
         static V8Value _layout_at (const v8::Arguments &Args);
         static V8Value _layout_count (const v8::Arguments &Args);
         static V8Value _layout_remove_layout (const v8::Arguments &Args);
         static V8Value _layout_remove_item (const v8::Arguments &Args);
         static V8Value _layout_remove_widget (const v8::Arguments &Args);
         static V8Value _layout_take_at (const v8::Arguments &Args);
         static V8Value _layout_contents_margins (const v8::Arguments &Args);
         static V8Value _layout_parent (const v8::Arguments &Args);
         static V8Value _layout_parent_widget (const v8::Arguments &Args);

         // QBoxLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _box_layout_add_layout (const v8::Arguments &Args);
         static V8Value _box_layout_insert_layout (const v8::Arguments &Args);
         static V8Value _box_layout_add_stretch (const v8::Arguments &Args);
         static V8Value _box_layout_add_widget (const v8::Arguments &Args);
         static V8Value _box_layout_insert_widget (const v8::Arguments &Args);
         static V8Value _box_layout_direction (const v8::Arguments &Args);
         static V8Value _create_box_layout (const v8::Arguments &Args);
         static V8Value _create_hbox_layout (const v8::Arguments &Args);
         static V8Value _create_vbox_layout (const v8::Arguments &Args);

         // QGridLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _grid_layout_add_layout (const v8::Arguments &Args);
         static V8Value _grid_layout_add_widget (const v8::Arguments &Args);
         static V8Value _grid_layout_column_count (const v8::Arguments &Args);
         static V8Value _grid_layout_row_count (const v8::Arguments &Args);
         static V8Value _grid_layout_column_stretch (const v8::Arguments &Args);
         static V8Value _grid_layout_row_stretch (const v8::Arguments &Args);
         static V8Value _grid_layout_column_min (const v8::Arguments &Args);
         static V8Value _grid_layout_row_min (const v8::Arguments &Args);
         static V8Value _grid_layout_item_at (const v8::Arguments &Args);
         static V8Value _create_grid_layout (const v8::Arguments &Args);

         // QFormLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _form_layout_add_row (const v8::Arguments &Args);
         static V8Value _form_layout_insert_row (const v8::Arguments &Args);
         static V8Value _form_layout_row_count (const v8::Arguments &Args);
         static V8Value _form_layout_vertical_spacing (const v8::Arguments &Args);
         static V8Value _form_layout_spacing (const v8::Arguments &Args);
         static V8Value _form_layout_at (const v8::Arguments &Args);
         static V8Value _create_form_layout (const v8::Arguments &Args);

         // QFileDialog bindings implemented in JsModuleUiV8QtBasicFileDialog.cpp
         static V8Value _file_dialog_get_existing_directory (const v8::Arguments &Args);
         static V8Value _file_dialog_get_open_file_name (const v8::Arguments &Args);
         static V8Value _file_dialog_get_save_file_name (const v8::Arguments &Args);

         // QMainWindow bindings implemented in JsModuleUiV8QtBasicMainWindow.cpp
         static V8Value _main_window_central_widget (const v8::Arguments &Args);
         static V8Value _main_window_close (const v8::Arguments &Args);
         static V8Value _main_window_create_dock_widget (const v8::Arguments &Args);
         static V8Value _main_window_remove_dock_widget (const v8::Arguments &Args);
         static V8Value _main_window_add_menu (const v8::Arguments &Args);
         static V8Value _main_window_add_separator (const v8::Arguments &Args);
         static V8Value _main_window_window (const v8::Arguments &Args);
         static V8Value _main_window_status_bar (const v8::Arguments &Args);

         // QDockWidget bindings implemented in JsModuleUiV8QtBasicDockWidget.cpp
         static V8Value _create_dock_widget (const v8::Arguments &Args);
//         static V8Value _dock_window_widget (const v8::Arguments &Args);
//         static V8Value _dock_window_toggle_view_action (const v8::Arguments &Args);
         static V8Value _dock_widget_floating (const v8::Arguments &Args);

         // QFrame bindings implemented in JsModuleUiV8QtBasicFrame.cpp
//         static V8Value _frame_create (const v8::Arguments &Args);
         static V8Value _frame_shadow (const v8::Arguments &Args);
         static V8Value _frame_shape (const v8::Arguments &Args);
         static V8Value _frame_width (const v8::Arguments &Args);
         static V8Value _frame_line_width (const v8::Arguments &Args);
         static V8Value _frame_mid_line_width (const v8::Arguments &Args);

         // QGroupBox implemented in JsModuleUiV8QtBasicGroupBox.cpp
         static V8Value _group_box_flat (const v8::Arguments &Args);
         static V8Value _group_box_title (const v8::Arguments &Args);
         static V8Value _group_box_alignment (const v8::Arguments &Args);
         static V8Value _group_box_checkable (const v8::Arguments &Args);
         static V8Value _create_group_box (const v8::Arguments &Args);

         // QDateTimeEdit implemented in JsModulkeUiV8QtBasicDateTimeEdit.cpp
         static V8Value _dt_maximum (const v8::Arguments &Args);
         static V8Value _dt_minimum (const v8::Arguments &Args);
         static V8Value _dt_datetime (const v8::Arguments &Args);
         static V8Value _create_dte (const v8::Arguments &Args);

         // QAction bindings implemented in JsModuleUiV8QtBasicAction.cpp
         static V8Value _action_enabled (const v8::Arguments &Args);
         static V8Value _action_shortcut (const v8::Arguments &Args);
         static V8Value _action_text (const v8::Arguments &Args);
         static V8Value _action_trigger (const v8::Arguments &Args);

         // QTreeWidget bindings implemented in JsModuleUiV8QtBasicTreeWidget.cpp
         static V8Value _tree_clear (const v8::Arguments &Args);
         static V8Value _tree_header_labels (const v8::Arguments &Args);
         static V8Value _tree_col_width (const v8::Arguments &Args);
         static V8Value _tree_col_hidden (const v8::Arguments &Args);
         static V8Value _tree_word_wrap (const v8::Arguments &Args);
         static V8Value _tree_add (const v8::Arguments &Args);
         static V8Value _tree_col_count (const v8::Arguments &Args);
         static V8Value _tree_curr_col (const v8::Arguments &Args);
         static V8Value _tree_curr_item (const v8::Arguments &Args);
         static V8Value _tree_find_items (const v8::Arguments &Args);
         static V8Value _tree_item_above (const v8::Arguments &Args);
         static V8Value _tree_item_below (const v8::Arguments &Args);
         static V8Value _tree_item_widget (const v8::Arguments &Args);
         static V8Value _tree_rem_item_widget (const v8::Arguments &Args);
         static V8Value _tree_index_of (const v8::Arguments &Args);
         static V8Value _tree_selected_items (const v8::Arguments &Args);
         static V8Value _tree_sort_col (const v8::Arguments &Args);
         static V8Value _tree_take_item_at (const v8::Arguments &Args);
         static V8Value _tree_item_at (const v8::Arguments &Args);
         static V8Value _tree_item_count (const v8::Arguments &Args);
         static V8Value _tree_root (const v8::Arguments &Args);
         static V8Value _tree_expand_all (const v8::Arguments &Args);
         static V8Value _tree_collapse_all (const v8::Arguments &Args);
         static V8Value _tree_resize_col_contents (const v8::Arguments &Args);
         static V8Value _create_tree_widget (const v8::Arguments &Args);

         // QTreeWidgetItem bindings implemented in JsModuleUiV8QtBasicTreeWidget.cpp
         static V8Value _tree_item_add (const v8::Arguments &Args);
         static V8Value _tree_item_background (const v8::Arguments &Args);
         static V8Value _tree_item_child (const v8::Arguments &Args);
         static V8Value _tree_item_child_count (const v8::Arguments &Args);
         static V8Value _tree_item_col_count (const v8::Arguments &Args);
         static V8Value _tree_item_data (const v8::Arguments &Args);
         static V8Value _tree_item_parent (const v8::Arguments &Args);
         static V8Value _tree_item_hidden (const v8::Arguments &Args);
         static V8Value _tree_item_text (const v8::Arguments &Args);
         static V8Value _tree_item_sort_children (const v8::Arguments &Args);
         static V8Value _tree_item_take_child (const v8::Arguments &Args);
         static V8Value _tree_item_take_children (const v8::Arguments &Args);
         static V8Value _tree_item_tree_widget (const v8::Arguments &Args);
         static V8Value _tree_item_index_of (const v8::Arguments &Args);
         static V8Value _tree_item_disable (const v8::Arguments &Args);
         static V8Value _tree_item_expand (const v8::Arguments &Args);
         static V8Value _tree_item_collapse (const v8::Arguments &Args);
         static V8Value _tree_item_is_expanded (const v8::Arguments &Args);
         static V8Value _create_tree_item (const v8::Arguments &Args);

         // QTableWidget bindings implemented in JsModuleUiV8QtBasicTableWidget.cpp
         static V8Value _table_clear_spans (const v8::Arguments &Args);
         static V8Value _table_set_span (const v8::Arguments &Args);
         static V8Value _table_col_width (const v8::Arguments &Args);
         static V8Value _table_col_hidden (const v8::Arguments &Args);
         static V8Value _table_row_height (const v8::Arguments &Args);
         static V8Value _table_word_wrap (const v8::Arguments &Args);
         static V8Value _table_row_hidden (const v8::Arguments &Args);
         static V8Value _table_resize_single_col (const v8::Arguments &Args);
         static V8Value _table_resize_all_col (const v8::Arguments &Args);
         static V8Value _table_resize_single_row (const v8::Arguments &Args);
         static V8Value _table_resize_all_row (const v8::Arguments &Args);
         static V8Value _table_select_col (const v8::Arguments &Args);
         static V8Value _table_select_row (const v8::Arguments &Args);
         static V8Value _table_cell_widget (const v8::Arguments &Args);
         static V8Value _table_col (const v8::Arguments &Args);
         static V8Value _table_col_count (const v8::Arguments &Args);
         static V8Value _table_curr_col (const v8::Arguments &Args);
         static V8Value _table_curr_item (const v8::Arguments &Args);
         static V8Value _table_curr_row (const v8::Arguments &Args);
         static V8Value _table_find_items (const v8::Arguments &Args);
         static V8Value _table_horiz_header (const v8::Arguments &Args);
         static V8Value _table_item_at (const v8::Arguments &Args);
         static V8Value _table_remove_cell_widget (const v8::Arguments &Args);
         static V8Value _table_row (const v8::Arguments &Args);
         static V8Value _table_row_count (const v8::Arguments &Args);
         static V8Value _table_selected_items (const v8::Arguments &Args);
         static V8Value _table_selected_ranges (const v8::Arguments &Args);
         static V8Value _table_set_current_cell (const v8::Arguments &Args);
         static V8Value _table_set_h_labels (const v8::Arguments &Args);
         static V8Value _table_set_v_labels (const v8::Arguments &Args);
         static V8Value _table_set_range_selected (const v8::Arguments &Args);
         static V8Value _table_sort_items (const v8::Arguments &Args);
         static V8Value _table_take_item (const v8::Arguments &Args);
         static V8Value _table_clear (const v8::Arguments &Args);
         static V8Value _table_clear_contents (const v8::Arguments &Args);
         static V8Value _table_insert_col (const v8::Arguments &Args);
         static V8Value _table_insert_row (const v8::Arguments &Args);
         static V8Value _table_remove_col (const v8::Arguments &Args);
         static V8Value _table_remove_row (const v8::Arguments &Args);
         static V8Value _table_vert_header (const v8::Arguments &Args);
         static V8Value _create_table_widget (const v8::Arguments &Args);

         // QTableWidgetItem bindings implemented in JsModuleUiV8QtBasicTableWidget.cpp
         static V8Value _table_item_column (const v8::Arguments &Args);
         static V8Value _table_item_row (const v8::Arguments &Args);
         static V8Value _table_item_data (const v8::Arguments &Args);
         static V8Value _table_item_selected (const v8::Arguments &Args);
         static V8Value _table_item_text (const v8::Arguments &Args);

         // QTableWidgetSelectionRange bindings implemented in
         // JsModuleUiV8QtBasicTableWidget.cpp
         static V8Value _table_range_bottom (const v8::Arguments &Args);
         static V8Value _table_range_top (const v8::Arguments &Args);
         static V8Value _table_range_left (const v8::Arguments &Args);
         static V8Value _table_range_right (const v8::Arguments &Args);
         static V8Value _table_range_col_count (const v8::Arguments &Args);
         static V8Value _table_range_row_count (const v8::Arguments &Args);

         // QToolBox bindings implemented in JsModuleUiV8QtBasicToolBox.cpp
         static V8Value _toolbox_add_item (const v8::Arguments &Args);
         static V8Value _toolbox_clear (const v8::Arguments &Args);
         static V8Value _toolbox_count (const v8::Arguments &Args);
         static V8Value _toolbox_curr_index (const v8::Arguments &Args);
         static V8Value _toolbox_curr_widget (const v8::Arguments &Args);
         static V8Value _toolbox_index_of (const v8::Arguments &Args);
         static V8Value _toolbox_insert_item (const v8::Arguments &Args);
         static V8Value _toolbox_item_text (const v8::Arguments &Args);
         static V8Value _toolbox_remove_item (const v8::Arguments &Args);
         static V8Value _toolbox_widget (const v8::Arguments &Args);
         static V8Value _create_toolbox (const v8::Arguments &Args);

         // QMessageBox bindings implemented in JsModuleUiV8QtBasicMessageBox.cpp
         static V8Value _messagebox_text (const v8::Arguments &Args);
         static V8Value _messagebox_dtext (const v8::Arguments &Args);
         static V8Value _messagebox_itext (const v8::Arguments &Args);

         // QInputDialog bindings implemented in JsModuleUiV8QtBasicInputDialog.cpp
         static V8Value _create_input_dialog (const v8::Arguments &Args);
         static V8Value _input_dialog_ddec (const v8::Arguments &Args);
         static V8Value _input_dialog_dmax (const v8::Arguments &Args);
         static V8Value _input_dialog_dmin (const v8::Arguments &Args);
         static V8Value _input_dialog_dval (const v8::Arguments &Args);
         static V8Value _input_dialog_imax (const v8::Arguments &Args);
         static V8Value _input_dialog_imin (const v8::Arguments &Args);
         static V8Value _input_dialog_istep (const v8::Arguments &Args);
         static V8Value _input_dialog_ival (const v8::Arguments &Args);
         static V8Value _input_dialog_label_text (const v8::Arguments &Args);
         static V8Value _input_dialog_ok_btn_text (const v8::Arguments &Args);
         static V8Value _input_dialog_cancel_btn_text (const v8::Arguments &Args);
         static V8Value _input_dialog_tval (const v8::Arguments &Args);
         static V8Value _input_dialog_cbox_items (const v8::Arguments &Args);
         static V8Value _input_dialog_current (const v8::Arguments &Args);
         static V8Value _input_dialog_cbox_editable (const v8::Arguments &Args);
         QInputDialog *_create_input_dialog (V8Object params, QWidget *parent);

         // Graph bindings implemented in JsModuleUiV8QtBasicGraph.cpp
         static V8Value _create_grect_item (const v8::Arguments &Args);
         static V8Value _create_gtext_item (const v8::Arguments &Args);
         static V8Value _create_gline_item (const v8::Arguments &Args);
         static V8Value _create_gpath_item (const v8::Arguments &Args);
         static V8Value _create_gpixmap_item (const v8::Arguments &Args);
         static V8Value _create_gwidget (const v8::Arguments &Args);
         static V8Value _create_gwebview (const v8::Arguments &Args);
         static V8Value _create_gscene (const v8::Arguments &Args);
         static V8Value _create_gview (const v8::Arguments &Args);
         static V8Value _create_gbrush (const v8::Arguments &Args);
         static V8Value _create_gpen (const v8::Arguments &Args);
         static V8Value _create_gpainter_path (const v8::Arguments &Args);
         static V8Value _create_gpixmap (const v8::Arguments &Args);
         static V8Value _create_gimage (const v8::Arguments &Args);
         static V8Value _create_gpaint_device (const v8::Arguments &Args);
         static V8Value _create_gpainter (const v8::Arguments &Args);

         static V8Value _gitem_drops (const v8::Arguments &Args);
         static V8Value _gitem_brect (const v8::Arguments &Args);
         static V8Value _gitem_child_items (const v8::Arguments &Args);
         static V8Value _gitem_children_brect (const v8::Arguments &Args);
         static V8Value _gitem_clear_focus (const v8::Arguments &Args);
         static V8Value _gitem_collides_item (const v8::Arguments &Args);
         static V8Value _gitem_collides_path (const v8::Arguments &Args);
         static V8Value _gitem_collides_items (const v8::Arguments &Args);
         static V8Value _gitem_contains (const v8::Arguments &Args);
         static V8Value _gitem_data (const v8::Arguments &Args);
         static V8Value _gitem_ensure_vis (const v8::Arguments &Args);
         static V8Value _gitem_flag (const v8::Arguments &Args);
         static V8Value _gitem_focus_item (const v8::Arguments &Args);
         static V8Value _gitem_has_focus (const v8::Arguments &Args);
         static V8Value _gitem_hide (const v8::Arguments &Args);
         static V8Value _gitem_show (const v8::Arguments &Args);
         static V8Value _gitem_active (const v8::Arguments &Args);
         static V8Value _gitem_enabled (const v8::Arguments &Args);
         static V8Value _gitem_visible (const v8::Arguments &Args);
         static V8Value _gitem_parent_item (const v8::Arguments &Args);
         static V8Value _gitem_z (const v8::Arguments &Args);
         static V8Value _gitem_pos (const v8::Arguments &Args);
         static V8Value _gitem_spos (const v8::Arguments &Args);
         static V8Value _gitem_stack_before (const v8::Arguments &Args);
         static V8Value _gitem_rotation (const v8::Arguments &Args);
         static V8Value _gitem_cursor (const v8::Arguments &Args);
         static V8Value _gitem_tooltip (const v8::Arguments &Args);
         static V8Value _gitem_hover_events (const v8::Arguments &Args);

         static V8Value _gabs_brush (const v8::Arguments &Args);
         static V8Value _gabs_pen (const v8::Arguments &Args);

         static V8Value _grect_rect (const v8::Arguments &Args);

         static V8Value _gpath_path (const v8::Arguments &Args);

         static V8Value _gtext_adjust_size (const v8::Arguments &Args);
         static V8Value _gtext_text_color (const v8::Arguments &Args);
         static V8Value _gtext_font (const v8::Arguments &Args);
         static V8Value _gtext_plain_text (const v8::Arguments &Args);
         static V8Value _gtext_html_text (const v8::Arguments &Args);
         static V8Value _gtext_width (const v8::Arguments &Args);
         static V8Value _gtext_text_interact_flags (const v8::Arguments &Args);

         static V8Value _gline_line (const v8::Arguments &Args);
         static V8Value _gline_pen (const v8::Arguments &Args);

         static V8Value _gpixitem_pixmap (const v8::Arguments &Args);

         static V8Value _gview_alignment (const v8::Arguments &Args);
         static V8Value _gview_bg_brush (const v8::Arguments &Args);
         static V8Value _gview_cache (const v8::Arguments &Args);
         static V8Value _gview_center (const v8::Arguments &Args);
         static V8Value _gview_visible (const v8::Arguments &Args);
         static V8Value _gview_fg_brush (const v8::Arguments &Args);
         static V8Value _gview_interactive (const v8::Arguments &Args);
         static V8Value _gview_item_at (const v8::Arguments &Args);
         static V8Value _gview_items (const v8::Arguments &Args);
         static V8Value _gview_rotate (const v8::Arguments &Args);
         static V8Value _gview_scale (const v8::Arguments &Args);
         static V8Value _gview_scene (const v8::Arguments &Args);
         static V8Value _gview_scene_rect (const v8::Arguments &Args);
         static V8Value _gview_viewport_update_mode (const v8::Arguments &Args);
         static V8Value _gview_translate (const v8::Arguments &Args);
         static V8Value _gview_render (const v8::Arguments &Args);
         static V8Value _gview_viewport (const v8::Arguments &Args);

         static V8Value _gscene_add_item (const v8::Arguments &Args);
         static V8Value _gscene_add_line (const v8::Arguments &Args);
         static V8Value _gscene_add_path (const v8::Arguments &Args);
         static V8Value _gscene_add_rect (const v8::Arguments &Args);
         static V8Value _gscene_add_text (const v8::Arguments &Args);
         static V8Value _gscene_add_pixmap (const v8::Arguments &Args);
         static V8Value _gscene_bg_brush (const v8::Arguments &Args);
         static V8Value _gscene_clear_focus (const v8::Arguments &Args);
         static V8Value _gscene_colliding_items (const v8::Arguments &Args);
         static V8Value _gscene_focus_item (const v8::Arguments &Args);
         static V8Value _gscene_fg_brush (const v8::Arguments &Args);
         static V8Value _gscene_active_panel (const v8::Arguments &Args);
//         static V8Value _gscene_item_at (const v8::Arguments &Args);
         static V8Value _gscene_items (const v8::Arguments &Args);
         static V8Value _gscene_remove_item (const v8::Arguments &Args);
         static V8Value _gscene_scene_rect (const v8::Arguments &Args);
         static V8Value _gscene_selected_items (const v8::Arguments &Args);
         static V8Value _gscene_width (const v8::Arguments &Args);
         static V8Value _gscene_height (const v8::Arguments &Args);
         static V8Value _gscene_clear (const v8::Arguments &Args);

         static V8Value _gbrush_color (const v8::Arguments &Args);
         static V8Value _gbrush_style (const v8::Arguments &Args);

         static V8Value _gpen_brush (const v8::Arguments &Args);
         static V8Value _gpen_color (const v8::Arguments &Args);
         static V8Value _gpen_join_style (const v8::Arguments &Args);
         static V8Value _gpen_width (const v8::Arguments &Args);

         static V8Value _gpp_add_ellipse (const v8::Arguments &Args);
         static V8Value _gpp_add_path (const v8::Arguments &Args);
         static V8Value _gpp_add_rect (const v8::Arguments &Args);
         static V8Value _gpp_add_rrect (const v8::Arguments &Args);
//         static V8Value _gpp_add_text (const v8::Arguments &Args);
         static V8Value _gpp_angle_at_pct (const v8::Arguments &Args);
         static V8Value _gpp_arc_move_to (const v8::Arguments &Args);
         static V8Value _gpp_arc_to (const v8::Arguments &Args);
         static V8Value _gpp_brect (const v8::Arguments &Args);
         static V8Value _gpp_close_subpath (const v8::Arguments &Args);
         static V8Value _gpp_connect_path (const v8::Arguments &Args);
         static V8Value _gpp_contains (const v8::Arguments &Args);
         static V8Value _gpp_count (const v8::Arguments &Args);
         static V8Value _gpp_cubic (const v8::Arguments &Args);
         static V8Value _gpp_current_position (const v8::Arguments &Args);
         static V8Value _gpp_intersected (const v8::Arguments &Args);
         static V8Value _gpp_intersects (const v8::Arguments &Args);
         static V8Value _gpp_empty (const v8::Arguments &Args);
         static V8Value _gpp_length (const v8::Arguments &Args);
         static V8Value _gpp_line_to (const v8::Arguments &Args);
         static V8Value _gpp_move_to (const v8::Arguments &Args);
         static V8Value _gpp_quad_to (const v8::Arguments &Args);
         static V8Value _gpp_simplified (const v8::Arguments &Args);
         static V8Value _gpp_translate (const v8::Arguments &Args);
         static V8Value _gpp_translated (const v8::Arguments &Args);

         static V8Value _gpd_width (const v8::Arguments &Args);
         static V8Value _gpd_height (const v8::Arguments &Args);

         static V8Value _gpixmap_fill (const v8::Arguments &Args);
         static V8Value _gpixmap_load (const v8::Arguments &Args);
         static V8Value _gpixmap_rect (const v8::Arguments &Args);
         static V8Value _gpixmap_save (const v8::Arguments &Args);
         static V8Value _gpixmap_scaled (const v8::Arguments &Args);
         static V8Value _gpixmap_to_image (const v8::Arguments &Args);

         static V8Value _gpainter_end (const v8::Arguments &Args);

         static V8Value _gimage_format (const v8::Arguments &Args);
         static V8Value _gimage_rect (const v8::Arguments &Args);
         static V8Value _gimage_load (const v8::Arguments &Args);
         static V8Value _gimage_save (const v8::Arguments &Args);
         static V8Value _gimage_scaled (const v8::Arguments &Args);

         static V8Value _gwidget_actions (const v8::Arguments &Args);
         static V8Value _gwidget_add_action (const v8::Arguments &Args);
         static V8Value _gwidget_add_actions (const v8::Arguments &Args);
         static V8Value _gwidget_adjust_size (const v8::Arguments &Args);
         static V8Value _gwidget_window_active (const v8::Arguments &Args);
         static V8Value _gwidget_rect (const v8::Arguments &Args);
         static V8Value _gwidget_remove_action (const v8::Arguments &Args);
         static V8Value _gwidget_window_title (const v8::Arguments &Args);

         static V8Value _gweb_modified (const v8::Arguments &Args);
         static V8Value _gweb_load (const v8::Arguments &Args);
//         static V8Value _gweb_page (const v8::Arguments &Args);
         static V8Value _gweb_reload (const v8::Arguments &Args);
         static V8Value _gweb_url (const v8::Arguments &Args);

         // QWebView bindings implemented in dmzJsModuleUiV8QtBasicWebView.cpp
         static V8Value _webview_find_text (const v8::Arguments &Args);
         static V8Value _webview_modified (const v8::Arguments &Args);
         static V8Value _webview_load (const v8::Arguments &Args);
         static V8Value _webview_selected_text (const v8::Arguments &Args);
         static V8Value _webview_set_html (const v8::Arguments &Args);
         static V8Value _webview_text_size_mult (const v8::Arguments &Args);
         static V8Value _webview_zoom_factor (const v8::Arguments &Args);
         static V8Value _webview_url (const v8::Arguments &Args);
         static V8Value _webview_reload (const v8::Arguments &Args);
         static V8Value _webview_page (const v8::Arguments &Args);
         static V8Value _webview_back (const v8::Arguments &Args);
         static V8Value _webview_forward (const v8::Arguments &Args);
         static V8Value _create_webview (const v8::Arguments &Args);

         // QWebPage bindings implemented in dmzJsModuleUiV8QtBasicWebView.cpp
         static V8Value _webpage_mainframe (const v8::Arguments &Args);
         static V8Value _webpage_link_delegation (const v8::Arguments &Args);

         // QWebFrame bindings implemented in dmzJsModuleUiV8QtBasicWebView.cpp
         static V8Value _webframe_load (const v8::Arguments &Args);

         // QEvent bindings implemented in dmzJsModuleUiV8QtBasicEvent.cpp
         static V8Value _event_type (const v8::Arguments &Args);

         // QMouseEvent bindings implemented in dmzJsModuleUiV8QtBasicEvent.cpp
         static V8Value _mouse_event_button (const v8::Arguments &Args);
         static V8Value _mouse_event_buttons (const v8::Arguments &Args);
         static V8Value _mouse_event_global_x (const v8::Arguments &Args);
         static V8Value _mouse_event_global_y (const v8::Arguments &Args);
         static V8Value _mouse_event_global_pos (const v8::Arguments &Args);
         static V8Value _mouse_event_pos (const v8::Arguments &Args);
         static V8Value _mouse_event_posf (const v8::Arguments &Args);
         static V8Value _mouse_event_x (const v8::Arguments &Args);
         static V8Value _mouse_event_y (const v8::Arguments &Args);

         // QGraphicsSceneMouseEvent bindings implemented in dmzJsModuleUiV8QtBasicEvent.cpp
         static V8Value _gscene_mouse_event_button (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_buttons (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_button_down_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_button_down_scene_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_button_down_screen_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_last_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_last_scene_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_last_screen_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_scene_pos (const v8::Arguments &Args);
         static V8Value _gscene_mouse_event_screen_pos (const v8::Arguments &Args);

         // QResizeEvent bindings implemented in dmzJsModuleUiV8QtBasicEvent.cpp
         static V8Value _resize_event_old_size (const v8::Arguments &Args);
         static V8Value _resize_event_size (const v8::Arguments &Args);

         // Phonon::MediaObject bindings implemented in dmzJsModuleUiV8QtBasicPhonon.cpp
         static V8Value _phonon_media_object_source (const v8::Arguments &Args);
         static V8Value _phonon_media_object_has_video (const v8::Arguments &Args);
         static V8Value _phonon_media_object_err_str (const v8::Arguments &Args);
//         static V8Value _phonon_media_object_err_type (const v8::Arguments &Args);
         static V8Value _phonon_media_object_curr_time (const v8::Arguments &Args);
         static V8Value _phonon_media_object_rem_time (const v8::Arguments &Args);
         static V8Value _phonon_media_object_total_time (const v8::Arguments &Args);
         static V8Value _phonon_media_object_tick_interval (const v8::Arguments &Args);
         static V8Value _phonon_media_object_pause (const v8::Arguments &Args);
         static V8Value _phonon_media_object_play (const v8::Arguments &Args);
         static V8Value _phonon_media_object_stop (const v8::Arguments &Args);
         static V8Value _phonon_media_object_state (const v8::Arguments &Args);
         static V8Value _create_phonon_media_object (const v8::Arguments &Args);

         // Phonon::VideoWidget bindings implemented in dmzJsModuleUiV8QtBasicPhonon.cpp
         static V8Value _phonon_video_enter_fullscreen (const v8::Arguments &Args);
         static V8Value _phonon_video_exit_fullscreen (const v8::Arguments &Args);
         static V8Value _create_phonon_video_widget (const v8::Arguments &Args);

         // Phonon bindings implemented in dmzJsModuleUiV8QtBasicPhonon.cpp
         static V8Value _phonon_create_path (const v8::Arguments &Args);
         static V8Value _phonon_clear_paths (const v8::Arguments &Args);

         // Crypto bindings implemented in dmzJsModuleUiV8QtBasicCrypto.cpp
         static V8Value _crypto_add_data (const v8::Arguments &Args);
         static V8Value _crypto_reset (const v8::Arguments &Args);
         static V8Value _crypto_result (const v8::Arguments &Args);
         static V8Value _create_crypto (const v8::Arguments &Args);
         static V8Value _crypto_hash (const v8::Arguments &Args);

         bool eventFilter (QObject *watched, QEvent *event);

         QWidget *_to_qwidget (V8Value value) { return v8_to_qobject<QWidget>(value); }
         QObject *_to_qobject (V8Value value);
         QListWidgetItem *_to_qlistwidgetitem (V8Value value);
         QTreeWidgetItem *_to_qtreewidgetitem (V8Value value);
         QTableWidgetItem *_to_qtablewidgetitem (V8Value value);
         QTableWidgetSelectionRange *_to_qtablewidgetselectionrange (V8Value value);

         QGraphicsItem *_to_graphics_item (V8Value value);
         QBrush *_to_gbrush (V8Value value);
         QPen *_to_gpen (V8Value value);
         QPainterPath *_to_gpainter_path (V8Value value);
         QPainter *_to_gpainter (V8Value value);
         QImage *_to_gimage (V8Value value);
         QPixmap *_to_gpixmap (V8Value value);
         QPaintDevice *_to_gpaint_device (V8Value value);

         QCryptographicHash *_to_crypto_hash (V8Value value);

         QEvent *_to_qevent (V8Value value);

         V8QtDialog *_to_v8_qt_dialog (V8Value value);
         V8QtWidget *_to_v8_qt_widget (V8Value value);
         V8QtObject *_to_v8_qt_object (V8Value value);

         QVariant _qvariant_wrap_v8 (V8Value value);
         V8Value _qvariant_unwrap_v8 (const QVariant &Value);

         void _get_file_dialog_params (
            V8Object params,
            QString &caption,
            QString &dir,
            QString &filter,
            Boolean &allowMultiple);

         void _init_object ();
         void _init_widget ();
         void _init_frame ();
         void _init_button ();
         void _init_list_widget ();
         void _init_list_widget_item ();
         void _init_tree_widget ();
         void _init_tree_widget_item ();
         void _init_table_widget ();
         void _init_table_widget_item ();
         void _init_table_selection ();
         void _init_toolbox ();
         void _init_spinbox ();
         void _init_combobox ();
         void _init_slider ();
         void _init_dial ();
         void _init_lineEdit ();
         void _init_textEdit ();
         void _init_label ();
         void _init_progressBar ();
         void _init_dialog ();
         void _init_message_box ();
         void _init_lcdNumber ();
         void _init_scroll_area ();
         void _init_stacked_widget ();
         void _init_tab_widget ();
         void _init_file_dialog ();
         void _init_main_window ();
         void _init_dock_widget ();
         void _init_group_box ();
         void _init_dt ();
         void _init_action ();
         void _init_input_dialog ();

         void _init_graph ();
         void _init_abs_graph_item ();
         void _init_grect_item ();
         void _init_gpath_item ();
         void _init_gtext_item ();
         void _init_gline_item ();
         void _init_gpixmap_item ();
         void _init_gview ();
         void _init_gscene ();
         void _init_gbrush ();
         void _init_gpen ();
         void _init_gpainter_path ();
         void _init_gpaint_device ();
         void _init_gpainter ();
         void _init_gpixmap ();
         void _init_gimage ();
         void _init_gwidget ();
         void _init_gwebview ();
         void _init_webview ();
         void _init_webframe ();
         void _init_webpage ();

         void _init_event ();
         void _init_mouse_event ();
         void _init_gscene_mouse_event ();
         void _init_resize_event ();

         void _init_phonon ();
         void _init_media_object ();
         void _init_video_player ();

         void _init_crypto ();

         void _init_layout ();
         void _init_box_layout ();
         void _init_hbox_layout ();
         void _init_vbox_layout ();
         void _init_grid_layout ();
         void _init_form_layout ();

         String _find_ui_file (const String &Name);
         void _init (Config &local);

         Log _log;

         StringContainer _searchPaths;
         String _mainWindowModuleName;

         JsModuleUiV8QtBasicState _state;
         V8ValuePersist _self;

         HashTableHandleTemplate<ObsStruct> _obsTable;
         QMap<QObject *, V8QtObject *> _objectMap;

         QMap<QString, QAction *> _menuActionMap;

         QList<QWidget *> _dialogList;
         QPointer<QWidget> _centralWidget;
         QList<String> _dockList;
         QByteArray _mainWindowState;
         QList<V8ValueRef *> _valueDeleteList;

         V8InterfaceHelper _qtApi;
         V8InterfaceHelper _uiLoaderApi;
         V8InterfaceHelper _mainWindowApi;
         V8InterfaceHelper _dockWidgetApi;
         V8InterfaceHelper _messageBoxApi;
         V8InterfaceHelper _layoutApi;
         V8InterfaceHelper _fileDialogApi;
         V8InterfaceHelper _actionApi;
         V8InterfaceHelper _frameApi;
         V8InterfaceHelper _groupBoxApi;
         V8InterfaceHelper _inputDialogApi;

         V8InterfaceHelper _buttonApi;
         V8InterfaceHelper _comboBoxApi;
         V8InterfaceHelper _dialApi;
         V8InterfaceHelper _dialogApi;
         V8InterfaceHelper _dteApi;
         V8InterfaceHelper _lcdApi;
         V8InterfaceHelper _labelApi;
         V8InterfaceHelper _lineEditApi;
         V8InterfaceHelper _listApi;
         V8InterfaceHelper _pbarApi;
         V8InterfaceHelper _scrollAreaApi;
         V8InterfaceHelper _sliderApi;
         V8InterfaceHelper _spinBoxApi;
         V8InterfaceHelper _stackApi;
         V8InterfaceHelper _tabApi;
         V8InterfaceHelper _tableApi;
         V8InterfaceHelper _textEditApi;
         V8InterfaceHelper _toolBoxApi;
         V8InterfaceHelper _treeApi;
         V8InterfaceHelper _widgetApi;

         V8InterfaceHelper _graphApi;
         V8InterfaceHelper _webviewApi;
         V8InterfaceHelper _eventApi;

         V8InterfaceHelper _phononApi;

         V8InterfaceHelper _cryptoApi;


         V8FunctionTemplatePersist _objectTemp;
         V8FunctionPersist _objectCtor;

         V8FunctionTemplatePersist _widgetTemp;
         V8FunctionPersist _widgetCtor;

         V8FunctionTemplatePersist _buttonTemp;
         V8FunctionPersist _buttonCtor;

         V8FunctionTemplatePersist _listWidgetItemTemp;
         V8FunctionPersist _listWidgetItemCtor;

         V8FunctionTemplatePersist _listWidgetTemp;
         V8FunctionPersist _listWidgetCtor;

         V8FunctionTemplatePersist _spinBoxTemp;
         V8FunctionPersist _spinBoxCtor;

         V8FunctionTemplatePersist _comboBoxTemp;
         V8FunctionPersist _comboBoxCtor;

         V8FunctionTemplatePersist _sliderTemp;
         V8FunctionPersist _sliderCtor;

         V8FunctionTemplatePersist _dialTemp;
         V8FunctionPersist _dialCtor;

         V8FunctionTemplatePersist _lineEditTemp;
         V8FunctionPersist _lineEditCtor;

         V8FunctionTemplatePersist _textEditTemp;
         V8FunctionPersist _textEditCtor;

         V8FunctionTemplatePersist _labelTemp;
         V8FunctionPersist _labelCtor;

         V8FunctionTemplatePersist _progressBarTemp;
         V8FunctionPersist _progressBarCtor;

         V8FunctionTemplatePersist _dialogTemp;
         V8FunctionPersist _dialogCtor;

         V8FunctionTemplatePersist _lcdNumberTemp;
         V8FunctionPersist _lcdNumberCtor;

         V8FunctionTemplatePersist _scrollAreaTemp;
         V8FunctionPersist _scrollAreaCtor;

         V8FunctionTemplatePersist _stackedWidgetTemp;
         V8FunctionPersist _stackedCtor;

         V8FunctionTemplatePersist _tabWidgetTemp;
         V8FunctionPersist _tabCtor;

         V8FunctionTemplatePersist _layoutTemp;
         V8FunctionPersist _layoutCtor;

         V8FunctionTemplatePersist _boxLayoutTemp;
         V8FunctionPersist _boxLayoutCtor;

         V8FunctionTemplatePersist _hBoxLayoutTemp;
         V8FunctionPersist _hBoxLayoutCtor;

         V8FunctionTemplatePersist _vBoxLayoutTemp;
         V8FunctionPersist _vBoxLayoutCtor;

         V8FunctionTemplatePersist _gridLayoutTemp;
         V8FunctionPersist _gridLayoutCtor;

         V8FunctionTemplatePersist _formLayoutTemp;
         V8FunctionPersist _formLayoutCtor;

         V8FunctionTemplatePersist _mainWindowTemp;
         V8FunctionPersist _mainWindowCtor;

         V8FunctionTemplatePersist _dockWidgetTemp;
         V8FunctionPersist _dockWidgetCtor;

         V8FunctionTemplatePersist _actionTemp;
         V8FunctionPersist _actionCtor;

         V8FunctionTemplatePersist _frameTemp;
         V8FunctionPersist _frameCtor;

         V8FunctionTemplatePersist _groupBoxTemp;
         V8FunctionPersist _groupBoxCtor;

         V8FunctionTemplatePersist _dteTemp;
         V8FunctionPersist _dteCtor;

         V8FunctionTemplatePersist _treeWidgetTemp;
         V8FunctionPersist _treeWidgetCtor;

         V8FunctionTemplatePersist _treeWidgetItemTemp;
         V8FunctionPersist _treeWidgetItemCtor;

         V8FunctionTemplatePersist _tableWidgetTemp;
         V8FunctionPersist _tableWidgetCtor;

         V8FunctionTemplatePersist _tableWidgetItemTemp;
         V8FunctionPersist _tableWidgetItemCtor;

         V8FunctionTemplatePersist _tableSelectionTemp;
         V8FunctionPersist _tableSelectionCtor;

         V8FunctionTemplatePersist _toolboxTemp;
         V8FunctionPersist _toolboxWidgetCtor;

         V8FunctionTemplatePersist _messageboxTemp;
         V8FunctionPersist _messageboxCtor;

         V8FunctionTemplatePersist _inputDialogTemp;
         V8FunctionPersist _inputDialogCtor;


         V8FunctionTemplatePersist _graphTemp;
         V8FunctionPersist _graphCtor;

         V8FunctionTemplatePersist _gRectTemp;
         V8FunctionPersist _gRectCtor;

         V8FunctionTemplatePersist _gTextTemp;
         V8FunctionPersist _gTextCtor;

         V8FunctionTemplatePersist _gLineTemp;
         V8FunctionPersist _gLineCtor;

         V8FunctionTemplatePersist _gPathTemp;
         V8FunctionPersist _gPathCtor;

         V8FunctionTemplatePersist _gPixmapItemTemp;
         V8FunctionPersist _gPixmapItemCtor;

         V8FunctionTemplatePersist _gSceneTemp;
         V8FunctionPersist _gSceneCtor;

         V8FunctionTemplatePersist _gViewTemp;
         V8FunctionPersist _gViewCtor;

         V8FunctionTemplatePersist _gBrushTemp;
         V8FunctionPersist _gBrushCtor;

         V8FunctionTemplatePersist _gPenTemp;
         V8FunctionPersist _gPenCtor;

         V8FunctionTemplatePersist _gPainterPathTemp;
         V8FunctionPersist _gPainterPathCtor;

         V8FunctionTemplatePersist _gAbsItemTemp;
         V8FunctionPersist _gAbsItemCtor;

         V8FunctionTemplatePersist _gPainterTemp;
         V8FunctionPersist _gPainterCtor;

         V8FunctionTemplatePersist _gPixmapTemp;
         V8FunctionPersist _gPixmapCtor;

         V8FunctionTemplatePersist _gImageTemp;
         V8FunctionPersist _gImageCtor;

         V8FunctionTemplatePersist _gPaintDeviceTemp;
         V8FunctionPersist _gPaintDeviceCtor;

         V8FunctionTemplatePersist _gWidgetTemp;
         V8FunctionPersist _gWidgetCtor;

         V8FunctionTemplatePersist _gWebViewTemp;
         V8FunctionPersist _gWebViewCtor;

         V8FunctionTemplatePersist _webviewTemp;
         V8FunctionPersist _webviewCtor;

         V8FunctionTemplatePersist _webpageTemp;
         V8FunctionPersist _webpageCtor;

         V8FunctionTemplatePersist _webframeTemp;
         V8FunctionPersist _webframeCtor;

         V8FunctionTemplatePersist _eventTemp;
         V8FunctionPersist _eventCtor;

         V8FunctionTemplatePersist _mouseEventTemp;
         V8FunctionPersist _mouseEventCtor;

         V8FunctionTemplatePersist _gsceneMouseEventTemp;
         V8FunctionPersist _gsceneMouseEventCtor;

         V8FunctionTemplatePersist _mediaObjectTemp;
         V8FunctionPersist _mediaObjectCtor;

         V8FunctionTemplatePersist _videoWidgetTemp;
         V8FunctionPersist _videoWidgetCtor;

         V8FunctionTemplatePersist _resizeEventTemp;
         V8FunctionPersist _resizeEventCtor;

         V8FunctionTemplatePersist _cryptoTemp;
         V8FunctionPersist _cryptoCtor;

         V8StringPersist _allowMultipleStr;
         V8StringPersist _allowedAreasStr;
         V8StringPersist _areaStr;
         V8StringPersist _captionStr;
         V8StringPersist _defaultButtonStr;
         V8StringPersist _detailedTextStr;
         V8StringPersist _dirStr;
         V8StringPersist _editableStr;
         V8StringPersist _featuresStr;
         V8StringPersist _filterStr;
         V8StringPersist _floatingStr;
         V8StringPersist _infoTextStr;
         V8StringPersist _optionsStr;
         V8StringPersist _standardButtonsStr;
         V8StringPersist _statusTipStr;
         V8StringPersist _textStr;
         V8StringPersist _toolTipStr;
         V8StringPersist _typeStr;
         V8StringPersist _visibleStr;
         V8StringPersist _currentStr;
         V8StringPersist _decimalStr;
         V8StringPersist _itemsStr;
         V8StringPersist _labelStr;
         V8StringPersist _maxStr;
         V8StringPersist _minStr;
         V8StringPersist _modeStr;
         V8StringPersist _stepStr;
         V8StringPersist _titleStr;
         V8StringPersist _valueStr;
         V8StringPersist _shortcutStr;
         V8StringPersist _iconStr;
         V8StringPersist _leftStr;
         V8StringPersist _topStr;
         V8StringPersist _rightStr;
         V8StringPersist _bottomStr;


      private:
         JsModuleUiV8QtBasic ();
         JsModuleUiV8QtBasic (const JsModuleUiV8QtBasic &);
         JsModuleUiV8QtBasic &operator= (const JsModuleUiV8QtBasic &);
   };
};


inline dmz::JsModuleUiV8QtBasic *
dmz::JsModuleUiV8QtBasic::_to_self (const v8::Arguments &Args) {

   return (dmz::JsModuleUiV8QtBasic *)v8::External::Unwrap (Args.Data ());
}


#endif // DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
