const express = require("express");
const {entryController} = require("../controllers/attendence")
const att_router = express.Router();

att_router.post('/', entryController);

module.exports = {att_router};