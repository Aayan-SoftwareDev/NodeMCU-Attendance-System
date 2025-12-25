// Import the mongoose package
const mongoose = require('mongoose');

// Make the mongoose schema for USERS
const userSchema = new mongoose.Schema({
    // The username which he enters during signup
    username: {
        type: String, // Its type should be string
        required: true, // It is required
    },

    // The password which he enters during signup, it should be unique
    password: {
        type: String, // Its type should be string
        required: true, // It is required
        unique: true, // It should be unique
    },
});

// make the model for Users using the userSchema
const user = mongoose.model('user', userSchema);

// Export the user model
module.exports = { user };