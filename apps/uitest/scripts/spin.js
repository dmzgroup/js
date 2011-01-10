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
   , form = dmz.ui.loader.load("./scripts/SpinBoxTestForm.ui")
  , spinbox
  , dspinbox
  , val
  , dval
  ;

puts("Script: " + self.name);

form.show();

spinbox = form.lookup("spinBox");
puts(spinbox);
spinbox.observe(self, "valueChanged", function (value) {
   puts("Value: " + value, spinbox.value());
   puts("Max:", spinbox.maximum());
   puts("Min:", spinbox.minimum());
   spinbox.maximum(spinbox.maximum() * 2);
//   spinbox.minimum(spinbox.maximum());
   puts("Max2:", spinbox.maximum());
   puts("Min2:", spinbox.minimum());
   puts("Text:", spinbox.text());
});

puts(dspinbox);
dspinbox = form.lookup("doubleSpinBox");
dspinbox.observe(self, "valueChanged", function (value) {
   puts("Value: " + value, dspinbox.value());
   puts("Max:", dspinbox.maximum());
   puts("Min:", dspinbox.minimum());
   dspinbox.maximum(dspinbox.maximum() * 2.5);
//   dspinbox.minimum(dspinbox.maximum() * 0.5);
   puts("Max2:", dspinbox.maximum());
   puts("Min2:", dspinbox.minimum());
   puts("Text:", dspinbox.text());
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
