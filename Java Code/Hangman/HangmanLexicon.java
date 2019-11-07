/*
 * File: HangmanLexicon.java
 * -------------------------
 * This file contains a stub implementation of the HangmanLexicon
 * class that you will reimplement for Part III of the assignment.
 */

import acm.util.*;
import java.util.*;
import java.io.*;

public class HangmanLexicon {

	public HangmanLexicon() {
		readLexicon();
		getWordCount();
	}

	/** Reads each line (word) in the lexicon file, adding each to the ArrayList of words. */
	public void readLexicon() {
		try {
			BufferedReader rd = new BufferedReader(new FileReader("HangmanLexicon.txt"));
			while(true) {
				String line = rd.readLine();
				if(line == null) break;
				strList.add(line);
			}
			rd.close();
		} catch(IOException ex) {
				throw new ErrorException(ex);
			}
		}

	/** Returns the number of words in the lexicon. */
	public int getWordCount() {
		return strList.size();
	}

	/** Returns the word at the specified index. */
	public String getWord() {
		RandomGenerator rgen = RandomGenerator.getInstance();
		int index = rgen.nextInt(0, getWordCount()-1);
		return strList.get(index);

	 /* switch (index) {
			case 0: return "BUOY";
			case 1: return "COMPUTER";
			case 2: return "CONNOISSEUR";
			case 3: return "DEHYDRATE";
			case 4: return "FUZZY";
			case 5: return "HUBBUB";
			case 6: return "KEYHOLE";
			case 7: return "QUAGMIRE";
			case 8: return "SLITHER";
			case 9: return "ZIRCON";
			default: throw new ErrorException("getWord: Illegal index");
		} */
	}
	
	/** ArrayList of the lexicon file */
	private ArrayList<String> strList = new ArrayList<String>();
}
