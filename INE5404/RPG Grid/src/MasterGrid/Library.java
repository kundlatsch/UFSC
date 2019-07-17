package MasterGrid;

import java.util.ArrayList;

public class Library {
	private ArrayList<String> titles = new ArrayList<String>();
	private ArrayList<String> bodys = new ArrayList<String>();
	
	public Library () {
		super ();
	}
	
	public void addText (String title, String body) {
		titles.add(title);
		bodys.add(body);
	}
	
	public void removeText (int index) {
		titles.remove(index);
		bodys.remove(index);
	}
	
	public void editText (int index, String title, String body) {
		titles.set(index, title);
		bodys.set(index, body);
	}
	
	public String toString () {
		String sReturn = "";
		for (int i = 0; i < titles.size(); i++) {
			sReturn += titles.get(i) + ";" + bodys.get(i) + ";";
		}
		return sReturn;
	}
	
	public String[] getTitles () {
		String[] arrayReturn = new String[titles.size()];
		arrayReturn = titles.toArray(arrayReturn);	
		return arrayReturn;
	}
	
	public String getBody (int i) {
		
		return bodys.get(i);
	}

	public String getTitle(int selectedIndex) {
		return titles.get(selectedIndex);
	}
	
}
