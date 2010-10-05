var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/components/ui/uiLoader')
  , form
  , pbar
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/ProgressBarForm.ui");
form.show();

pbar = form.lookup("progressBar");

pbar.observe(self, "valueChanged", function (val) {

   puts("pbar value changed:", val);
});


pbar.value(76);
puts("text:", pbar.text());
puts("maximum:", pbar.maximum());
puts("minimum:", pbar.minimum());
pbar.reset();
pbar.maximum(200);
pbar.minimum(50);
pbar.value(150);
puts("text:", pbar.text());
puts("maximum:", pbar.maximum());
puts("minimum:", pbar.minimum());

puts("Done.");
