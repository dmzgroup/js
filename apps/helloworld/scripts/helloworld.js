var puts = require('sys').puts;
var vector = require('dmz/types/vector');
var mask = require('dmz/types/mask');

var v1 = vector.create().set(1, 2, 3);

var f1 = function () {

   //foo.value = 1;
};

var f2 = function () { f1(); };
var f3 = function () { f2(); };
var f4 = function () { f3(); };
var f5 = function () { f4(); };

f5();

var m1 = mask.create().set([1,2,4,8]);
puts(m1.bits.length);

puts("Hello World: " + info.name);
puts("v1 = " + v1.toString());
puts("m1 = " + m1.toString());

var date = new Date;
puts(date.toString());
