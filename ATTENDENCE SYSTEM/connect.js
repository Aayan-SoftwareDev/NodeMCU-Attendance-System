const mongoose = require('mongoose');

async function connectMongoDB(url){
    mongoose.connect(url)
    .then(() => console.log("connected to MongoDB"))
    .catch(() => console.log("failed to connect to MongoDB"));
}

module.exports = {connectMongoDB};