var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/components/ui/uiLoader')
  , messageBox = require('dmz/components/ui/messageBox')
  , fileDialog = require('dmz/components/ui/fileDialog')
  , form
  , button1
  , button2
  , list1
  , item
  , lastFile = "/"
  ;

puts("Script: " + self.name);

form = uiLoader.load("TestForm");
form.show();

button2 = form.lookup("button2");
if(button2) {
   button2.observe(self, "clicked", function (btn) {
      btn.text("Don't Push Me!!!");
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

puts("messageBox.create");
      var mb = messageBox.create(
         form,
         {
            type: messageBox.Information,
            text: "Some Text",
            informativeText: "This is some informative Text",
            standardButtons: [messageBox.Ok, messageBox.Cancel, messageBox.Save],
            defaultButton: messageBox.Cancel
         }
      );
      
      if (mb) {
         
         mb.open(self, function () {
            puts("mb: dialog closed");
            button1.hide();
            var file = fileDialog.getExistingDirectory(
               form,
               {
                  caption: "this is my dialog",
                  dir: lastFile,
                  filter_: "Source Code (*.lmk *.h *.cpp)",
                  allowMultiple: true
               }
            );
            if (file) {
               puts(file);
               lastFile = file[0];
            }
         });
      }
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
