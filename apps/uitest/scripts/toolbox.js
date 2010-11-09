var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/ui/uiLoader')
  , form
  , pbar = uiLoader.load("./scripts/ProgressBarForm.ui")
  , widget = uiLoader.load("./scripts/SliderForm.ui")
  , cb = uiLoader.load("./scripts/CheckBoxForm.ui")
  , label = uiLoader.load("./scripts/LabelForm.ui")
  , combo = uiLoader.load("./scripts/comboBoxForm.ui")
  , tb
  , at
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/ToolBoxForm.ui");
form.show();

tb = form.lookup("toolBox");

form.observe(self, "add", "clicked", function () {

   tb.addItem(label, "label");
   tb.addItem(cb, "cb");
   tb.addItem(pbar, "pbar");
   tb.addItem(combo, "combo");
   tb.insertItem(widget, "slider", 1);
});

puts("item text:",tb.itemText(1));

tb.observe(self, "currentChanged", function (val) {

   puts("tb.observe");
   puts("changed to:", val);
   puts("tT:", tb.itemText(val), "cI:", tb.currentIndex());
   puts("at:", tb.currentWidget());
   puts(tb.itemText(tb.indexOf(tb.currentWidget())));
});

puts("tab count:", tb.count());
tb.removeItem(2);
puts("tab count2:", tb.count());
at = tb.widget(1);
puts("cw:", tb.currentWidget());
tb.currentWidget(cb);
puts("at:", at);
tb.currentWidget(at);
puts("finished cW");
tb.currentIndex(tb.indexOf(tb.widget(2)));

puts("Done.");
