const mongoose = require('mongoose');

// Make the mongoose schema for SESSIONS
const sessionSchema = new mongoose.Schema({
    sessionId: {
        type: String,
        required: true,
        unique: true,
    },
    userId: {
        type: mongoose.Schema.Types.ObjectId,
        ref: 'user',
        required: true,
    }
});

// make the model for Sessions using the sessionSchema
const session = mongoose.model('session', sessionSchema);

// Export the session model
module.exports = { session };