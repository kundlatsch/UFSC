const path = require("path");
const fs = require("fs");
const notesPath = path.join(__dirname, "notas.txt");

function fetchNotes() {
	try {
		let notesString = fs.readFileSync(notesPath, "utf8");
		return notes = JSON.parse(notesString);
	}
	catch(error) {
		return [];
	}
}

function saveNotes(notes) {
	fs.writeFileSync(notesPath, JSON.stringify(notes));
}

function addNote(title, body){
	let notes = fetchNotes();

	let note = {
		title, 
		body
	};

	var duplicatedNotes = notes.filter((note) => note.title === title);
	
	if (!!duplicatedNotes.length) {
		notes.push(note);
		saveNotes(notes);
		return note;
	}
}

function getAll() {
	let allNotes = fetchNotes();
	return allNotes;
}



module.exports = {addNote, getAll};