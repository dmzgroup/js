var dmz =
       { ui:
          { crypto: require("dmz/ui/crypto")
          }
       }
   , _log = require('sys').puts
   , _exports = {}
   , test =
        [ "mq2thez@gmail.com"
        , "sangster@me.com"
        , "benjamin.e.sangster@gmail.com"
        , "techsupport@chds.us"
        , "cwtrusko@nps.edu"
        ]
   ;

test.forEach(function (email) {

   _log (email + ": [" + dmz.ui.crypto.hash(email, dmz.ui.crypto.Sha1) + "]");
});
