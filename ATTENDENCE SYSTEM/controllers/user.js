/*
    This file contains the controller function for the user router, 
    it contains function for user authentication like signup and signin.
*/

// Imports that are neccessary
const {user} = require('../models/user');
const bcrypt = require('bcrypt');
const { v4: uuidv4 } = require('uuid');
const {session} = require('../models/session')

// function for signin
async function controllerSignin(req, res){
    // check if the body includes username and password
    if (!req.body.username || !req.body.password){
        return res.status(400).end("<p>Not logged</p>");
    }
    // extract the username and password from the body
    const {username, password} = req.body;
    // find the username in the database
    const existUser = await user.findOne({username});
    // if the user does not exist, return status 404
    if(!existUser){
        return res.status(404).end("<p>Not logged</p>");
    }
    // Now it the username is valid compare the password in the request with hashed password
    const isPasswordValid = await bcrypt.compare(password, existUser.password);
    // if wrong password, return status 401
    if(!isPasswordValid){
        return res.status(401).end("<p>Not logged</p>");
    }
    // generate a random string and store in variable for sessionId in the cookie
    const sessionId = uuidv4();
    // set the cookie in the response
    res.cookie('session_id', sessionId, {maxAge: 24 * 60 * 60 * 1000});
    // create a new session in the session collection
    const newSession = await session.create({
        sessionId: sessionId,
        userId: existUser._id,
    })
    if(!newSession){
        return res.status(500).end("<p>Not logged</p>");
    }

    return res.status(200).redirect("http://localhost:8000/"); // if successful
}

// export the contoller functions
module.exports = {controllerSignin};