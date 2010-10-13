var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/ui/uiLoader')
  , form
  , cb
  , count
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/comboBoxForm.ui");
form.show();

cb = form.lookup("comboBox");
cb.observe(self, "currentIndexChanged", function (value) {
   puts("Count:", cb.count());
   puts("currentIndex:", value, cb.currentIndex());
   puts("currentText:", cb.currentText());
});

cb.addItem("addItem1");
cb.addItems(["addItems2", "addItems3", "addItems4", "addItems5"]);
puts("findText (2):", cb.findText("addItems3"));
cb.removeIndex(1);

for (count = 0; count < cb.count(); count += 1) {
   puts(count, cb.itemText(count));
}

//cb.clear();
puts("count:", cb.count());

for (count = 0; count < cb.count(); count += 1) {
   puts(count, cb.itemText(count));
}

cb.currentIndex (3);
puts("Done.");
