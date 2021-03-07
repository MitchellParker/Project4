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
	void getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
	int cursorX;
	int cursorY;
	std::list<std::string> lines;
	std::list<std::string>::iterator currLine;
};

#endif // STUDENTTEXTEDITOR_H_