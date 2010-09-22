var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , line
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/LineEditForm.ui");
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
