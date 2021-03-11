#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct TrieNode {
		TrieNode(char val = '~') : ch(val) { }
		char ch;
		std::vector<TrieNode*> branches;
	};

	// The root of the dictionary trie. The root node always has a value of '~'
	// valid words have a '~' appended to the end to distinguish them from being
	// just a substring of a valid word
	// ex) "perfect" is a dictionary word, so it is in the trie, and the string
	//     "perf" knows it is invalid because there is no '~' after the 'f'
	TrieNode root;
	
	// Adds a single dictionary word to the trie. The word should include nothing
	// but letters (case insensitive) and apostrophes. If word includes other symbols,
	// they are discarded, and the remaining letters and 's are added
	void addWord(std::string word);

	// Deletes all the branches of the dictonary trie, leaving only the root
	void deconstructTrie();
	// Recursively deletes a branch of the trie, only meant to be called by the 0-parameter overload
	void deconstructTrie(TrieNode* n);

	// Returns true if word (case insensitive) is in the dictionary trie, false otherwise.
	// Any non-letter, non-' symbols in word will cause this to return false
	bool isDictionaryWord(std::string word);

	void findSuggestions(std::string word, std::vector<std::string>& suggestions);
	//std::vector<std::string> findSuggestions(std::string word, std::string suggestion, TrieNode* node, int errors);
};

#endif  // STUDENTSPELLCHECK_H_
