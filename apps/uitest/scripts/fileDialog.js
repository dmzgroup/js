var puts = require('sys').puts
  , uiLoader = require('dmz/ui/uiLoader')
  , file = require("dmz/ui/fileDialog")
  , form
  , text
  ;

puts("Script:", self.name);

form = uiLoader.load("./TextEditForm.ui");
form.show();

text = form.lookup("textEdit");

form.observe(self, "saveButton", "clicked", function () {

   var str = file.getSaveFileName(
      form,
      { caption: "Save File Dialog"
      , dir: "/Users/Ben/cm"
      , filter: "Scripts (*.js)"
      });
   puts("saveFileName:", str);
});
