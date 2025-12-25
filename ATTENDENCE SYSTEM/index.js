/*
    This is the main file of the project, this project is used to create an authentication
    system using the express framework and using cookies, meaning that it is stateful authentication.
*/

// Imports
const express = require('express');
const userRoutes = require('./routes/user');
const { connectMongoDB } = require('./connect');
const cookieParser = require('cookie-parser');
const { session } = require('./models/session');
const { att_router } = require("./routes/attendence");
const path = require('path');
const { present_students } = require("./models/present_students");
const {histroy_model} = require('./models/history');
const {student} = require("./models/students")

// WebSocket imports
const http = require("http");
const { Server } = require("socket.io");

// App
const app = express();
const PORT = 8000;

// Create HTTP + WebSocket server
const server = http.createServer(app);
const io = new Server(server);

// View engine setup
app.set('view engine', 'ejs');
app.set('views', path.resolve('./views'));

// Middlewares
app.use(cookieParser());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// MongoDB connect
const url = "mongodb://0.0.0.0:27017";
connectMongoDB(url);

// Routes
app.use("/entry", att_router);
app.use('/user', userRoutes);

// Authentication middleware
app.use(async (req, res, next) => {
    if (!req.cookies.session_id) {
        return res.status(401).render('signin.ejs');
    }

    const sessionId = req.cookies.session_id;
    const findSession = await session.findOne({ sessionId });

    if (!findSession) {
        return res.status(401).end("<p>Not</p>");
    }

    req.userId = findSession.userId;
    next();
});

app.get('/', (req, res) => {
    res.render('dashboard.ejs');
})

// Main page
app.get('/present', async (req, res) => {
    const entries = await present_students.find();
    res.render("attendence.ejs", { entries });
});

app.get('/history', async (req, res) => {
    const entries = await histroy_model.find();
    res.render("history.ejs", {entries});
});

app.get('/absent', async (req, res) => {
    const present = await present_students.find({});
    const presentIds = present.map(s => s.identification);
    const absentStudents = await student.find({
        id: {$nin: presentIds}
    });
    res.render("absent.ejs", {entries:absentStudents});
});

io.on("connection", (socket) => {
    console.log("Client connected");

    // Send initial datasets
    sendAttendance(socket);
    sendHistory(socket);
    sendAbsent(socket)

    // Update both every 5 sec
    setInterval(() => {
        sendAttendance(socket);
        sendHistory(socket);
        sendAbsent(socket);
    }, 5000);
});

// WebSocket Logic
async function sendAttendance(socket) {
    const entries = await present_students.find();
    socket.emit("update-attendance", entries);
}

async function sendHistory(socket) {
    const history = await histroy_model.find();
    socket.emit("update-history", history);
}

async function sendAbsent(socket) {
    const present = await present_students.find({});
    const presentIds = present.map(s => s.identification);
    const absentStudents = await student.find({
        id: {$nin: presentIds}
    });
    socket.emit("update-absent", absentStudents);
}

// Start server
server.listen(PORT, () => {
    console.log(`Server is running on PORT: ${PORT}`);
});