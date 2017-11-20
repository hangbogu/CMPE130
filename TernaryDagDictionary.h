#ifndef _TERNARY_DAG_DICTIONARY_H
#define _TERNARY_DAG_DICTIONARY_H

#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include "TernaryDagNode.h"

class TernaryDagDictionary {
  private:
    TernaryDagNodePtr root;
    string longestWord;
    int longestWordLength;
    int numOfWords;
    int numOfNodes;
  
  public:
    TernaryDagDictionary(TernaryDagNodePtr root = NULL, string longestWord = "", int longestWordLength = 0, int numOfWords = 0, int numOfNodes = 0);
    
    void downloadDictionary();
    void dictionaryDimensions(TernaryDagDictionary &trieDictionary);
    
    void insert(const string &word);
    TernaryDagNodePtr addAWord(TernaryDagNodePtr root, const string &word, int index);
    
    void balance();
    unsigned setCount(TernaryDagNodePtr root);
    void balanceTernaryTrieDictionary(TernaryDagNodePtr root);
    TernaryDagNodePtr balanceLevel(TernaryDagNodePtr root);
    TernaryDagNodePtr divide(TernaryDagNodePtr root, int dividingCount);
    TernaryDagNodePtr rotateRight(TernaryDagNodePtr root);
    TernaryDagNodePtr rotateLeft(TernaryDagNodePtr root);
    void balancedDictionaryDimensions();
    
    void compressTernaryTrieDictionary();
    unsigned computeHash(TernaryDagNodePtr root);
    void removeDuplicateSuffixes(TernaryDagNodePtr root);
    bool checkAndRemoveDuplicateSuffixes(TernaryDagNodePtr *root);
    bool isEqual(TernaryDagNodePtr oldNode, TernaryDagNodePtr newNode);
    void freeNode(TernaryDagNodePtr root);
    //void freeHashTable();
    
    void print();
    void printLexicographically(TernaryDagNodePtr root, char* buffer, int depth);
    
    void testPrint();
};

#endif