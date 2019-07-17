import java.util.ArrayList;

import org.jsoup.Jsoup;


public class Crawler extends Thread {
	
	private ArrayList<String> downloadedDocs;
	
	public Crawler(ArrayList<String> downloadedDocs) {
		this.downloadedDocs = downloadedDocs;
	}

	private static String downloadLink(String link) {
	      try {
	          String document = Jsoup.connect(link).get().html();
	          return document;
	      } catch (Throwable e) {
	          e.printStackTrace();
	          return null;
	      }
	  }

}
