package MasterGrid;

import java.lang.SecurityException;
import java.util.*;
import java.io.*;

public class GenericFileReader {
	Formatter dataBase;
	private Scanner input;
	private String target;
	
	public GenericFileReader (String s)
	   {
		  this.target = s;
	      try
	      {
	         input = new Scanner( new File( s ) );
	      } // end try
	      catch ( FileNotFoundException fileNotFoundException )
	      {
	         System.err.println( "Error opening file." );
	         System.exit( 1 );
	      } // end catch
	   } // end method openFile

	public String readFile() throws IOException{
		
		    BufferedReader br = new BufferedReader(new FileReader(target));
		    try {
		        StringBuilder sb = new StringBuilder();
		        String line = br.readLine();

		        while (line != null) {
		            sb.append(line);
		            sb.append("\n");
		            line = br.readLine();
		        }
		        return sb.toString();
		       
		    } finally {
		        br.close();
		    }
		}

	public void close () {
		input.close();
	}

}