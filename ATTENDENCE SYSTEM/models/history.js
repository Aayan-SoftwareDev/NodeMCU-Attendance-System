const mongoose = require("mongoose");

const schema = new mongoose.Schema({
    name: {
        type: String,
        required: true,
    },
    id: {
        type: String,
        required: true,
    },
    class: {
        type: String,
        required: true,
    },
    section: {
        type: String,
        required: true,
    },
    date: {
        type: Date,
        required: true,
    }
});

const histroy_model = mongoose.model('history', schema);
module.exports = {histroy_model};