var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/components/ui/uiLoader')
  , form
  , text
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/TextEditForm.ui");
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
