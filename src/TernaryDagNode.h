#ifndef _TERNARY_DAG_NODE_H
#define _TERNARY_DAG_NODE_H

struct TernaryDagNode;

typedef TernaryDagNode* TernaryDagNodePtr;

#define MAX_WORD_LENGTH 50

struct TernaryDagNode {
    char symbol;
    bool isAWord;
    unsigned count;
    unsigned hash;
    TernaryDagNodePtr leftChild;
    TernaryDagNodePtr middleChild;
    TernaryDagNodePtr rightChild;
};

#endif