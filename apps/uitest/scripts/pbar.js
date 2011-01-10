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
  , form = dmz.ui.loader.load("./scripts/ProgressBarForm.ui")
  , pbar
  ;

puts("Script: " + self.name);

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

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});
puts("Done.");
