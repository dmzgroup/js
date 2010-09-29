var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , messageBox = require('dmz/components/ui/messageBox')
  , form
  , button1
  , button2
  , list1
  , item
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TestForm.ui");
form.show();

button2 = form.lookup("button2");
if(button2) {
   button2.observe(self, "clicked", function () {
      button2.text("Don't Push Me!!!");
      timer.setTimer(self, 2, function () {
         button2.text("Push Me");
         if(button1) {
            button1.enabled(true);
            button1.show();
         }
      });
   });
}

button1 = form.lookup("button1");
if(button1) {
   // button1.enabled(false);
   button1.observe (self, "clicked", function () {
      button1.text(self.name);
      // button1.enabled(false);
//      messageBox.critical();
      var mb = messageBox.create(
         form,
         {
            type: messageBox.Information,
            text: "Some Text",
            informativeText: "This is some informative Text"
            // standardButtons: [messageBox.Ok]
         }
      );
      mb.open(self, function () {
         puts("mb: dialog closed");
      });
//      messageBox.question();
//      messageBox.warning();
      button1.hide();
   });
}

list1 = form.lookup("list1");
if(list1) {

   list1.addItem("this is item 1");
   list1.addItem("another item");
   list1.addItem("last item");

   list1.observe(self, "itemActivated", function (item) {
      puts(item.text());
      item.text("hello world");
   });
}

var list2 = form.lookup("list1");
if(list2 == list1) {
   list2.addItem('adding to list2');

   // list2.observe(self, "currentItemChanged", function(curr, prev) {
   //    puts(curr.text());
   // });
}

puts("Done.");
