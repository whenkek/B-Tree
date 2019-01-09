#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  InternalNode* newChild(BTreeNode *newNode);
  void maxToRight(BTreeNode* newNode);
  void minToLeft(BTreeNode* newNode);
  void print(Queue <BTreeNode*> &queue);
  void setKeys();
  void bubble_sort();
  void quick_sort(int left, int right);
  void split(BTreeNode* newNode);
}; // InternalNode class

#endif
