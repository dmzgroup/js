var puts = require('sys').puts
  , Qt = require('dmz/ui')
  , QUiLoader = require('dmz/ui/uiLoader')
  , QMessageBox = require('dmz/ui/messageBox')
  , QDockWidget = require('dmz/ui/dockWidget')
  , mainWindow = require('dmz/ui/mainWindow')
  , mainForm = QUiLoader.load('./scripts/main.ui')
  , listWidget = mainForm.lookup('listWidget')
  , stackedWidget = mainForm.lookup('stackedWidget')
  , index = 0
  ;

puts('Script: ' + self.name);

function dump (obj) {
   puts('-d-u-m-p----------------------');
   puts('objectName: ' + obj.objectName ());
   puts('className: ' + obj.className ());
   puts('------------------------------');
}

function add_script (script) {
   var form = QUiLoader.load('./scripts/' + script + '.ui');
   form.objectName(script);
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
         text: "Some Text",
         informativeText: "This is some informative Text",
         standardButtons: [QMessageBox.Ok, QMessageBox.Cancel],
         defaultButton: QMessageBox.Cancel
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

mainForm.observe(self, 'nextButton', 'clicked', function (button) {
   var mb = QMessageBox.create(
      button.parent(),
      {
         type: QMessageBox.Information,
         text: "Some Text",
         informativeText: "This is some informative Text",
         standardButtons: [QMessageBox.Ok, QMessageBox.Cancel, QMessageBox.Save],
         defaultButton: QMessageBox.Cancel
      }
   );

   mb.open(self, function (val) {
      if (val === QMessageBox.Ok) {

         puts("mb: OK pressed");
      }
      else if (val === QMessageBox.Save) {

         puts("mb: Save pressed");
      }
      else if (val === QMessageBox.Cancel) {

         puts("mb: Cancel pressed");
      }
   });
});

dump(mainForm);
dump(listWidget);
dump(stackedWidget);

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
   
   toolsWidget.observe(self, 'button1', 'clicked', function (btn) {
      dump (btn);
   });

   toolsWidget.observe(self, 'button2', 'clicked', function (btn) {
   });

   toolsWidget.observe(self, 'button3', 'clicked', function (btn) {
   });
   
   // var toolsDock = QDockWidget.create (dockName, toolsWidget);
   var toolsDock = mainWindow.createDock (dockName, toolsWidget);
   if (toolsDock) {
      
      dump (toolsDock);
      mainWindow.addDock (dockName, Qt.LeftDockWidgetArea);
   }
}
   
   // var dock = dockWidget.create ("Tools", QUiLoader);
   // dock.floating (true);
   // dock.featrues ()
   // dock.allowedAreas(Qtui.LeftDockWidgetArea, ui.RightDockWidgetArea]);
   // mainWindow.addDock (dock, )
   // 
   // dock.setObjectName ("my_dock");
   // dock.addWidget(widget);

// mainWindow.dockWidget (
//    "Tools",
//    QUiLoader.load("./scripts/Tools.ui"),
//    {
//       title: "My Dock Window",
//       allowedArea: [],
//       visiable: true,
//       floating: true
//    }
// );

puts('Done with: ' + self.name);
