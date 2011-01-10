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
   , form = dmz.ui.loader.load("./scripts/TextEditForm.ui")
  , text
  ;

puts("Script: " + self.name);

form.show();

text = form.lookup("textEdit");
text.observe(self, "textChanged", function () {
   puts("text changed:");
});

text.clear();
text.text("This is the text.");
text.undo();
text.redo();
text.text("This isn't the text.");
text.clear();

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
