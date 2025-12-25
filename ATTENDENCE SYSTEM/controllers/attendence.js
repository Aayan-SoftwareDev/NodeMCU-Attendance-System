const {student} = require("../models/students");
const {present_students} = require("../models/present_students");
const {histroy_model} = require("../models/history");

async function entryController(req, res){
    if(!req.body.name || !req.body.id){ console.log(false); return res.status(401).json({success: false});};
    console.log(req.body.name);
    console.log(req.body.id);
    const already_present = await present_students.findOne({identification: req.body.id})
    if (already_present) return res.status(200).json({success: true});
    const stud = await student.findOne({id:req.body.id});
    if(!stud) return res.status(404).json({success: false});
    const now = new Date();
    const newHistory = await histroy_model.create({
        name: stud.name,
        class: stud.class,
        section: stud.section,
        date: now,
        id: stud.id,
    });

    if(!newHistory) res.status(500).json({success: false});

    let hours = now.getHours();
    const minutes = now.getMinutes();
    const seconds = now.getSeconds();
    hours = hours % 12;
    hours = hours ? hours : 12;
    const formattedTime = 
        `${hours.toString().padStart(2, '0')}:` +
        `${minutes.toString().padStart(2, '0')}:` +
        `${seconds.toString().padStart(2, '0')}`;
    
    const new_present_student = await present_students.create({
        name: req.body.name,
        class: stud.class,
        timeIn: formattedTime,
        identification: req.body.id,
        section: stud.section,
    });
    
    if(!new_present_student) return res.status(500).json({success: false});

    return res.status(201).json({success: true});

}

module.exports = {entryController};