var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
   , _main
   , _exports = {}
   , puts = require('sys').puts
   , form = dmz.ui.loader.load("./scripts/TabForm.ui")
  , pbar = dmz.ui.loader.load("./scripts/ProgressBarForm.ui")
  , widget = dmz.ui.loader.load("./scripts/SliderForm.ui")
  , cb = dmz.ui.loader.load("./scripts/CheckBoxForm.ui")
  , label = dmz.ui.loader.load("./scripts/LabelForm.ui")
  , combo = dmz.ui.loader.load("./scripts/comboBoxForm.ui")
  , tw
  , at
  ;

puts("Script: " + self.name);

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

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
