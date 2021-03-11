#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include <clocale>
using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	deconstructTrie();
}

bool StudentSpellCheck::load(string dictionaryFile) {
	std::locale::global(std::locale("")); // stolen shamelessly from StackOverflow, lets it not crash processing weird chars
	deconstructTrie(); // shouldn't really be necessary, but will prevent weird memory leaks
	ifstream file(dictionaryFile);
	if (!file)
		return false;
	string word;
	while (getline(file, word))
		addWord(word);
	return true;
}

bool StudentSpellCheck::spellCheck(string word, int max_suggestions, vector<string>& suggestions) {
	if (isDictionaryWord(word))
		return true;
	suggestions.clear();
	vector<string> allSuggestions;
	findSuggestions(word, allSuggestions); // O(L^2)
	for (int i = 0; i < max_suggestions && i < allSuggestions.size(); i++) // O(max_suggestions)
		suggestions.push_back(allSuggestions[i]);
	return false;
}

void StudentSpellCheck::spellCheckLine(const string& line, vector<SpellCheck::Position>& problems) {
	problems.clear();
	bool currentlyInTheMiddleOfAWord = false;
	Position wordPosition;
	string word = "";
	for (int i = 0; i < line.size(); i++)
	{
		if (currentlyInTheMiddleOfAWord)
		{
			if (isalpha(line[i]) || line[i] == '\'')
			{	// i is in the middle of a word
				wordPosition.end++;
				word.push_back(line[i]);
			}
			else
			{	// i is right after the end of the word
				if (!isDictionaryWord(word))
					problems.push_back(wordPosition);
				currentlyInTheMiddleOfAWord = false;
				word = "";
			}
		}
		else
		{
			if (isalpha(line[i]) || line[i] == '\'')
			{	// i is first char of a word
				wordPosition.start = i;
				wordPosition.end = i;
				word.push_back(line[i]);
				currentlyInTheMiddleOfAWord = true;
			}
		}
	}
	if (currentlyInTheMiddleOfAWord && !isDictionaryWord(word))
		problems.push_back(wordPosition);
}

void StudentSpellCheck::addWord(string word)
{
	TrieNode* node = &root;
	for (char c : word)
	{
		c = tolower(c);
		if (!isalpha(c) && c != '\'') // discard non-letters and non-'s
			continue;
		bool found = false;
		for (TrieNode* next : node->branches)
		{
			if (next->ch == c)
			{	// this branch of the trie already exists, no need to make new one
				found = true;
				node = next;
				break;
			}
		}
		if (!found)
		{	// no branch of the trie matches the char c, make new branch
			TrieNode* newNode = new TrieNode(c);
			node->branches.push_back(newNode);
			node = newNode;
		}
	}
	if (node->ch == '~') // no characters left after discarding invalid ones, don't add new word
		return;
	for (TrieNode* branch : node->branches)
		if (branch->ch == '~') // word is already in dictionary trie
			return;
	node->branches.push_back(new TrieNode('~')); // add '~' to signify end of word
}

void StudentSpellCheck::deconstructTrie()
{
	for (TrieNode* branch : root.branches)
		deconstructTrie(branch);
	root.branches.clear();
}

void StudentSpellCheck::deconstructTrie(TrieNode* n)
{
	for (TrieNode* branch : n->branches)
		deconstructTrie(branch);
	delete n;
}

bool StudentSpellCheck::isDictionaryWord(string word)
{
	word += '~'; // valid words must precede '~' in the trie
	TrieNode* node = &root;
	for (char c : word)
	{
		c = tolower(c);
		bool found = false;
		for (TrieNode* branch : node->branches)
		{
			if (c == branch->ch)
			{
				found = true;
				node = branch;
				break;
			}
		}
		if (!found) // no branches from this node match the current char in word
			return false;
	}
	// all chars in word appear in order in the trie, followed by '~'
	return true;
}

void StudentSpellCheck::findSuggestions(string word, vector<string>& suggestions)
{
	TrieNode* node = &root;
	for (int i = 0; i < word.size(); i++) // O(L)
	{
		char c = tolower(word.at(i));
		TrieNode* next = nullptr;
		for (TrieNode* branch : node->branches)
		{
			if (c == branch->ch)
				next = branch;
			else
			{
				string potentialSuggestion = word.substr(0, i) + branch->ch + word.substr(i + 1);
				if (isDictionaryWord(potentialSuggestion)) // O(L)
					suggestions.push_back(potentialSuggestion);
			}
		}
		if (next == nullptr) // no branches from this node match the current char in word
			return;
		node = next;
	}
	// all chars in word appear in order in the trie
	for (TrieNode* branch : node->branches)
		if (branch->ch == '~') // this should never happen, but I guess a word can be its own suggestion
			suggestions.push_back(word);
}
