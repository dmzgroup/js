var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , file: require("dmz/ui/fileDialog")
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
  , _main
  , _exports = {}
  , puts = require('sys').puts
  , form = dmz.ui.loader.load("./TextEditForm.ui")
  , mainWindow = require("dmz/ui/mainWindow")
  , text
  ;

puts("Script:", self.name);

form.show();

text = form.lookup("textEdit");

form.observe(self, "saveButton", "clicked", function () {

   var str = dmz.ui.file.getSaveFileName(
      { caption: "Save File Dialog"
      , dir: "/Users/Ben/cm"
      , filter: "Scripts (*.js)"
      }
      , mainWindow.window());
   puts("saveFileName:", str);
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});
