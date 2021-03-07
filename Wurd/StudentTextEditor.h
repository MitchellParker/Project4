#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"

#include <string>
#include <list>

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
	int cursorCol;
	int cursorRow;
	std::list<std::string> document;
	std::list<std::string>::iterator currLine;

	// Sets the cursor and currLine to the specified row and col (Expensive! use sparingly)
	// If row or col is out of bounds, undefined behavior
	void setPos(int row, int col);
	// Sets the row as normal and col to end of line
	void setPos(int row);

	// Inserts toBeInserted at position (row, col) Afterwards, the cursor
	// goes 1 position after the last inserted character
	// Set addToUndoStack to false if the action should not be added to undo stack
	void insert(int row, int col, char ch, bool addToUndoStack = true);

	// Removes numToBeDeleted chars from the document. Afterwards, the cursor
	// goes 1 position after the last deleted character.
	// Set addToUndoStack to false if the action should not be added to undo stack
	void del(int row, int col, bool addToUndoStack = true);

	// Combine the specified row with the row immediately after. Afterwards,
	// the cursor goes to the first position after the join.
	// Set addToUndoStack to false if the action should not be added to undo stack
	void join(int row, int col, bool addToUndoStack = true);

	// Split the specified row at the specified column into two rows.
	// Afterwards, the cursor goes to the beginning of the line after the split
	// Set addToUndoStack to false if the action should not be added to undo stack
	void split(int row, int col, bool addToUndoStack = true);
};

#endif // STUDENTTEXTEDITOR_H_
