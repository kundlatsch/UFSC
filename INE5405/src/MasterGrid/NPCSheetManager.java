package MasterGrid;

import java.io.IOException;
import java.util.ArrayList;

public class  NPCSheetManager implements Sheet {
	
	private ArrayList<String> titles = new ArrayList<String>();
	private ArrayList<String> contents = new ArrayList<String>();
	
	public NPCSheetManager () {
		super ();
	}

	@Override
	public void processSheets(String content) {
		String[] contentA = content.split(";");
		for (int i = 0; i < contentA.length - 1; i += 2) {
			this.addSheet(contentA[i], contentA[i + 1]);
		}
	}

	@Override
	public void addSheet (String title, String body) {
		titles.add(title);
		contents.add(body);
	}
	
	@Override
	public void removeSheet (int index) {
		titles.remove(index);
		contents.remove(index);
	}
	@Override
	public void editSheet (int index, String title, String body) {
		titles.set(index, title);
		contents.set(index, body);
	}
	
	@Override
	public String getDefaultSheet () {
		GenericFileReader characterModel = new GenericFileReader("NPCModel.txt");
		String sReturn = "";
		try {
			sReturn = characterModel.readFile();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			return "erro de leitura";
		}
		
		return sReturn;
	}

	@Override
	public String[] getTitles () {
		String[] arrayReturn = new String[titles.size()];
		arrayReturn = titles.toArray(arrayReturn);	
		return arrayReturn;
	}
	
	@Override
	public String getTitle (int index) {
		return titles.get(index);
	}
	
	@Override
	public String getContent (int index) {
		return contents.get(index);
	}
	
	@Override
	public String toString() {
		String sReturn = "";
		for (int i = 0; i < titles.size(); i++) {
			sReturn += titles.get(i) + ";" + contents.get(i) + ";";
		}
		return sReturn;
	};
}
