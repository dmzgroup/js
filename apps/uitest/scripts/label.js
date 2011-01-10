var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , layout: require("dmz/ui/layout")
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
   , _main
   , _exports = {}
   , puts = require('sys').puts
   , form = dmz.ui.loader.load("./scripts/LabelForm.ui")
  , label
  ;

puts("Script: " + self.name);

form.show();

label = form.lookup("label");
form.layout(dmz.ui.layout.createHBoxLayout());
label.text("This is the text.");
label.text("This isn't the text. This isn't the text. This isn't the text.");

dmz.timer.setTimer(self, 5, function () {

   puts("word wrap:", label.wordWrap());
   label.wordWrap(true);
   puts("word wrap2:", label.wordWrap());
   dmz.timer.setTimer(self, 5, function () { label.clear(); });
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
