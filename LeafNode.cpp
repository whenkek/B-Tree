#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{ //simply inserting
  if(count < leafSize)
    values[count++] = value;
  // count does not change in left and right cases
  else if(leftSibling && leftSibling->getCount() < leafSize)
    minToLeft(value); // min to the left

  else if(rightSibling && rightSibling->getCount() < leafSize)
    maxToRight(value); //pass max to the right,

  else
  {
    split(value);

    return (LeafNode*)rightSibling; // return new node splited
  } // split
  // "simply inserting" and minToLeft() do not sort. Need to sort
  sort(); // keep sorted
  return NULL;
}  // LeafNode::insert()


void LeafNode::minToLeft(int value)
{
  ((LeafNode*)leftSibling)->insert(values[0]);
  values[0] = value;
} //pass to the left


void LeafNode::maxToRight(int value)
{
  int max = value;

  if(values[count - 1] > max)
  {
      max = values[count - 1];
      values[count - 1] = value;
      sort();
  } // max swap

  ((LeafNode*)rightSibling)->insert(max);
}


void LeafNode::sort()
{
  bool swapped = true;
  int j = 0, tmp;
  while (swapped)
  {
    swapped = false;
    j++;
    for (int i = 0; i < count - j; i++)
    {
      if(values[i] > values[i + 1])
      {
        tmp = values[i];
        values[i] = values[i + 1];
        values[i + 1] = tmp;
        swapped = true;
      }
    }
  }
} // LeafNode::sort()


void LeafNode::split(int value)
{
  rightSibling = new LeafNode(leafSize, parent, this, rightSibling);
    // handle linking
  if(rightSibling->getRightSibling())
    rightSibling->getRightSibling()->setLeftSibling(rightSibling);

  maxToRight(value); // max to the right
  // transfer values to new node
  for(int i = leafSize - leafSize/2; i < leafSize; i++, count--)
    rightSibling->insert(values[i]);
}


void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


