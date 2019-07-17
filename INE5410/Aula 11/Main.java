import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import java.util.concurrent.LinkedBlockingQueue;

import java.util.ArrayList;
import java.util.List;


public class Main {

     public static void main(String[] args) {
       LinkedBlockingQueue toVisit = new LinkedBlockingQueue();
       ArrayList<String> downloadedDocs = new ArrayList<String>();
       
       Crawler crawlers[] = new Crawler[20];
       Parser parsers[] = new Parser[2];
       
       for(int i = 0; i < 20; i++) {
         crawlers[i] = new Crawler(downloadedDocs);
         crawlers[i].start();
       }
       
       for(int i = 0; i < 2; i++) {
         parsers[i] = new Parser(toVisit);
         parsers[i].start();
       }
       
     }

  }