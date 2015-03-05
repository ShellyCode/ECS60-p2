#include <iostream>
#include <climits>
#include <math.h>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

void LeafNode::addToLeft(int value, int last)
{
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::ToLeft()

void LeafNode::addToRight(int value, int last)
{
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

void LeafNode::borrowFromLeft(){
    int value=leftSibling->getMaximum();
    LeafNode *left=(LeafNode*)leftSibling;
    left->dropFromThis(value);
    addToThis(value);
    if (parent)
        parent->resetMinimum(this);
    
}

void LeafNode::borrowFromRight(){
    int value=rightSibling->getMinimum();;
    LeafNode *right=(LeafNode *)rightSibling;
    right->dropFromThis(value);
    addToThis(value);
//    cout<<"borrow right"<<value<<endl;
    if (parent)
        parent->resetMinimum(right);
}

void LeafNode::leftMerge(){
    
//    cout<<"value"<<value<<count<<endl;
    LeafNode *left=(LeafNode*)leftSibling;
    while(count>0){
        int value=getMinimum();
        dropFromThis(value);
        left->addToThis(value);
    }
    if(rightSibling){
        rightSibling->setLeftSibling(left);
    }
    leftSibling->setRightSibling(rightSibling);
}
void LeafNode::rightMerge(){
    
    LeafNode *right=(LeafNode*)rightSibling;
    while(count>0){
    int value=getMaximum();
    dropFromThis(value);
    right->addToThis(value);
    }
    if(leftSibling){
        leftSibling->setRightSibling(right);
    }
    rightSibling->setLeftSibling(leftSibling);
}


void LeafNode::addToThis(int value)
{
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis()
//*****************my function:*************************//
void LeafNode::dropFromThis(int value)
{
    int i,j;
    int min=getMinimum();
//  cout<<"before delete"<<count<<endl;
    for (i=0;i<=count-1&&values[i]!=value;i++);  //find the value
    if(i!=count){             //if the value is in the leaf then delete
        for(j=i;j<=count-1;j++)
            values[j]=values[j+1];
        values[j]=NULL;
        count--;
    }
//   cout<<"after delete"<<count<<endl;
//    else{
//        cout<<"i=count"<<i<<" "<<count<<endl;
//    }
//    cout<<"min:"<<values[0]<<"&"<<getMinimum()<<endl;
//    cout<<"value0"<<values[0]<<endl;
    if (value==min&&parent)
        parent->resetMinimum(this);
    
//    cout<<"delete"<<value<<endl;
}

void LeafNode::addValue(int value, int &last) //用来sort
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode:: addValue()


int LeafNode::getMaximum()const
{
  if(count > 0)  // should always be the case
    return values[count - 1];
  else
    return INT_MAX;
} // getMaximum()


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)
  {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)
  {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


LeafNode* LeafNode::remove(int value)
{// To be written by students
    
    dropFromThis(value);
    
    if (parent&&count< ceil(float(leafSize)/2)) {
//       cout<<"keep"<<endl;
        if (leftSibling){   //has left
//            cout<<"lsi"<<leftSibling->getCount()<<endl;
            if(leftSibling->getCount()>ceil(float(leafSize)/2) ){
//                cout<<"borrowleft"<<endl;
                //cout<<ceil(leafSize/2)<<"borrow"<<ceil(float(leafSize)/2)<<endl;
                borrowFromLeft();
                return NULL;
            }
            else {
                
//            if(count>0){
   //            cout<<"leftmerge"<<endl;
                leftMerge();
//            }
                
                return this;
                }
        }
        else if(rightSibling){   //does not have left but have right
//            cout<<"rsi"<<rightSibling->getCount()<<endl;
            if (rightSibling->getCount()>ceil(float(leafSize)/2) ){
//                cout<<"borrowright"<<endl;
                //cout<<ceil(leafSize/2)<<"borrow"<<ceil(float(leafSize)/2)<<endl;
                borrowFromRight();
                return NULL;
            }
            else{
//                if (count>0) {
  //                 cout<<"rightmerge"<<endl;
                    rightMerge();
//                }
                return this;
            }
        }
        
    }

    return NULL;  // filler for stub
}  // LeafNode::remove()



LeafNode* LeafNode::split(int value, int last)
{
  LeafNode *ptr = new LeafNode(leafSize, parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(ptr);

  rightSibling = ptr;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
    ptr->values[ptr->count++] = values[i];

  ptr->values[ptr->count++] = last;
  count = (leafSize + 1) / 2;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
  return ptr;
} // LeafNode::split()

