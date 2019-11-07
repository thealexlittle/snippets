/*
 * File: NameSurferEntry.java
 * --------------------------
 * This class represents a single entry in the database.  Each
 * NameSurferEntry contains a name and a list giving the popularity
 * of that name for each decade stretching back to 1900.
 */

import acm.util.*;
import java.util.*;

public class NameSurferEntry implements NameSurferConstants {

/* Constructor: NameSurferEntry(line) */
/**
 * Creates a new NameSurferEntry from a data line as it appears
 * in the data file.  Each line begins with the name, which is
 * followed by integers giving the rank of that name for each
 * decade.
 */
	public NameSurferEntry(String line) {
		int nameStart = 0;
		int nameEnd = line.indexOf(" ");
		name = line.substring(nameStart, nameEnd);
		int rankStart = nameEnd + 1;
		int rankEnd = line.indexOf(" ", rankStart);
		String rank = "";
		for (int i = 0; i < NDECADES; i++) {
			if (i == NDECADES - 1) {
				rank = line.substring(rankStart);
				nameRanks[i] = Integer.parseInt(rank);
			} else {
				rank = line.substring(rankStart, rankEnd);
				nameRanks[i] = Integer.parseInt(rank);
				rankStart += rank.length() + 1;
				rankEnd = line.indexOf(" ", rankStart);
			}
		}
	}

/* Method: getName() */
/**
 * Returns the name associated with this entry.
 */
	public String getName() {
		return name;
	}

/* Method: getRank(decade) */
/**
 * Returns the rank associated with an entry for a particular
 * decade.  The decade value is an integer indicating how many
 * decades have passed since the first year in the database,
 * which is given by the constant START_DECADE.  If a name does
 * not appear in a decade, the rank value is 0.
 */
	public int getRank(int decade) {
		return nameRanks[decade];
	}

/* Method: toString() */
/**
 * Returns a string that makes it easy to see the value of a
 * NameSurferEntry.
 */
	public String toString() {
		String ranks = "";
		for (int i = 0; i < NDECADES; i++) {
			if (i == NDECADES - 1) {
				ranks += nameRanks[i];
			} else {
				ranks += (nameRanks[i] + " ");
			}
		}
		return name + " [" + ranks + "]";
	}
	
	// The name that is retrieved from each line of the file
	private String name;
	
	// Stores each name's ranks for each decade as integers
	private int[] nameRanks = new int[NDECADES];
}

