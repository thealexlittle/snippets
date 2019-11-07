/*
 * File: NameSurferGraph.java
 * ---------------------------
 * This class represents the canvas on which the graph of
 * names is drawn. This class is responsible for updating
 * (redrawing) the graphs whenever the list of entries changes
 * or the window is resized.
 */

import acm.graphics.*;
import java.awt.event.*;
import java.util.*;
import java.awt.*;

public class NameSurferGraph extends GCanvas
	implements NameSurferConstants, ComponentListener {

	/**
	* Creates a new NameSurferGraph object that displays the data.
	*/
	public NameSurferGraph() {
		addComponentListener(this);
		nameEntries = new ArrayList<NameSurferEntry>();
	}
	
	
	/**
	* Clears the list of name surfer entries stored inside this class.
	*/
	public void clear() {
		nameEntries.removeAll(nameEntries);
	}
	
	
	/* Method: addEntry(entry) */
	/**
	* Adds a new NameSurferEntry to the list of entries on the display.
	* Note that this method does not actually draw the graph, but
	* simply stores the entry; the graph is drawn by calling update.
	*/
	public void addEntry(NameSurferEntry entry) {
		nameEntries.add(entry);
	}
	
	
	/**
	* Updates the display image by deleting all the graphical objects
	* from the canvas and then reassembling the display according to
	* the list of entries. Your application must call update after
	* calling either clear or addEntry; update is also called whenever
	* the size of the canvas changes.
	*/
	public void update() {
		removeAll();
		double[] xCoords = new double[NDECADES];
		drawGrid(xCoords);
		addEntriesToGraph(xCoords);
	}
	
	/*
	 * Adds the lines and labels that make up the grid
	 * Also stores each vertical line's x-coordinate in an array for reference later
	 */
	private void drawGrid(double[] xCoords) {
		double gap = getWidth()/NDECADES;
		double x2 = getWidth();
		double y2 = getHeight();
		for (int i = 0; i < NDECADES; i++) {
			double x = gap*i;
			xCoords[i] = x;
			add(new GLine(x, xCoords[0], x, y2));
			String decade = Integer.toString(START_DECADE + i*10);
			add(new GLabel(decade, x + LABEL_SPACE, y2 - LABEL_SPACE));
		}
		add(new GLine(xCoords[0], GRAPH_MARGIN_SIZE, x2, GRAPH_MARGIN_SIZE));
		add(new GLine(xCoords[0], y2 - GRAPH_MARGIN_SIZE, x2, y2 - GRAPH_MARGIN_SIZE));
	}
	
	/*
	 * Graphs each entry stored in the nameEntries arrayList to the grid
	 */
	private void addEntriesToGraph(double[] xCoords) {
		if(nameEntries.size() > 0) {
			for (int i = 0; i < nameEntries.size(); i++) {
				String name = nameEntries.get(i).getName();
				graphEntry(xCoords, name, i);
			}
		}
	}
	
	/*
	 * Adds labels for entries displaying their name and rank for each decade in the appropriate location
	 * Adds lines connecting the ranks from decade to decade
	 * The x-coordinates depend on the grid lines, making use of the passed-in array
	 * The y-coordinates depend on the rank in each decade
	 */
	private void graphEntry(double[] xCoords, String name, int nameEntry) {
		int nextRank = 0;
		int currentRank = 0;
		Color entryColor = pickColor(nameEntry);
		for (int i = 0; i < NDECADES - 1; i++) {
			currentRank = nameEntries.get(nameEntry).getRank(i);
			nextRank = nameEntries.get(nameEntry).getRank(i + 1);
			double y1 = calcYCoord(currentRank);
			double y2 = calcYCoord(nextRank);
			GLine rankLine = new GLine(xCoords[i], y1, xCoords[i + 1], y2);
			rankLine.setColor(entryColor);
			add(rankLine);
			GLabel nameLabel = makeLabel(currentRank, name);
			nameLabel.setColor(entryColor);
			add(nameLabel, xCoords[i] + LABEL_SPACE, y1 - LABEL_SPACE);
		}
		GLabel lastLabel = makeLabel(nextRank, name);
		lastLabel.setColor(entryColor);
		add(lastLabel, xCoords[NDECADES - 1] + LABEL_SPACE, calcYCoord(nextRank) - LABEL_SPACE);
	}
	
	/*
	 * Creates the appropriate label for each entry on the graph
	 * Gives name and rank, or name and "*" indicating that the name 
	 * did not rank in the top 1000 names for that decade
	 */
	private GLabel makeLabel(int rank, String name) {
		if (rank != 0) {
			return new GLabel(name + " " + rank);
		}
		return new GLabel(name + "*");
	}
	
	/*
	 * Calculates the y-coordinate for graphing lines and adding labels
	 * Depends on the rank of the name for each decade
	 */
	private double calcYCoord(int rank) {
		double yCoord = getHeight() - GRAPH_MARGIN_SIZE;
		if(rank != 0) {
			return yCoord = (getHeight() - 2.0*GRAPH_MARGIN_SIZE)/(MAX_RANK - 1.0)*rank + GRAPH_MARGIN_SIZE;
		}
		return yCoord;
	}
	
	/* Cycles through the colors to set for each entry added to the graph */
	private Color pickColor(int nameEntry) {
		int colorIndex = (nameEntry % NCOLORS);
		switch(colorIndex) {
			case 0: return Color.BLACK;
			case 1: return Color.RED;
			case 2: return Color.BLUE;
			case 3: return Color.MAGENTA;
			default: return Color.BLACK;
		}
	}
	
	/* Implementation of the ComponentListener interface */
	public void componentHidden(ComponentEvent e) { }
	public void componentMoved(ComponentEvent e) { }
	public void componentResized(ComponentEvent e) { update(); }
	public void componentShown(ComponentEvent e) { }
	
	/* Stores all of the entries for the names the user types in */
	private ArrayList<NameSurferEntry> nameEntries;
}
