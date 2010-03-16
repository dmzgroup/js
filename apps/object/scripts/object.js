var puts = require('sys').puts;
var vector = require('dmz/types/vector');
var mask = require('dmz/types/mask');
var util = require('dmz/types/util');
var createError = require('dmz/types/util').createError;
var defs = require('dmz/runtime/definitions');
var time = require('dmz/runtime/time');
var data = require('dmz/runtime/data');
var message = require('dmz/runtime/messaging');
var undo = require('dmz/runtime/undo');
var config = require('dmz/runtime/config');
var ot = require('dmz/runtime/objectType');
var obj = require('dmz/framework/object');

obj.create.observe(self, function (handle, type) {

   puts("created:", handle, type);
});

var cb = obj.position.observe(self, function (handle, attr, pos, lastpos, self) {

   puts("Got pos:", pos, lastpos);

});

var o1 = obj.create("tank");
obj.position(o1, null, [1,2,3]);
obj.activate(o1);
obj.release(self, cb);
obj.position(o1, null, [4,5,6]);
var cb = obj.position.observe(self, cb);
obj.position(o1, null, [7,8,9]);
