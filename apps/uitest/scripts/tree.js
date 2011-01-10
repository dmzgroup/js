var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , file: require("dmz/ui/fileDialog")
          , loader: require('dmz/ui/uiLoader')
          , mainWindow: require('dmz/ui/mainWindow')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
  , _main
  , _exports = {}
  , puts = require('sys').puts
  , form = dmz.ui.loader.load("./scripts/TreeForm.ui")
  , tree = form.lookup ("treeWidget")
  , item
  , item2
  , item3
  , widget = dmz.ui.loader.load ("lcd.ui")
  , item_to_str
  ;

item_to_str = function (item) {

   var str = "item: ";
   for (var ix = 0; ix < item.columnCount(); ix += 1) {
      str += item.text(ix) + " ";
   }
   puts(str);
};

puts('Script: ' + self.name);

form.show();

widget.lookup("lcd").value (100);

form.observe(self, "pushButton", "clicked", function () { tree.clear(); });

tree.columnCount (4);
tree.headerLabels (["0", "1", "2", "3"]);
item = tree.add(["one", "two", "three"]);
tree.add(["three", "four", "five", "six"]);
tree.add(["seven"]);
item3 = tree.add(item);

puts ("itemCount:", tree.itemCount());

item.add(["three, four", "Five", "SEVEN"]);
item2 = item.add(["eight", "twenty", "hello", "two"]);
item2.add(["five", "seven"]);

puts ("item2.text: <twenty>", item2.text (1));

tree.currentItem(item);
puts ("hidden:", tree.columnHidden (1, true));
puts ("width:", tree.columnWidth (2, 10));
puts ("wordWrap:", tree.wordWrap (true));
puts ("currentColumn:", tree.currentColumn());
puts ("currentItem:", tree.currentItem().text(1));

tree.findItems ("four", dmz.ui.consts.MatchContains).forEach(function (item) {

   var str = "item: ";
   for (var ix = 0; ix < item.columnCount(); ix += 1) {
      str += item.text(ix) + " ";
   }
   puts(str);
});

puts (tree.itemAbove(tree.itemBelow(item)).text(0), item.text(0));
puts (tree.itemBelow(item2).text(0));

tree.itemWidget(item3, 0, widget);
tree.itemWidget(item3, 0).lookup("lcd").value (tree.itemWidget(item3, 0).lookup("lcd").value() * 2);

tree.selectedItems().forEach(item_to_str);

tree.sortColumn(0, dmz.ui.consts.Ascending);

item_to_str(item.child(0));
puts(item.childCount());
puts(item.columnCount());
item_to_str(item);
item_to_str(item2.parent());

item.sortChildren(dmz.ui.consts.Descending);

widget.observe(self, "b0", "clicked", function () { item.takeChild(0); });
widget.observe(self, "b1", "clicked", function () { item.takeChildren(); });
widget.observe(self, "b2", "clicked", function () { item.treeWidget().clear(); });

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});
