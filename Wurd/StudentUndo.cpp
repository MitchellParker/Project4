#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	if (edits.empty())
	{
		edits.push(Edit(action, row, col, ch));
		return;
	}
	Edit& prev = edits.top();
	if (action == DELETE && prev.action == DELETE && prev.row == row)
	{	// delete operations at the same row might be batched
		if (prev.col == col) // delete key was pressed
		{
			prev.text.push_back(ch);
			return;
		}
		else if (prev.col == col + 1) // backspace key was pressed
		{
			prev.text = ch + prev.text;
			prev.col = col;
			return;
		}
	}	// delete on same row but not same place in row --> don't batch

	else if (action == INSERT && prev.action == INSERT && prev.row == row && prev.col == col - 1)
	{	// 2 insertions one after another --> get batched
		prev.text.push_back(ch);
		prev.col = col;
		return;
	}
	// Default: just make a new edit
	edits.push(Edit(action, row, col, ch));
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
    return Action::ERROR;  // TODO
}

void StudentUndo::clear() {
	// TODO
}
