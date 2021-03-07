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
	: TextEditor(undo), cursorX(), cursorY(), document(), currLine() {
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
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
		document.push_back(line);
	}
	currLine = document.begin();
	return true;
}

bool StudentTextEditor::save(string file) {
	return false;  // TODO
}

void StudentTextEditor::reset() {
	document.clear(); // O(N) since this is a linked list
	document.push_front("");
	currLine = document.begin();
	cursorX = 0;
	cursorY = 0;
	getUndo()->clear();
}

void StudentTextEditor::move(Dir dir) {
	switch (dir)
	{
	case Dir::UP:
		if (currLine != document.begin())
		{
			cursorY--;
			currLine--;
			if (cursorX > currLine->size())
				cursorX = currLine->size();
		}
		break;
	case Dir::DOWN:
		if (currLine != --document.end())
		{
			cursorY++;
			currLine++;
			if (cursorX > currLine->size())
				cursorX = currLine->size();
		}
		break;
	case Dir::LEFT:
		if (cursorX == 0)
		{
			if (currLine != document.begin())
			{
				currLine--;
				cursorY--;
				cursorX = currLine->size();
			}
		}
		else
		{
			cursorX--;
		}
		break;
	case Dir::RIGHT:
		if (cursorX == currLine->size())
		{
			if (currLine != --document.end())
			{
				currLine++;
				cursorY++;
				cursorX = 0;
			}
		}
		else
		{
			cursorX++;
		}
		break;
	case Dir::HOME:
		currLine = document.begin();
		cursorX = 0;
		cursorY = 0;
		break;
	case Dir::END:
		currLine = --document.end();
		cursorX = currLine->size();
		cursorY = document.size()-1;
		break;
	}
}

void StudentTextEditor::del() {
	if (cursorX < currLine->size())
	{
		string& line = *currLine;
		char ch = line[cursorX];
		line = line.substr(0, cursorX) + line.substr(cursorX+1);
		getUndo()->submit(Undo::Action::DELETE, cursorY, cursorX, ch);
	}
}

void StudentTextEditor::backspace() {
	// TODO
}

void StudentTextEditor::insert(char ch) {
	if (ch != '\t')
	{
		string& line = *currLine;
		string start = line.substr(0, cursorX);
		string end = line.substr(cursorX);
		line = start + ch + end;
		cursorX++;
		getUndo()->submit(Undo::INSERT, cursorY, cursorX, ch);
	}
	else
	{
		for (int i = 0; i < 4; i++)
			insert(' ');
	}
}

void StudentTextEditor::enter() {
	// TODO
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = cursorY;
	col = cursorX;
}

int StudentTextEditor::getLines(int startRow, int numRows, vector<string>& lines) const {
	if (startRow < 0 || numRows < 0 || startRow >= document.size())
		return -1;
	lines.clear();
	list<string>::const_iterator iter = currLine;
	for (int i = cursorY; i < startRow; i++)
		iter++;
	for (int i = cursorY; i > startRow; i--)
		iter--;

	for (int i = 0; i < numRows && startRow + i < document.size(); i++)
		lines.push_back(*(iter++));
	return lines.size();
}

void StudentTextEditor::undo() {
	// TODO
}
