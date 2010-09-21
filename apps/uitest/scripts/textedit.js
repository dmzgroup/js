var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , text
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TextEditForm.ui");
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

puts("Done.");
