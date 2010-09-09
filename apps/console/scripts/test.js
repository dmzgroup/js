var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , button1
  , button2
  , list1
  , item
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TestForm.ui");
self.form = form;
form.show();

button2 = form.lookup("button2");
button2.observe(self, "clicked", function () {
   button2.text("Don't Push Me!!!");
   timer.setTimer(self, 2, function () {
      button2.text("Push Me");
      button1.enabled(true);
      button1.show();
   });
});

button1 = form.lookup("button1");
button1.observe (self, "clicked", function () {
   button1.text(self.name);
   button1.enabled(false);
   button1.hide();
});

list1 = form.lookup("list1");
list1.addItem("this is item 1");
list1.addItem("another item");
list1.addItem("last item");
self.list1 = list1;
// timer.setTimer(self, 2, function () {
//    item.text("Look at me!!!!");
// });

list1.observe(self, "itemActivated", function (item) {
   puts('----- js.itemActivated ------');
   puts(item.text("hello world"));
});

// list1.observe(self, "currentItemChanged", function(curr, prev) {
//    puts('----- js.currentItemChanged ------');
// //   puts(curr.text);
// });

// timer.setRepeatingTimer(self, 10, function () {
   // var btn1 = button;
   // var form1 = form;
// });

puts("Done.");
