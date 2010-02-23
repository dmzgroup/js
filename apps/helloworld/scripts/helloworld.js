var puts = require('sys').puts;
var vector = require('dmz/kernel/vector');

var v1 = vector.create().setXYZ(1, 2, 3);

var f1 = function () {

   //foo.value = 1;
};

var f2 = function () { f1 (); };
var f3 = function () { f2 (); };
var f4 = function () { f3 (); };
var f5 = function () { f4 (); };

f5();

puts ("Hello World: " + info.name);
puts ("v1 = " + v1.toString ());

