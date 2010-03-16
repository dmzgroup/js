#ifndef DMZ_JS_FUNCTIONS_DOT_H
#define DMZ_JS_FUNCTIONS_DOT_H

void
set_js_functions (QStringList &list) {

   list
      << "\\bArray.isArray\\b"
      << "\\bObject.create\\b"
      << "\\bObject.keys\\b"
      << "\\bObject.getOwnPropertyNames\\b"
      << "\\bObject.getPrototypeOf\\b"
      << "\\bObject.getOwnPropertyDescriptor\\b"
      << "\\bObject.defineProperty\\b"
      << "\\bObject.defineProperties\\b"
      << "\\bJSON.stringify\\b"
      << "\\bJSON.parse\\b"
      << "\\bprint\\b"
      ;
}

#endif //  DMZ_JS_FUNCTIONS_DOT_H
