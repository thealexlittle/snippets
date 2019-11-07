/*
 * File: NameSurfer.java
 * ---------------------
 * When it is finished, this program will implements the viewer for
 * the baby-name database described in the assignment handout.
 */

import acm.program.*;
import acmx.classfile.JavaField;

import java.awt.event.*;
import javax.swing.*;

public class NameSurfer extends Program implements NameSurferConstants {

/* Method: init() */
/**
 * This method has the responsibility for reading in the data base
 * and initializing the interactors at the top of the window.
 */
	public void init() {
	   JLabel name = new JLabel("Name ");
	   JButton graphButton = new JButton("Graph");
	   JButton clearButton = new JButton("Clear");
	   graph = new NameSurferGraph();
	   tf = new JTextField(TEXT_FIELD_LENGTH);
	   data = new NameSurferDataBase(NAMES_DATA_FILE);
	   add(name, NORTH);
	   add(tf, NORTH);
	   add(graphButton, NORTH);
	   add(clearButton, NORTH);
	   add(graph, CENTER);
	   tf.addActionListener(this);
	   tf.setActionCommand("Graph");
	   addActionListeners();
	}

/* Method: actionPerformed(e) */
/**
 * This class is responsible for detecting when the buttons are
 * clicked, so you will have to define a method to respond to
 * button actions.
 */
	public void actionPerformed(ActionEvent e) {
		String cmd = e.getActionCommand();
		if (cmd.equals("Graph")) {
			String name = tf.getText();
			if (isWord(name)) {
				name = toUpperCamelCase(name);
				NameSurferEntry entry = data.findEntry(name);
				if (entry != null) graph.addEntry(entry);
				graph.update();
			}
		} else {
			graph.clear();
			graph.update();
		}
	}
	
	/*
	 * Converts the string of letter characters the user typed to match the structure of names in the data file:
	 * Capital first-letter followed by the remaining lower-case letters
	 */
	private String toUpperCamelCase (String name) {
		String result = "";
		name = name.toLowerCase();
		result += Character.toUpperCase(name.charAt(0));
		for (int i = 1; i < name.length(); i++) {
			result += name.charAt(i);
		}
		return result;
	}
	
	/* Checks if the user input is a string of only letters */
	private boolean isWord(String name) {
		if (name.length() == 0) return false;
		for (int i = 0; i < name.length(); i++) {
			if (!Character.isLetter(name.charAt(i))) return false;
		}
		return true;
	}
	
	/* Creates the text field interactor */
	private JTextField tf;

	/* Instances of the NameSurferDataBase and NameSurferGraph */
	private NameSurferDataBase data;
	private NameSurferGraph graph;
}
