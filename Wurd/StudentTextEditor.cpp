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
	: TextEditor(undo), cursorCol(), cursorRow(), document(), currLine() {
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
	document.clear(); //reset() leaves one empty line that needs to go
	string line;
	while (getline(input, line))
	{
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
		document.push_back(line);
	}
	currLine = document.begin();
	return true;
}

bool StudentTextEditor::save(string file) {
	ofstream output(file);
	if (!output)
		return false;
	for (const string& line : document)
		output << line << endl;
	return true;
}

void StudentTextEditor::reset() {
	document.clear(); // O(N) since this is a linked list
	document.push_front("");
	currLine = document.begin();
	cursorCol = 0;
	cursorRow = 0;
	getUndo()->clear();
}

void StudentTextEditor::move(Dir dir) {
	switch (dir)
	{
	case Dir::UP:
		if (currLine != document.begin())
		{
			cursorRow--;
			currLine--;
			if (cursorCol > currLine->size())
				cursorCol = currLine->size();
		}
		break;
	case Dir::DOWN:
		if (currLine != --document.end())
		{
			cursorRow++;
			currLine++;
			if (cursorCol > currLine->size())
				cursorCol = currLine->size();
		}
		break;
	case Dir::LEFT:
		if (cursorCol == 0)
		{
			if (currLine != document.begin())
			{
				currLine--;
				cursorRow--;
				cursorCol = currLine->size();
			}
		}
		else
		{
			cursorCol--;
		}
		break;
	case Dir::RIGHT:
		if (cursorCol == currLine->size())
		{
			if (currLine != --document.end())
			{
				currLine++;
				cursorRow++;
				cursorCol = 0;
			}
		}
		else
		{
			cursorCol++;
		}
		break;
	case Dir::HOME:
		cursorCol = 0;
		break;
	case Dir::END:
		cursorCol = currLine->size();
		break;
	}
}

void StudentTextEditor::del() {
	if (cursorCol == currLine->size())
	{
		if (cursorRow == document.size() - 1)
			return;
		join(cursorRow);
	}
	else
	{
		del(cursorRow, cursorCol);
	}
}

void StudentTextEditor::backspace() {
	if (cursorCol == 0 && cursorRow == 0)
		return;
	move(Dir::LEFT);
	del();
}

void StudentTextEditor::insert(char ch) {
	if (ch == '\t')
	{
		for (int i = 0; i < 4; i++)
			insert(' ');
		return;
	}
	insert(cursorRow, cursorCol, ch);
}

void StudentTextEditor::enter() {
	split(cursorRow, cursorCol);
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = cursorRow;
	col = cursorCol;
}

int StudentTextEditor::getLines(int startRow, int numRows, vector<string>& lines) const {
	if (startRow < 0 || numRows < 0 || startRow >= document.size())
		return -1;
	lines.clear();
	list<string>::const_iterator iter = currLine;
	for (int i = cursorRow; i < startRow; i++)
		iter++;
	for (int i = cursorRow; i > startRow; i--)
		iter--;

	for (int i = 0; i < numRows && startRow + i < document.size(); i++)
		lines.push_back(*(iter++));
	return lines.size();
}

void StudentTextEditor::undo() {
	// TODO
}

void StudentTextEditor::setPos(int row, int col)
{
	setPos(row);
	cursorCol = col;
}

void StudentTextEditor::setPos(int row)
{
	if (row != cursorRow)
	{
		for (cursorRow; cursorRow < row; cursorRow++)
			currLine++;
		for (cursorRow; cursorRow > row; cursorRow--)
			currLine--;
	}
	cursorCol = currLine->size();
}

void StudentTextEditor::insert(int row, int col, char ch, bool addToUndoStack)
{
	setPos(row, col);
	string& line = *currLine;
	string start = line.substr(0, cursorCol);
	string end = line.substr(cursorCol);
	line = start + ch + end;
	cursorCol++;
	if (addToUndoStack)
		getUndo()->submit(Undo::Action::INSERT, cursorRow, cursorCol, ch);
}

void StudentTextEditor::del(int row, int col, bool addToUndoStack)
{
	setPos(row, col);
	string& line = *currLine;
	string start = line.substr(0, cursorCol);
	char deleted = line.at(cursorCol);
	string end = line.substr(cursorCol+1);
	line = start + end;
	if (addToUndoStack)
		getUndo()->submit(Undo::Action::DELETE, cursorRow, cursorCol, deleted);
}

void StudentTextEditor::join(int row, int col, bool addToUndoStack)
{
	setPos(row, col);
	string& line1 = *currLine;
	string line2 = *(++currLine);
	line1 += line2;
	currLine = document.erase(currLine);
	currLine--;
	if (addToUndoStack)
		getUndo()->submit(Undo::Action::JOIN, cursorRow, cursorCol);
}

void StudentTextEditor::split(int row, int col, bool addToUndoStack)
{
	setPos(row, col);
	if (addToUndoStack)
		getUndo()->submit(Undo::Action::SPLIT, cursorRow, cursorCol);
	string& line = *currLine;
	string start = line.substr(0, cursorCol);
	string end = line.substr(cursorCol);
	line = start;
	currLine = document.insert(++currLine, end);
	cursorRow++;
	cursorCol = 0;
}
