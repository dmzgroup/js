// Requires dmz-core.js

DMZ.Vector = function () {

   this.x = 0;
   this.y = 0;
   this.z = 0;
};

DMZ.Vector.create = function () {

   return new DMZ.Vector();
};


DMZ.Vector.prototype.create = function () {

   return new DMZ.Vector();
};


DMZ.Vector.prototype.toString = function () {

   return "[" + this.x + ", " + this.y + ", " + this.z + "]";
};


DMZ.Vector.prototype.fromArray = function (values) {

   this.x = values[0];
   this.y = values[1];
   this.z = values[2];

   return this;
};


DMZ.Vector.prototype.toArray = function () {

   return [this.x, this.y, this.z];
};


DMZ.Vector.prototype.setXYZ = function (x, y, z) {
   
   this.x = x;
   this.y = y;
   this.z = z;

   return this;
};


DMZ.Vector.prototype.magnitude = function () {

   var result = Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
   return result > DMZ.Epsilon ? result : 0.0;
};


DMZ.Vector.prototype.add = function (vec) {

   return this.create().setXYZ(this.x + vec.x, this.y + vec.y, this.z + vec.z);
};


DMZ.Vector.prototype.subtract = function (vec) {

   return this.create().setXYZ(this.x - vec.x, this.y - vec.y, this.z - vec.z);
};


DMZ.Vector.prototype.multiplyConst = function (k) {

   return this.create().setXYZ(this.x * k, this.y * k, this.z * k)
};


DMZ.Vector.prototype.dot = function (vec) {

   return (this.x * vec.x) + (this.y * vec.y) + (this.z * vec.z);
};


DMZ.Vector.prototype.cross = function (vec) {

   return this.create().setXYZ(
      (this.y * vec.z) - (this.z * vec.y),
      (this.z * vec.x) - (this.x * vec.z),
      (this.x * vec.y) - (this.y * vec.x));
};


DMZ.Vector.prototype.getAngle = function (vec) {

   return Math.atan2(this.cross(vec).magnitude(), this.dot(vec));
};

