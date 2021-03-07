#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
	: TextEditor(undo), cursorX(), cursorY(), lines(), currLine() {
	reset();
}

StudentTextEditor::~StudentTextEditor()
{
	reset();
}

bool StudentTextEditor::load(string file) {
	ifstream input(file);
	if (!input)
		return false;
	reset();
	string line;
	while (getline(input, line))
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line = line.substr(0, line.size() - 1);
		lines.push_back(line);
	}
	currLine = lines.begin();
	return true;
}

bool StudentTextEditor::save(string file) {
	return false;  // TODO
}

void StudentTextEditor::reset() {
	lines.clear(); // O(N) since lines is a linked list
	lines.push_front("");
	currLine = lines.begin();
	cursorX = 0;
	cursorY = 0;
}

void StudentTextEditor::move(Dir dir) {
	// TODO
}

void StudentTextEditor::del() {
	// TODO
}

void StudentTextEditor::backspace() {
	// TODO
}

void StudentTextEditor::insert(char ch) {
	// TODO
}

void StudentTextEditor::enter() {
	// TODO
}

void StudentTextEditor::getPos(int& row, int& col) const {
	// TODO
}

void StudentTextEditor::getLines(int startRow, int numRows, vector<string>& lines) const {
	// TODO
}

void StudentTextEditor::undo() {
	// TODO
}
