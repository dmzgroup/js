var puts = require('sys').puts;
var vector = require('dmz/types/vector');
var mask = require('dmz/types/mask');
var util = require('dmz/types/util');
var createError = require('dmz/types/util').createError;
var defs = require('dmz/runtime/definitions');
var time = require('dmz/runtime/time');
var data = require('dmz/runtime/data');
var messaging = require('dmz/runtime/messaging');
var undo = require('dmz/runtime/undo');
var config = require('dmz/runtime/config');
var ot = require('dmz/runtime/objectType');

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

self.log.out(self.log);

var d1 = data.create();
d1.number("foo", 0, 1001);
d1.number("foo", 1, 2002);
var handle = defs.createNamedHandle("goo");
d1.number(handle, 0, 3003);
d1.string("moo", 0, "a string value");
d1.string("moo", 1, v1);
d1.vector("voo", 0, v1);
d1.vector("voo", 1, v2);
d1.vector("voo", 2, v3);
d1.boolean("boo", 0, undefined);
d1.boolean("boo", 1, true);
d1.boolean("boo", 2, false);
d1.boolean("boo", 3, v1);
self.log.out("v2 =", d1.vector("voo", 1), v2);
self.log.out("Data:", d1.number("foo", 0), d1);
self.log.out("wrap string:", data.wrapString("This string is wrapped"));
self.log.out("wrap nan number:", data.wrapNumber("This string is wrapped"));
self.log.out("wrap number:", data.wrapNumber(v1.x));
var m1 = messaging.create("Foo Bar Message");
m1.subscribe(self, function (inData) { puts("****** received:", data.unwrapString(inData)); });
m1.send (data.wrapString("This is the string payload"));
m1.unsubscribe(self);
m1.send (data.wrapString("This is the string payload"));
self.log.out("Message:", m1);
self.log.out("Undo Types:", undo.UndoType, undo.RedoType);
self.log.out("State Names:", defs.DeadStateName, defs.DeactivateStateName, defs.SmokingStateName);
var cd = config.create(self.name);
self.log.out ("Print config:", self.config);
self.log.out (self.config.toVector ("vec"));
self.log.out (self.config.toString ("name.value"));
self.log.out (self.config.toString ("no-default.value"));
self.log.out (self.config.toString ("not-defined.value", "This is the default value"));
var objList = self.config.get("obj");
objList.forEach(function (cd) { self.log.out (cd.toString("value")); });
var gv = ot.lookup("ground-vehicle");
var ct = gv.getChildren();
ct.forEach (function (type) { self.log.out(gv.getName(), "child =", type.getName()); });
self.log.out ("is undefine:", util.isUndefined (self.foo));

var cb = time.setRepeatingTimer(self, 1, function () { puts("******* Hello World!!!"); });

time.setTimer(self, 4, function (dt) {

   puts("Killing repeating timer.", dt);
   time.cancleTimer(self, cb);

   puts("System Time:", time.getSystemTime());
   puts("Frame Time: ", time.getFrameTime());
   puts("Frame Delta:", time.getFrameDelta());
});

time.setRepeatingTimer(self, 3, function () { puts("this is the other timer."); });

