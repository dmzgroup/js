var test = require("dmz/test")
  , vector = require("dmz/types/vector")
  ;

test.start("dmzJsVectorTest");

var v1 = vector.create();

test.validate("vector.isZero()", v1.isZero());
test.validate("vector.isTypeOf()", vector.isTypeOf(v1));

test.stop();
