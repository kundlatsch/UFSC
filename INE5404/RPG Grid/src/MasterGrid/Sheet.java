package MasterGrid;

public interface Sheet {
	
	public void processSheets (String content);
	public String toString();
	public void addSheet(String title, String content);
	public void editSheet(int index, String titles, String content);
	public void removeSheet(int index);
	public String getDefaultSheet();
	public String[] getTitles();
	String getTitle(int index);
	String getContent(int index);
}
