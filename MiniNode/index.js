const path = require("path");
const fs = require("fs");
const argv = require("yargs").argv;

const notes = require("./notes");

// Criar pasta
// fs.mkdir(path.join(__dirname, "teste"), {}, (err) => {
// 	if (err) {
// 		console.log("pasta já existe");
// 	}
// 	else {
// 		console.log("pasta criada");
// 	}
// })


// Criar, ler e escrever arquivo HTML
// fs.writeFile(path.join(__dirname, "index.html"), "<h1>Hello World!</h1>", {}, (err) => {
// 	if (err) throw err;

// 	fs.appendFile(path.join(__dirname, "index.html"), "\n<h1>APENDIDO</h1>", {}, (err) => {
// 		if (err) throw err;

// 		fs.readFile(path.join(__dirname, "index.html"), "utf8", (err, data) => {
// 			if (err) throw err;
// 			console.log(data);
// 		});
// 	});
// });


const command = argv._[0];

if(command === "add") {
	let {title, body} = argv;
	var note = notes.addNote(title, body);
	!!note ? console.log(note) : console.log("Nota duplicada");
}

else if(command === "list") {
	var allNotes = notes.getAll();
	allNotes.forEach(notes => {
		console.log(note);
	});
}

else if(command == "read") {

}

else if(command == "remove") {

}

else {
	console.log("Comando inválido");
}