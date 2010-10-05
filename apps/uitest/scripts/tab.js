var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/components/ui/uiLoader')
  , form
  , pbar = uiLoader.load("./scripts/ProgressBarForm.ui")
  , widget = uiLoader.load("./scripts/SliderForm.ui")
  , cb = uiLoader.load("./scripts/CheckBoxForm.ui")
  , label = uiLoader.load("./scripts/LabelForm.ui")
  , combo = uiLoader.load("./scripts/comboBoxForm.ui")
  , tw
  , at
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/TabForm.ui");
form.show();

tw = form.lookup("tabWidget");

tw.add(label, "label");
tw.add(cb, "cb");
tw.add(pbar, "pbar");
tw.add(combo, "combo");
tw.add(widget, "slider", 1);

puts("tab text:",tw.tabText(1));

tw.observe(self, "currentChanged", function (val) {

   puts("tw.observe");
   puts("changed to:", val);
   puts("tT:", tw.tabText(val), "cI:", tw.currentIndex());
   puts("at:", tw.currentWidget());
   puts(tw.tabText(tw.indexOf(tw.currentWidget())));
});

puts("tab count:", tw.count());
tw.remove(2);
puts("tab count2:", tw.count());
at = tw.at(1);
puts("cw:", tw.currentWidget());
tw.currentWidget(cb);
puts("at:", at);
tw.currentWidget(at);
puts("finished cW");
tw.currentIndex(tw.indexOf(tw.at(2)));

puts("Done.");
