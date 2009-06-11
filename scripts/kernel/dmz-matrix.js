// Requires dmz-core.js

DMZ.Matrix = function () {

   this.v = [1, 0, 0, 0, 1, 0, 0, 0, 1];
};

DMZ.Matrix.create = function () {

   return new DMZ.Matrix();
};


DMZ.Matrix.prototype.create = function () {

   return new DMZ.Matrix();
};


DMZ.Matrix.prototype.set = function () {

   var v;

   if (arguments.length === 1) {

      if (DMZ.isArray (arguments[0].v)) {

         v = arguments[0].v;
      }
      else if (DMZ.isArray(arguments[0])) {

         v = arguments[0];
      }
      else { throw "Invalid DMZ.Matrix initialization value"; }
   }
   else if (arguments.length === 9) {

      v = arguments;
   } 
   else { throw "Invalid number of parameters for DMZ.Matrix.set()"; }

   if (v !== undefined) {

      this.fromArray (v)
   }

   return this;
};


DMZ.Matrix.prototype.toString = function () {

   return "[" + this.v[0] + ", " + this.v[1] + ", " + this.v[2] + ", " +
      this.v[3] + ", " + this.v[4] + ", " + this.v[5] + ", " +
      this.v[6] + ", " + this.v[7] + ", " + this.v[8] + "]";
};


DMZ.Matrix.prototype.fromArray = function (values) {

   if (values.length >= 9) {

      for (var ix = 0; ix < 9; ix++) { this.v[ix] = values[ix] }
   }

   return this;
};


DMZ.Matrix.prototype.toArray = function () {

   return [this.v[0], this.v[1], this.v[2], this.v[3], this.v[4], this.v[5], this.v[6], this.v[7], this.v[8]];
};


DMZ.Matrix.prototype.multiply = function (mat) {

   var result = this.create();
   var sum = 0.0;
   var offset0 = 0, offset1 = 0, offset2 = 0;

   for (var row = 0; row < 3; row++) {

      for (var col = 0; col < 3; col++) {

         offset0 = (row * 3) + col;
         sum = 0.0;

         for (var count = 0; count < 3; count++) {

            offset1 = (row * 3) + count;
            offset2 = (count * 3) + col;
            sum += this.v[offset1] * mat.v[offset2];
         }

         result.v[offset0] = sum;
      }
   }

   return result;
};


DMZ.Matrix.prototype.transform = function (vec) {

   var result = DMZ.Vector.create();

   if (DMZ.hasFunction (vec, "toArray")) { 

      vec = vec.toArray();
   }
   else if (DMZ.isArray(vec) === false) {

      if (arguments.length >= 3) { vec = arguments; }
      else { throw "Invalid argument(s) passed to DMZ.Matrix.transform"; }
   }

   return result.setXYZ (
      (this.v[0] * vec[0]) + (this.v[1] * vec[1]) + (this.v[2] * vec[2]),
      (this.v[3] * vec[0]) + (this.v[4] * vec[1]) + (this.v[5] * vec[2]),
      (this.v[6] * vec[0]) + (this.v[7] * vec[1]) + (this.v[8] * vec[2]));
}
