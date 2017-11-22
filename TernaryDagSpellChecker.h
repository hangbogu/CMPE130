#ifndef _TERNARY_DAG_SPELL_CHECKER_H
#define _TERNARY_DAG_SPELL_CHECKER_H

struct TernaryDagFileNode;

typedef TernaryDagFileNode* TernaryDagFileNodePtr;
#define DAG_DICTIONARY_SIGNATURE "alpha dictionary"
#define MAX_WORD_LENGTH 50

struct TernaryDagFileNode {
	char _symbol;
	char _padding;
	unsigned short _leftChild;
	unsigned short _middleChild;
	unsigned short _rightChild;
};

//Open Ternary DAG Dictionary, returns NULL in case of error
TernaryDagFileNodePtr openTernaryDagDictionary();

//Returns true if the word is an exact match in the dictionary; false, otherwise
bool isExactMatch(TernaryDagFileNodePtr dictionary, const char* word);

//Returns true if the word is in the dictionary; false, otherwise
bool findExactMatch(TernaryDagFileNodePtr node, const char* word, TernaryDagFileNodePtr dictionary);

//Enumerate spelling suggestions (fuzzy matches)
void enumerateSpellingSuggestions(TernaryDagFileNodePtr dictionary, const char * word, void (* enumproc)(const char*, void*), void * param);

//Close the dictionary
void closeTernaryDagDictionary(TernaryDagFileNodePtr dictionary);

TernaryDagFileNodePtr getLeftChild(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary);

TernaryDagFileNodePtr getRightChild(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary);

TernaryDagFileNodePtr getMiddleChild(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary);

bool isNullLink(TernaryDagFileNodePtr node, TernaryDagFileNodePtr dictionary);

#endif