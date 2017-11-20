#ifndef _TERNARY_DAG_NODE_H
#define _TERNARY_DAG_NODE_H

#include <iostream>

using namespace std;

struct TernaryDagNode;

typedef TernaryDagNode* TernaryDagNodePtr;

struct TernaryDagNode {
    char symbol;
    bool isAWord;
    unsigned count;
    unsigned hash;
    unsigned nodeNumber;
    TernaryDagNodePtr leftChild;
    TernaryDagNodePtr middleChild;
    TernaryDagNodePtr rightChild;
};

TernaryDagNodePtr newTernaryDagNode(char sym);

#endif