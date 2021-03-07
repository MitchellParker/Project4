#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

#include <stack>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct Edit {
		Edit(Action a, int r, int c, char ch) : action(a), row(r), col(c), text(1, ch) { }
		Action action;
		int row;
		int col;
		std::string text;
	};

	// Stores the most recent edits in order, so that they may later be undone
	std::stack<Edit> edits;
};

#endif // STUDENTUNDO_H_
