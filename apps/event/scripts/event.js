var puts = require('sys').puts;
var vector = require('dmz/types/vector');
var mask = require('dmz/types/mask');
var util = require('dmz/types/util');
var defs = require('dmz/runtime/definitions');
var time = require('dmz/runtime/time');
var data = require('dmz/runtime/data');
var et = require('dmz/runtime/eventType');
var event = require('dmz/components/event');

var cb = event.create.observe(self, "event", function (handle, type) {

   puts("created:", handle, type, event.type(handle));
});

event.create.observe(self, "fire", function (handle, type) {

   puts("created2:", handle, type, event.type(handle));
});


event.close.observe(self, "event", function (handle, type) {

   puts("closed:", handle, type, event.type(handle), event.position(handle));
});

var e = event.create(et.lookup("fire"));
event.position(e, null, [1, 2, 3]);
event.close(e);

event.release(self, cb);

e = event.create("event");
event.position(e, null, [4,5,6]);
event.close(e);

e = event.create(et.lookup("fire"));
event.position(e, null, [7,8,9]);
event.close(e);
