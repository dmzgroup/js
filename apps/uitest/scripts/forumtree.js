var dmz =
       { module: require('dmz/runtime/module')
       , message: require("dmz/runtime/messaging")
       , ui:
          { consts: require('dmz/ui/consts')
          , file: require("dmz/ui/fileDialog")
          , loader: require('dmz/ui/uiLoader')
          , mainWindow: require('dmz/ui/mainWindow')
          , widget: require("dmz/ui/widget")
          , layout: require("dmz/ui/layout")
          , button: require("dmz/ui/button")
          , lineEdit: require("dmz/ui/lineEdit")
          }
       , timer: require('dmz/runtime/time')
       }
  , _main
  , _exports = {}
  , puts = require('sys').puts

  // UI elements
  , form = dmz.ui.loader.load("./scripts/ForumTreeForm.ui")
  , tree = form.lookup ("treeWidget")
  , textBox = form.lookup ("textEdit")
  , replyTitleText = form.lookup("titleEdit")
  , postText = form.lookup("postTextEdit")
  , submitPostButton = form.lookup("submitButton")

  // Global vars
  , newPostMessage = dmz.message.create(
       "NewPostMessage")

  // Function decls
  , forumRoot
  , convertNewPost
  , testMessages
  , newPost
  , collapseAll
  ;

tree.observe (self, "currentItemChanged", function (curr) {

   var data = curr.data(0);
   textBox.text (data.text.toString());
   if (submitPostButton.enabled()) {

      submitPostButton.enabled(false);
      replyTitleText.enabled(false);
      postText.enabled(false);
   }

   replyTitleText.enabled(true);
   postText.enabled(true);
   submitPostButton.enabled(true);
   submitPostButton.observe(self, "clicked", function () {

      var post = postText.text()
        , title = replyTitleText.text()
        ;
      replyTitleText.clear();
      postText.clear();
      replyTitleText.enabled(false);
      postText.enabled(false);
      submitPostButton.enabled(false);
      newPost(data, post, "Obama", title);
   });
});

forumRoot =
   { children: []
   , text: "Root"
   , poster: "Root"
   , title: "Root"
   , id: ""
   , widget: tree.root()
   };

newPost = function (parent, text, poster, title) {

   var result
     , textWidget
     , layout
     , button
     ;

   result =
      { children: [] // Array of posts
      , text: text // String
      , poster: poster // String username
      , title: title ? title: "Re: " + parent.title // String
      , id: parent.id + parent.children.length.toString() // String
      , date: new Date() // Replace with additional argument -- need to take post date as another piece of data
      };

   result.widget = parent.widget.add ([result.title, result.poster, result.date, result.id]);
   result.widget.data(0, result);
   parent.widget.expand();
   tree.currentItem(result.widget);
   return result;
};

convertNewPost = function (id, text, poster, title) {

  var currPost = forumRoot
    , idx
    , chr
    , result = false
    ;

  idx = 0;

  while ((idx < id.length - 1) && currPost) {

     chr = parseInt(id.charAt(idx));

     if (chr !== "") {

        if (chr < currPost.children.length) { currPost = currPost.children[chr]; }
        else {

           puts ("Invalid ID string -- tried to access nonexistent child.");
           currPost = false;
        }
     }
     else {

        puts ("Error reading character:", id.charAt(idx));
        currPost = false;
     }

     idx += 1;
  }

  if (currPost) {

     chr = parseInt(id.charAt(id.length - 1));
//     puts (id, "inserted to:", currPost.id);
     currPost.children[chr] = newPost(currPost, text, poster, title);
     result = true;
  }

  return result;
};

puts('Script: ' + self.name);

form.show();

//widget.lookup("lcd").value (100);

//form.observe(self, "pushButton", "clicked", function () { tree.clear(); });

tree.columnCount (4);
tree.headerLabels (["Title", "Poster", "Post Time", "PostID"]);

testMessages =
   [ { id: "0", title: "Congress", poster: "", text: "" }
   , { id: "1", title: "White House", poster: "", text: "" }
   , { id: "2", title: "Drug Cartel", poster: "", text: "" }
   , { id: "00", title: "Health Care", poster: "Kennedy", text: "Nationalize Health Care!" }
   , { id: "000", title: "", poster: "Nixon", text: "Don't be silly" }
   , { id: "001", title: "I don't know...", poster: "Clinton", text: "This seems like a bit of a risky idea." }
   , { id: "002", title: "GENIUS!", poster: "Obama", text: "I'm going to do this for sure!" }
   , { id: "0020", title: "", poster: "Palin", text: "STFU DIAF" }
   , { id: "0021", title: "Not if I have anything to say about it!", poster: "Palin",  text: "I'm coming for you!" }
   , { id: "01", title: "I can see Russia!", poster: "Palin", text: "See! I can be famous!" }
   , { id: "10", title: "Budget", poster: "Biden", text: "Are we screwed, or what?" }
   , { id: "11", title: "Small Government", poster: "Tea Party", text: "We hate big government!" }
   , { id: "110", title: "Uh...", poster: "NY Times", text: "So we should cut all government spending?" }
   , { id: "1100", title: "", poster: "Tea Party", text: "Hell yes!" }
   , { id: "11000", title: "", poster: "NY Times", text: "So we'll start by cutting unemployment, and medicare..." }
   , { id: "110000", title: "", poster: "Tea Party", text: "Wait, what? No, I LIKE that stuff. It helps ME!" }
   , { id: "20", title: "Smoking crack", poster: "Junkie Jim", text: "I want to quit!" }
   , { id: "21", title: "$10000 bounty", poster: "Leader", text: "I'll pay $10000 for the head of that federal marshall" }
   , { id: "210", title: "Done", poster: "Enforcer", text: "I left his head on your front porch." }
   , { id: "200", title: "", poster: "Dealer", text: "Aww, don't be that way! Here's a free sample, on the house." }
   , { id: "22", title: "Conquering America", poster: "Leader", text: "Let's get this underway already!" }
   ];

testMessages.forEach (function (message) {

   if (!convertNewPost (message.id, message.text, message.poster, message.title)) {

      puts (message.id + ": failed");
   }
});

tree.resizeColumnToContents(0);
tree.collapseAll();


dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});
