var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , pbar = ui.load("./scripts/ProgressBarForm.ui")
  , widget = ui.load("./scripts/SliderForm.ui")
  , cb = ui.load("./scripts/CheckBoxForm.ui")
  , label = ui.load("./scripts/LabelForm.ui")
  , combo = ui.load("./scripts/comboBoxForm.ui")
  , sw
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/StackedWidgetForm.ui");
form.show();

sw = form.lookup("stackedWidget");

label.show();
cb.show();
pbar.show();
combo.show();
widget.show();

sw.add(label);
sw.add(cb);
sw.add(pbar);
sw.add(combo);
sw.add(widget, 1);

self.log.warn ("count:", sw.count());

timer.setTimer(self, 5, function () {

   sw.currentIndex(0);
   puts("stacked: 0 ", sw.currentIndex ());
});

timer.setTimer(self, 10, function () {

   sw.currentIndex(1);
   puts("stacked: 1 ", sw.currentIndex ());
});

timer.setTimer(self, 15, function () {

   sw.currentIndex(2);
   puts("stacked: 2 ", sw.currentIndex ());
});

timer.setTimer(self, 20, function () {

   sw.currentIndex(3);
   puts("stacked: 3 ", sw.currentIndex ());
});

timer.setTimer(self, 25, function () {

   sw.currentIndex(4);
   puts("stacked: 4 ", sw.currentIndex ());
});

timer.setTimer(self, 30, function () {

   sw.currentIndex(5);
   puts("stacked: 5 ", sw.currentIndex ());
});

timer.setTimer(self, 35, function () {

   sw.currentIndex(6);
   puts("stacked: 6 ", sw.currentIndex ());
});

timer.setTimer(self, 40, function () {

   puts("3: ", sw.indexOf(sw.at(3)));
   sw.remove(widget);
   puts(sw.at(3));
   puts(sw.count());
   timer.setTimer(self, 5, function () {

      sw.currentIndex(0);
      puts("stacked: 0 ", sw.currentIndex ());
   });

   timer.setTimer(self, 10, function () {

      sw.currentIndex(1);
      puts("stacked: 1 ", sw.currentIndex ());
   });

   timer.setTimer(self, 15, function () {

      sw.currentIndex(2);
      puts("stacked: 2 ", sw.currentIndex ());
   });

   timer.setTimer(self, 20, function () {

      sw.currentIndex(3);
      puts("stacked: 3 ", sw.currentIndex ());
   });

   timer.setTimer(self, 25, function () {

      sw.currentIndex(4);
      puts("stacked: 4 ", sw.currentIndex ());
   });

   timer.setTimer(self, 30, function () {

      sw.currentIndex(5);
      puts("stacked: 5 ", sw.currentIndex ());
   });
});

//timer.setTimer(self, 5 * (sw.count () + 1), function () {
//   sw.remove(widget);
//   sw.remove(sw.at(2));

//   self.log.warn ("count:", sw.count());
//   for (var ix = 0; ix < sw.count (); ix += 1) {
//      timer.setTimer(self, 5 * (ix + 1), function () {
//         sw.currentIndex(ix);
//         puts("stacked2:", ix, sw.currentIndex ());
//      });
//   }

//   sw.add(widget);
//   puts(sw.indexOf(widget));
//   sw.currentWidget(pbar);

//   puts("Done.");

//});
