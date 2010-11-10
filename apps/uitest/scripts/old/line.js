var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/ui/uiLoader')
  , form
  , line
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/LineEditForm.ui");
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

puts("Done.");
