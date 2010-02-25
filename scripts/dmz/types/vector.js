var util = require('dmz/types/util')
var Epsilon = util.Epsilon;

var Vector = function () {

   this.x = 0;
   this.y = 0;
   this.z = 0;
};

exports.create = function () {

   return new Vector();
};


Vector.prototype.create = exports.create;


Vector.prototype.copy = function () {

   return this.create().set(this);
};


Vector.prototype.toString = function () {

   return "[" + this.x + ", " + this.y + ", " + this.z + "]";
};


Vector.prototype.fromArray = function (values) {

   this.x = values[0];
   this.y = values[1];
   this.z = values[2];

   return this;
};


Vector.prototype.toArray = function () {

   return [this.x, this.y, this.z];
};


Vector.prototype.set = function () {

   var vaules;

   if (arguments.length === 1) {

      var arg = arguments[0];

      if (Vector.prototype.isPrototypeOf(arg)) {

         values = [ arg.x, arg.y, arg.z ];
      }
      else if (Array.isArray(arg) && (arg.length === 3)) {

         vaules = arg;
      }
      else { throw new Error("Invalid Vector initialization value"); }
   }
   else if (arguments.length === 3) {

      vaules = arguments;
   } 
   else { throw new Error("Invalid number of parameters for Vector.set()"); }

   if (vaules !== undefined) {

      this.fromArray (vaules)
   }

   return this;
};


Vector.prototype.setXYZ = function (x, y, z) {
   
   this.x = x;
   this.y = y;
   this.z = z;

   return this;
};


Vector.prototype.magnitude = function () {

   var result = Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
   return result > Epsilon ? result : 0.0;
};


Vector.prototype.normalized = function () {

   var mag = Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
   var div = 0.0;

   if (mag > Epsilon) {

      div = 1 / mag;
   }

   return this.multiplyConst (div);
}


Vector.prototype.add = function (vec) {

   return this.create().setXYZ(this.x + vec.x, this.y + vec.y, this.z + vec.z);
};


Vector.prototype.subtract = function (vec) {

   return this.create().setXYZ(this.x - vec.x, this.y - vec.y, this.z - vec.z);
};


Vector.prototype.multiplyConst = function (k) {

   return this.create().setXYZ(this.x * k, this.y * k, this.z * k)
};


Vector.prototype.dot = function (vec) {

   return (this.x * vec.x) + (this.y * vec.y) + (this.z * vec.z);
};


Vector.prototype.cross = function (vec) {

   return this.create().setXYZ(
      (this.y * vec.z) - (this.z * vec.y),
      (this.z * vec.x) - (this.x * vec.z),
      (this.x * vec.y) - (this.y * vec.x));
};


Vector.prototype.getAngle = function (vec) {

   return Math.atan2(this.cross(vec).magnitude(), this.dot(vec));
};

