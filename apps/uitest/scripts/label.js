var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/components/ui/uiLoader')
  , form
  , label
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/LabelForm.ui");
form.show();

label = form.lookup("label");

label.text("This is the text.");
label.text("This isn't the text. This isn't the text. This isn't the text.");

timer.setTimer(self, 5, function () {

   puts("word wrap:", label.wordWrap());
   label.wordWrap(true);
   puts("word wrap2:", label.wordWrap());
   timer.setTimer(self, 5, function () { label.clear(); });
});

puts("Done.");
