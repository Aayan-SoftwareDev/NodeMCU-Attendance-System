const mongoose = require("mongoose");
const shcema = new mongoose.Schema({
    name: {
        type: String,
        required: true,
    },
    class: {
        type: String,
        required: true,
    },
    section: {
        type: String,
        required: true
    },
    
    timeIn: {
        type: String,
        required: true,
    },
    identification: {
        type: String,
        required: true
    },

    createdAt: {
        type: Date,
        default: Date.now,
        expires: 60 * 60 * 12   // 12 hours (in seconds)
    }
});

const present_students = mongoose.model('presentStudents', shcema);
module.exports = {present_students};