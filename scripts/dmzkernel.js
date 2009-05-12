var DMZ = (DMZ ? DMZ : {});


DMZ.Epsilon = 0.000000001;


DMZ.Vector = function () {

   this.x = 0;
   this.y = 0;
   this.z = 0;
}

DMZ.Vector.create = function () {

   return new DMZ.Vector ();
}


DMZ.Vector.prototype.create = function () {

   return new DMZ.Vector ();
}


DMZ.Vector.prototype.toString = function () {

   return "[" + this.x + ", " + this.y + ", " + this.z + "]";
}


DMZ.Vector.prototype.fromArray = function (values) {

   this.x = values[0];
   this.y = values[1];
   this.z = values[2];
}


DMZ.Vector.prototype.toArray = function () {

   return [this.x, this.y, this.z];
}


DMZ.Vector.prototype.setXYZ = function (x, y ,z) {
   
   this.x = x;
   this.y = y;
   this.z = z;

   return this;
}


DMZ.Vector.prototype.magnitude = function () {

   var result =  Math.sqrt ((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
   return result > DMZ.Epsilon ? result : 0.0;
}


DMZ.Vector.prototype.add = function (vec) {

   var result = this.create ();

   result.x = this.x + vec.x;
   result.y = this.y + vec.y;
   result.z = this.z + vec.z;

   return result;
}


DMZ.Vector.prototype.subtract = function (vec) {

   var result = this.create ();

   result.x = this.x - vec.x;
   result.y = this.y - vec.y;
   result.z = this.z - vec.z;

   return result;
}


DMZ.Vector.prototype.multiplyConst = function (k) {

   var result = this.create ();

   result.x = this.x * k;
   result.y = this.y * k;
   result.z = this.z * k;

   return result;
}


DMZ.Vector.prototype.dot = function (vec) {

   return (this.x * vec.x) + (this.y * vec.y) + (this.z * vec.z);
}


DMZ.Vector.prototype.cross = function (vec) {

   var result = this.create ();

   result.x = (this.y * vec.z) - (this.z * vec.y);
   result.y = (this.z * vec.x) - (this.x * vec.z);
   result.z = (this.x * vec.y) - (this.y * vec.x);

   return result;
}


DMZ.Vector.prototype.getAngle = function (vec) {

   return Math.atan2 (this.cross (vec).magnitude (), this.dot (vec));
}

var vec1 = DMZ.Vector.create ().setXYZ (1, 2, 3);
var vec2 = DMZ.Vector.create ().setXYZ (4, 5, 6);

print ("Angle: " + vec1.getAngle (vec2))
