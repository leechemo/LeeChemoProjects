require("dotenv").config();
//#region express configures
var express = require("express");
var path = require("path");
var logger = require("morgan");
// const session = require("client-sessions");
const session = require("express-session");

const DButils = require("./routes/utils/DButils");
const cors = require('cors')

var app = express();

const corsOptions = {
  origin: ["http://localhost:8082"], 
  credentials: true
};
app.use(cors(corsOptions));


app.use(logger("dev")); //logger
app.use(express.json()); // parse application/json
// app.use(
//   session({
//     cookieName: "session", // the cookie key name
//     //secret: process.env.COOKIE_SECRET, // the encryption key
//     secret: "template", // the encryption key
//     duration: 24 * 60 * 60 * 1000, // expired after 20 sec
//     activeDuration: 1000 * 60 * 5, // if expiresIn < activeDuration,
//     cookie: {
//       httpOnly: false,
//     }
//     //the session will be extended by activeDuration milliseconds
//   })
// );

app.use(session({
  name: 'session',
  secret: 'template',
  resave: false,
  saveUninitialized: false,
  cookie: {
    httpOnly: true,
    secure: false,      
    sameSite: 'lax'      
  }
}));

app.use(express.urlencoded({ extended: false })); // parse application/x-www-form-urlencoded
app.use(express.static(path.join(__dirname, "public"))); //To serve static files such as images, CSS files, and JavaScript files
//local:
// app.use(express.static(path.join(__dirname, "dist")));
//remote:
app.use(express.static(path.join(__dirname, '../assignment-3-3-frontend/dist')));

app.get("/",function(req,res)
{ 
  //remote: 
  res.sendFile(path.join(__dirname, '../assignment-3-3-frontend/dist/index.html'));
  //local:
  // res.sendFile(__dirname+"/index.html");

});




var port = process.env.PORT || "3001"; //local=3001 remote=80
//#endregion
const user = require("./routes/user");
const recipes = require("./routes/recipes");
const auth = require("./routes/auth");


//#region cookie middleware לבדוק אם מי שנרשם משתמש או לא
app.use(function (req, res, next) {
  if (req.session && req.session.username) {
    DButils.execQuery("SELECT username FROM users")
      .then((users) => {
        if (users.find((x) => x.username === req.session.username)) {
          req.username = req.session.username;
        }
        next();
      })
      .catch((error) => next());
  } else {
    next();
  }
});
//#endregion

// ----> For cheking that our server is alive
app.get("/alive", (req, res) => res.send("I'm alive"));

// Routings
app.use("/user", user);
app.use("/recipes", recipes);
app.use("/", auth);



// Default router
app.use(function (err, req, res, next) {
  console.error(err);
  res.status(err.status || 500).send({ message: err.message, success: false });
});

const server = app.listen(port, () => {
  console.log(`Server listen on port ${port}`);
});

process.on("SIGINT", function () {
  if (server) {
    server.close(() => console.log("server closed"));
  }
  process.exit();
});

module.exports = app;