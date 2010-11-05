var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/ui/uiLoader')
  , form
  , spinbox
  , dspinbox
  , val
  , dval
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/SpinBoxTestForm.ui");
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

puts("Done.");
