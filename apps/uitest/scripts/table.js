var puts = require('sys').puts
  , Qt = require('dmz/ui/consts')
  , QUiLoader = require('dmz/ui/uiLoader')
  , file = require("dmz/ui/fileDialog")
  , QAction = require('dmz/ui/action')
  , mainWindow = require('dmz/ui/mainWindow')
  , form = QUiLoader.load("./scripts/TableForm.ui")
  , table = form.lookup ("tableWidget")
  , item
  , item2
  , item3
  , widget = QUiLoader.load ("lcd.ui")
  , item_to_str
  , ranges
  ;

item_to_str = function (item) {

   var str = "item: ";
   for (var ix = 0; ix < item.columnCount(); ix += 1) {
      str += item.text(ix) + " ";
   }
   puts(str);
};

for (var ix = 0; ix < 6; ix += 1) {

   table.insertColumn();
   table.insertRow();
}

table.setSpan (1, 2, 1, 2);

//table.columnCount (6);
//table.rowCount (6);


puts('Script: ' + self.name);

form.show();

widget.lookup("lcd").value (100);

form.observe(self, "pcol", "clicked", function () { table.insertColumn(); });
form.observe(self, "prow", "clicked", function () { table.insertRow(); });
form.observe(self, "mcol", "clicked", function () { table.removeColumn(); });
form.observe(self, "mrow", "clicked", function () { table.removeRow(); });
form.observe(self, "clear", "clicked", function () { table.clear(); });
form.observe(self, "clearContents", "clicked", function () { table.clearContents(); });
form.observe(self, "clearSpans", "clicked", function () { table.clearSpans(); });
form.observe(self, "resizeAll", "clicked", function () { table.resizeColumnsToContents(); });
form.observe(self, "resizeCurrentColumn", "clicked", function () { table.resizeColumnToContents(table.currentColumn()); });
form.observe(self, "resizeCurrentRow", "clicked", function () { table.resizeRowToContents(table.currentRow()); });
form.observe(self, "ranges", "clicked", function () {

   table.sortItems(0, 1);
});

table.setHorizontalLabels(["Hello", "Goodbye", "What", "Yes"]);
table.setVerticalLabels(["Why", "Not", "Do", "Something", "Rather", "Crazy", "With", "Labels"]);

table.horizontalHeaderItem(0).text("Is this");
table.verticalHeaderItem(4).text("Mildly");

table.observe(self, "cellClicked", function (row, col) {

   puts (row, col, "clicked");
});

table.observe(self, "cellDoubleClicked", function (row, col) {

   puts (row, col, "double-clicked");
});

table.observe(self, "itemSelectionChanged", function () {

   puts ("item selection changed");
});

table.observe(self, "currentItemChanged", function (curr, prev) {

   puts (curr, prev);
});

//table.selectRow(2);
//table.selectColumn (3);

//table.rowHidden (1, true);
//table.columnHidden(1, true);

table.cellWidget(0, 0, widget);

table.setCurrentCell (0, 1);
