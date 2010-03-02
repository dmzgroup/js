var puts = require('sys').puts;
var vector = require('dmz/types/vector');
var mask = require('dmz/types/mask');
var createError = require('dmz/types/util').createError;
var defs = require('dmz/runtime/definitions');

var v1 = vector.create(1, 2, 3);
var v2 = vector.create([4, 5, 6]);
var v3 = vector.create(v1);

self.log.info(v1, v2, v3, v2.copy());

var f1 = function () {

   var foo = 1;
};

var f2 = function () { f1(); };
var f3 = function () { f2(); };
var f4 = function () { f3(); };
var f5 = function () { f4(); };

f5();

var m1 = mask.create([1,2,4,8]);

puts("Hello World: " + self.name);
puts("v1 = " + v1);
puts("m1 = " + m1);

self.log.error("This is an error log");
self.log.warn("This is a warn log");
self.log.info("This is an info log");
self.log.debug("This is a debug log");
self.log.out("This is an out log");
self.log.out("This is a", "multipart log");
self.log.out("Lookup Handle:", defs.lookupNamedHandle("Foo bar"));
self.log.out("Named Handle:", defs.createNamedHandle("Foo bar"));
self.log.out("Lookup Handle:", defs.lookupNamedHandle("Foo bar"));
self.log.out("Handle Name:", defs.lookupNamedHandleName(defs.lookupNamedHandle("Foo bar")));
self.log.out("Dead state:", defs.lookupState("Dead"));
self.log.out("Undefined Alive state:", defs.lookupState("Alive | Large"));
self.log.out("Undefined state:", defs.lookupState());
self.log.out("Dead state name:", defs.lookupStateName (defs.lookupState("Dead")));
self.log.out("Undefine state name:", defs.lookupStateName (mask.create([100])));
self.log.out("state name:", defs.lookupStateName (mask.create([1])));
self.log.out("state name:", defs.lookupStateName (mask.create([0x10])));
