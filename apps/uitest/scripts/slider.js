var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/components/ui/uiLoader')
  , form
  , slider
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/SliderForm.ui");
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

puts("Done.");
