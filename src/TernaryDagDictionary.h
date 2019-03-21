#ifndef _TERNARY_DAG_DICTIONARY_H
#define _TERNARY_DAG_DICTIONARY_H

#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <algorithm>
#include "TernaryDagNode.h"

using namespace std;

class TernaryDagDictionary;

typedef TernaryDagDictionary* TernaryDagDictionaryPtr;

class TernaryDagDictionary {
  private:
    TernaryDagNodePtr root;
    string longestWord;
    unsigned longestWordLength;
    unsigned numOfWords;
    unsigned numOfNodes;
  
  public:
    TernaryDagDictionary(TernaryDagNodePtr root = NULL, string longestWord = "", unsigned longestWordLength = 0, unsigned numOfWords = 0, unsigned numOfNodes = 0);

    TernaryDagNodePtr getRootValue() const;

    void downloadDictionary();
    void dictionaryDimensions(TernaryDagDictionary &dictionary, TernaryDagDictionaryPtr &dictionaryPtr);
    
    void insert(const string &word);
    TernaryDagNodePtr newTernaryDagNode(char sym);
    TernaryDagNodePtr addAWord(TernaryDagNodePtr root, const string &word, int index);
    
    void balance();
    unsigned setCount(TernaryDagNodePtr root);
    void balanceTernaryTrieDictionary(TernaryDagNodePtr root);
    TernaryDagNodePtr balanceLevel(TernaryDagNodePtr root);
    TernaryDagNodePtr divide(TernaryDagNodePtr root, int dividingCount);
    TernaryDagNodePtr rotateRight(TernaryDagNodePtr root);
    TernaryDagNodePtr rotateLeft(TernaryDagNodePtr root);
    
    void compressTernaryTrieDictionary();
    unsigned computeHash(TernaryDagNodePtr root);
    void removeDuplicateSuffixes(TernaryDagNodePtr root);
    bool checkAndRemoveDuplicateSuffixes(TernaryDagNodePtr *root);
    bool isEqual(TernaryDagNodePtr oldNode, TernaryDagNodePtr newNode);
    void freeNode(TernaryDagNodePtr root);
    void freeHashTable();
    void compressedDictionaryDimensions();
    
    string correct(const string &word);
    void correct(const string &word, vector<string> candidates);
    void edits(const string &word, vector<string> &result);
    bool search(TernaryDagNodePtr root, const string &word, unsigned index);
    
    void print();
    void printLexicographically(TernaryDagNodePtr root, char* buffer, int depth);
    void writeToScreen();
};

#endif