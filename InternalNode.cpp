#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
  for(int i = 1; i < count; i++)
  { // inserting before the end
    if(value < keys[i])
    { // if value is less than key, insert to the left
      BTreeNode* newNode = children[i - 1]->insert(value);
      setKeys(); // update keys

      if(newNode) // if new child is created
        return newChild(newNode);
      return NULL;
    }
  }
  // inserting at the last child
  BTreeNode* newNode = children[count - 1]->insert(value);
  setKeys(); // update keys
  // new child
  if(newNode)
    return newChild(newNode); // return NULL or new sibling
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  // set parent
  oldRoot->setParent(this);
  node2->setParent(this);

  // set children
  children[0] = oldRoot;
  children[1] = node2;
  count = 2; // only 2 nodes here
  setKeys(); // update keys
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  children[count++] = newNode;
  quick_sort(0, count - 1); // sort does sort and setkeys
} // InternalNode::insert()


InternalNode* InternalNode::newChild(BTreeNode* newNode)
{ //simply inserting
  if(count < internalSize)
    children[count++] = newNode;
  // look left first
  else if (leftSibling && leftSibling->getCount() < internalSize)
    minToLeft(newNode);
  // else look right
  else if (rightSibling && rightSibling->getCount() < internalSize)
    maxToRight(newNode);

  else
  {
    split(newNode);

    return (InternalNode*)rightSibling;
  } // else split

  quick_sort(0, count - 1);
  return NULL;
}


void InternalNode::maxToRight(BTreeNode* newNode)
{
  BTreeNode* max = newNode;

  if(max->getMinimum() < children[count - 1]->getMinimum())
  {
    max = children[count - 1];
    children[count - 1] = newNode;
    quick_sort(0, count - 1); // keep sorted
  } // max swap
  // max always goes to the right
  ((InternalNode*)rightSibling)->insert(max);
}


void InternalNode::minToLeft(BTreeNode* newNode)
{
  ((InternalNode*)leftSibling)->insert(children[0]); // min always goes to the left
  children[0] = newNode; // this may not be sorted
  quick_sort(0, count - 1); // need to keep sorted
}


void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


void InternalNode::setKeys()
{
  for(int i = 0; i < count; i++)
  {
    keys[i] = children[i]->getMinimum();
    children[i]->setParent(this);
  }
} // setkeys and handle parent*


void InternalNode::bubble_sort()
{
  bool swapped = true;
  //int j = 0;

  //while(swapped)
  for(int j = 1; swapped; j++)
  {
    swapped = false;
    //j++;

    for(int i = 0; i < count - j; i++)
    {
      if(children[i]->getMinimum() > children[i + 1]->getMinimum())
      {
        BTreeNode* tmp = children[i];
        children[i] = children[i + 1];
        children[i + 1] = tmp;
        swapped = true;
      }
    }
  }

  setKeys(); // update keys
} // bubble sort

void InternalNode::quick_sort(int left, int right)
{
  int i = left, j = right, pivot = children[(left + right) / 2]->getMinimum();

  while(i <= j)
  {
    while(children[i]->getMinimum() < pivot)
      i++;
    while(children[j]->getMinimum() > pivot)
      j--;
    
    if(i <= j)
    {
      BTreeNode* tmp = children[i];
      children[i++] = children[j];
      children[j--] = tmp;
    }
  }

  if(left < j)
    quick_sort(left, j);
  if(i < right)
    quick_sort(i, right);

  setKeys();
} // quick sort


void InternalNode::split(BTreeNode* newNode)
{
  rightSibling = new InternalNode(internalSize, leafSize, parent, this, rightSibling);

  if(rightSibling->getRightSibling()) // handle linking between sibling
    rightSibling->getRightSibling()->setLeftSibling(rightSibling);

  maxToRight(newNode); // pass max to the right, original node will be sorted

  for(int i = internalSize - internalSize/2; i < internalSize; i++, count--)
    ((InternalNode*)rightSibling)->insert(children[i]); // transfer nodes to the right

  setKeys(); // update keys
}
