
var io = require('socket.io').listen(2000);
console.log('Server on port 2000.');

// create databases dir
var fs = require("fs");
if (!fs.existsSync("databases")) {
    fs.mkdirSync("databases", function (err) {
        if (err) {
            console.log(err);
            return;
        }
    });
}

var sqlite3 = require('sqlite3');
//init databases
var db = new sqlite3.Database('databases/NoteDB.sqlite3');
db.run("CREATE TABLE IF NOT EXISTS Note (username TEXT PRIMARY KEY, password TEXT)");
db.close();

io.sockets.on('connection', function (socket) 
{
    socket.on('signup', function (data)
    {
        data = JSON.parse(data);
        // console.log(data.username);
        // console.log(data.password);
        var db = new sqlite3.Database('databases/NoteDB.sqlite3');
        var stmt = db.prepare("INSERT OR REPLACE INTO note (username, password) VALUES (?,?)");
        stmt.run(data.username, data.password);
        stmt.finalize();
        db.close();
    });
        //×¢²á¿Í»§¶ËÏûÏ¢
    socket.on('message', function (data) 
    {
        console.log(data);
    });
    
    socket.on('signin', function(data)
    {
        data = JSON.parse(data);
        var db = new sqlite3.Database('databases/NoteDB.sqlite3');
        db.all("SELECT * FROM note WHERE username = $name AND password = $pass", 
            {$name: data.username, 
             $pass: data.password}, function(err, res){
                if(!err){
                    // console.log("signin has no error!");
                    // console.log(data.username);
                    // console.log(data.password);
                    var jsonObj = {
                        ResultCode: 0,
                        Record: res
                    };
                    socket.emit('findRes', jsonObj);
                }
                else
                    console.log("signin has error!");
            });
        // db.all("SELECT username, password FROM Note", function (err, res) {
        //     if (!err) {
        //         var jsonObj = {
        //             ResultCode: 0,
        //             Record: res
        //         };
        //         socket.emit('findRes', jsonObj);
        //     }
        // });
        db.close();
    });     
    
        //×¢²ácallServerEventÊÂ¼þ£¬±ãÓÚ¿Í»§¶Ëµ÷ÓÃ
    socket.on('callServerEvent', function (data) 
    {
        // console.log('get it!');
         // console.log(data);
                //Ïò¿Í»§¶Ë·¢ËÍÏûÏ¢£¬µ÷ÓÃ¿Í»§¶ËµÄcallClientEventÊÂ¼þ
                socket.emit('callClientEvent', { message: 'hello liuyangguang.' });
    });

});                                                                       