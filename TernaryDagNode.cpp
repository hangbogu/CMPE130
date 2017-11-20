#include "TernaryDagNode.h"

TernaryDagNodePtr newTernaryDagNode(char sym) {
  TernaryDagNodePtr currentNode = new TernaryDagNode;
  currentNode->symbol = sym;
  currentNode->isAWord = false;
  currentNode->leftChild = NULL;
  currentNode->middleChild = NULL;
  currentNode->rightChild = NULL;
  return currentNode;
};