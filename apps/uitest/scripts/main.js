var puts = require('sys').puts
  , Qt = require('dmz/ui')
  , QUiLoader = require('dmz/ui/uiLoader')
  , QMessageBox = require('dmz/ui/messageBox')
  , QDockWidget = require('dmz/ui/dockWidget')
  , file = require("dmz/ui/fileDialog")
  , QAction = require('dmz/ui/action')
  , mainWindow = require('dmz/ui/mainWindow')
  , mainForm = QUiLoader.load('./scripts/main.ui')
  , listWidget = mainForm.lookup('listWidget')
  , stackedWidget = mainForm.lookup('stackedWidget')
  , index = 0
  ;

puts('Script: ' + self.name);


function add_script (script) {
   var form = QUiLoader.load('./scripts/' + script + '.ui');
   form.name(script);
   listWidget.addItem(script);
   stackedWidget.add(form);
}

listWidget.observe(self, 'currentRowChanged', function (row) {
   index = row;
   // puts('row: ' + index);
   stackedWidget.currentIndex(index);

   var label = mainForm.lookup('infoLabel');
   if (label) { label.text(index);}
});

mainForm.observe(self, 'doneButton', 'clicked', function (button) {
   var mb = QMessageBox.create(
      button.parent(),
      {
         type: QMessageBox.Information,
         text: "Are you done?",
         informativeText: "Hit Ok to quit!",
         standardButtons: [QMessageBox.Ok, QMessageBox.Cancel],
         defaultButton: QMessageBox.Ok
      }
   );

   mb.open(self, function (val) {
      if (val === QMessageBox.Ok) {

         puts("mb: OK pressed");
         mainWindow.close();
      }
      else if (val === QMessageBox.Cancel) {

         puts("mb: Cancel pressed");
      }
   });
});

// mainForm.observe(self, 'prevButton', 'clicked', function (btn) {
// });

// mainForm.observe(self, 'nextButton', 'clicked', function (button) {
// });

add_script('Slider');
add_script('Widgets');


mainForm.observe(self, 'comboBox', 'currentIndexChanged', function (value, comboBox) {

   // puts('comboBox: ' + value);
   var comboBoxLabel = mainForm.lookup ('comboBoxLabel');
   if (comboBoxLabel) { comboBoxLabel.text (comboBox.itemText(value)); }
});

mainForm.observe(self, 'lineEdit', 'textChanged', function (value) {

   // puts('lineEdit: ' + value);
   var lineEditLabel = mainForm.lookup ('lineEditLabel');
   if (lineEditLabel) { lineEditLabel.text (value); }
});

mainForm.observe(self, 'spinBox', 'valueChanged', function (value) {

   // puts('spinBox: ' + value);
   var spinBoxLabel = mainForm.lookup ('spinBoxLabel');
   if (spinBoxLabel) { spinBoxLabel.text (value); }
});

mainForm.observe(self, 'doubleSpinBox', 'valueChanged', function (value) {

   // puts('doubleSpinBox: ' + value);
   var doubleSpinBoxLabel = mainForm.lookup ('doubleSpinBoxLabel');
   if (doubleSpinBoxLabel) { doubleSpinBoxLabel.text (value); }
});

function sliderUpdate (val) {

   // puts('slider update: ' + val);

   var lcd = mainForm.lookup ('lcd');
   if (lcd) { lcd.property ('value', val); }

   var progressBar = mainForm.lookup ('progressBar');
   if (progressBar) {
      // progressBar.value (val);
      progressBar.property ('value', val);
   }

   var dial = mainForm.lookup ('dial');
   if (dial) { dial.value (val); }

   var slider = mainForm.lookup ('slider');
   if (slider) { slider.value (val); }
}

mainForm.observe(self, 'slider', 'valueChanged', sliderUpdate);
mainForm.observe(self, 'dial', 'valueChanged', sliderUpdate);

stackedWidget.currentIndex(0);

mainWindow.centralWidget (mainForm);

var dockName = "My Tools";
var toolsWidget = QUiLoader.load('./scripts/Tools.ui');
if(toolsWidget) {
   
   // toolsWidget.observe(self, 'button1', 'clicked', function (btn) {
   // });

   // toolsWidget.observe(self, 'button2', 'clicked', function (btn) {
   // });

   // toolsWidget.observe(self, 'button3', 'clicked', function (btn) {
   // });
   
   // var toolsDock = QDockWidget.create (dockName, toolsWidget);
   var toolsDock = mainWindow.createDock (dockName, toolsWidget);
   if (toolsDock) {
      
      toolsDock.title ("Tools I Like");
      mainWindow.addDock (dockName, Qt.LeftDockWidgetArea);
      toolsWidget.observe(self, "button1", "clicked", function () {

         var str = file.getSaveFileName(
            mainWindow.mainWidget(),
            { caption: "Save File Dialog"
            , dir: "/Users/Ben/cm"
            , filter: "Scripts (*.js)"
            });

         puts("saveFileName:", str, "mainWidget:" , mainWindow.mainWidget());
      });
   }
}

mainWindow.addMenu (self, "&File", "Hello", "Ctrl+n", function () {
   puts("Friends");
});

mainWindow.addSeparator("&File");

mainWindow.addMenu (self, "&File", "Show It", "", function (obj) {
   toolsDock.show();
});

mainWindow.addSeparator("&File");

mainWindow.addMenu (self, "&File", "What", "Ctrl+h", function (obj) {
   toolsDock.hide();
}).text("Hide It");

mainWindow.addSeparator("&File");

var action = mainWindow.addMenu (self, "&Name", "My Name Is1", "Meta+y", function (obj) {
   obj.enabled(false);
   puts("action:", action.callback(self, "triggered"));
});

mainWindow.addSeparator("&Name");

action.text ({statusTip:"What ever you do, do not select this menu"});

mainWindow.addMenu (self, "&Name", "My Name Is2", function (obj) {
   action.enabled(true);
});

action.trigger();

puts('Done with: ' + self.name);
