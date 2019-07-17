import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

public class Parser extends Thread {
	
	private LinkedBlockingQueue toVisit;
	
	public Parser(LinkedBlockingQueue toVisit) {
		this.toVisit = toVisit;
	}

	private List<String> extractDocumentLinks(String html) {
	    ArrayList<String> result = new ArrayList<String>();
	    Document doc = Jsoup.parse(html);
	    Elements links = doc.select("a[href]");
	    for (Element link : links) {
	      String url = link.attr("abs:href");
	      if (url.length() > 0 && url.startsWith("http"))
	        result.add(url);
	    }
	    return result;
	  }
}
