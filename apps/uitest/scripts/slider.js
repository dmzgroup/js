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
  , form = dmz.ui.loader.load("./scripts/SliderForm.ui")
  , slider
  ;

puts("Script: " + self.name);

form.show();

slider = form.lookup("slider");
slider.observe(self, "sliderPressed", function () {
   puts("Slider pressed.");
   puts("Slider isDown:", slider.isDown());
});

slider.observe(self, "sliderReleased", function () {
   puts("Slider released.");
   puts("Slider isDown:", slider.isDown());
});

slider.observe(self, "valueChanged", function (val) {
   puts("Value changed:", val, slider.value());
});

puts("slider.max:", slider.maximum());
slider.maximum(slider.maximum() * 2);
puts("slider.max2:", slider.maximum());
puts("slider.min:", slider.minimum());
slider.minimum(slider.minimum() + 1);
puts("slider.min2:", slider.minimum());

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
