package MasterGrid;

import java.io.FileNotFoundException;
import java.lang.SecurityException;
import java.util.Formatter;

public class FileWriter {
	Formatter dataBase;

	public FileWriter (String target) {
		try
        {
            dataBase = new Formatter(target); // open the file
        } // end try
        catch ( SecurityException securityException )
        {
            System.err.println(
                    "You do not have write access to this file.");
            System.exit( 1 ); // terminate the program
        } // end catch
        catch ( FileNotFoundException fileNotFoundException )
        {
            System.err.println("Error opening or creating file.");
            System.exit( 1 ); // terminate the program
        } // end catch
	}

	public void write (String text) {
		dataBase.format("%s", text);
	}

	public void close () {
		dataBase.close();
	}
}