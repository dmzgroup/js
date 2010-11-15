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
   , form = dmz.ui.loader.load("./scripts/LineEditForm.ui")
  , line
  ;

puts("Script: " + self.name);

form.show();

line = form.lookup("lineEdit");
line.observe(self, "textChanged", function (text) {
   puts("text changed:", text);
});

line.clear();
line.text("This is the text.");
line.undo();
line.redo();
line.text("This isn't the text.");

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
