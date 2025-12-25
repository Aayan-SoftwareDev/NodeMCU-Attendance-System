const mongoose = require("mongoose");

const shcema = new mongoose.Schema({
    name: {
        type: String,
        required: true
    },

    class: {
        type: String,
        required: true
    },
    id: {
        type: String,
        required: true,
    },
    section: {
        type: String,
        required: true,
    }
});

const student = mongoose.model('students', shcema);

module.exports = {student}