/*
    This file is a Router that handles requests related to user authentication, 
    the index.js will redirect a request to this router if it includes
    '/user' in the path.
*/

// Neccessary imports 
const express = require('express');
const {controllerSignin} = require('../controllers/user')
const router = express.Router();

// The signup routes 
router.post('/signin', controllerSignin); // for signin

// Export the router
module.exports = router;