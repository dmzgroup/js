var util = require('dmz/types/util');
var createError = util.createError;

var Mask = function () {

   this.bits = [];
};

exports.create = function () {

   var result = new Mask();
   if (arguments.length > 0) { result.set.apply(result, arguments); }
   return result;
};


Mask.prototype.create = exports.create;


Mask.prototype.copy = function () {

   return this.create(this);
};


Mask.prototype.clear = function () { this.bits = []; };


Mask.prototype.toString = function () {

   var size = this.bits.length,
      bit = 0,
      place = 0,
      result = '[';

   for (place = 0; place < size; place++) {

      if (place > 0) { result += " | "; }

      for (bit = 31; bit >= 0; bit--) {

         if ((bit < 31) && (((bit + 1) % 4) === 0)) { result += " "; }
         result += this.bits[place] & (1 << bit) ? "1" : "0";
      }
   }

   return result + "]";
};


Mask.prototype.fromArray = function (values) {

   var ix = 0;

   if (Array.isArray(values)) {

      this.bits = new Array(values.length);

      for (ix = 0; ix < this.bits.length; ix++) {

         this.bits[ix] = values[ix];
      }
   }

   return this;
};


Mask.prototype.toArray = function () {

   var result = new Array(this.bits.length),
      ix = 0;

   for (ix = 0; ix < this.bits.length; ix++) {

      result[ix] = this.bits[ix];
   }

   return result;
};


Mask.prototype.set = function () {

   var arg;

   if (arguments.length >= 1) {

      arg = arguments[0];

      if (Array.isArray(arg)) { this.fromArray(arg); }
      else if (Array.isArray(arg.bits)) { this.fromArray(arg.bits); }
      else { throw createError("Invalid values passed to Mask.set function."); }
   }
   
   return this;
};


Mask.prototype.bool = function () {

   var result = false,
      count = 0;

   while (!result && (count < this.bits.length)) {

      if (this.bits[count] !== 0) { result = true; }
      else { count++; }
   }

   return result;
};


Mask.prototype.contains = function (mask) {

   return this.and(mask).equals(mask).bool();
};


Mask.prototype.unset = function (mask) {

   var size = 0,
      length = this.bits.length,
      ix = 0,
      result = this.copy();
   
   if (Array.isArray(mask.bits)) {
     
      size = length > mask.bits.length ? length : mask.bits.length;

      for (ix = 0; ix < size; ix++) { result.bits[ix] &= ~(mask.bits[ix]); }
   }

   return result;
};


Mask.prototype.and = function (mask) {

   var result = this.copy(),
      size = this.bits.length,
      ix = 0,
      length = 0;

   if (Array.isArray(mask.bits)) {

      length = mask.bits.length;

      for (ix = 0; ix < size; ix++) {

         result.bits[ix] &= (length > ix) ? mask.bits[ix] : 0;
      }
   }

   return result;
};


Mask.prototype.or = function (mask) {

   var result = this.copy(),
      size = 0,
      ix = 0;

   if (Array.isArray(mask.bits)) {

      size = mask.bits.length;

      for (ix = 0; ix < size; ix++) {

         result.bits[ix] |= mask.bits[ix];
      }
   }

   return result;
};


Mask.prototype.xor = function (mask) {

   var result = this.copy(),
      size = 0,
      ix = 0;

   if (Array.isArray(mask.bits)) {

      size = mask.bits.length;

      for (ix = 0; ix < size; ix++) {

         result.bits[ix] ^= mask.bits[ix];
      }
   }

   return result;
};


Mask.prototype.not = function () {

   var result = this.copy (),
      ix = 0;

   for (ix = 0; ix < result.bits.length; ix++) {

      result.bits[ix] = ~(this.bits[ix]);
   }

   return result;
};


Mask.prototype.equal = function (mask) {

   var result = false,
      count = 0,
      done = false,
      len1 = 0,
      len2 = 0,
      maxSize = 0;

   if (Array.isArray (mask.bits)) {

      len1 = this.bits.length;
      len2 = mask.bits.length;

      maxSize = len1 > len2 ? len1 : len2;

      while (!done) {

         if (((count < len1) ? this.bits[count] : 0) !=
               ((count < len2) ? mask.bits[count] : 0)) {

            done = true;
            result = false;
         }
         else {

            count++;
            if (count >= maxSize) { done = true; }
         }
      }
   }

   return result;
};


/*
// To be implemented later -rb
Mask.prototype.shift = function (shift) {

   var result = this.create().set(this);

   return result;
}
*/
