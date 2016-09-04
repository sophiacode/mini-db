#include "MemPool.h"



template<class KEYTYPE>
void MemPool<KEYTYPE>::release()
{
  nodelist.clear();
  while (!blocklist.empty())
  {
    delete[] blocklist.front();
    blocklist.pop_front();
  }
}

template<class KEYTYPE>
void MemPool<KEYTYPE>::deleteNode(BPlusTreeNode<KEYTYPE> *p)
{
  nodelist.push_back(p);
}

template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>* MemPool<KEYTYPE>::NewNode()
{
  BPlusTreeNode *p;
  if (nodelist.empty()){
    blocklist.push_back(p = new BPlusTreeNode<KEYTYPE>[1000]);
    for (int i = 0; i < 1000; i++, p++){
      nodelist.push_back(p);
    }
  }
  p = nodelist.front();
  nodelist.pop_front();
  new(p)BPlusTreeNode<KEYTYPE>();
  return p;
}
