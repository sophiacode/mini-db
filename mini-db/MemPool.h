#ifndef _MEM_POOL_H_
#define _MEM_POOL_H_
#include <deque>
#include <vector>;

#include "global.h"
#include "BPlusTreeNode.h"

using namespace std;
template<class KEYTYPE>
class MemPool
{
  deque<BPlusTreeNode<KEYTYPE>*>blocklist;
  deque<BPlusTreeNode<KEYTYPE>*>nodelist;
public:\
  vector<BPlusTreeNode<KEYTYPE>*>cachelist;
  BPlusTreeNode<KEYTYPE>* NewNode(std::string path);
  void deleteNode(BPlusTreeNode<KEYTYPE> *p);
  void release();
  MemPool() { ; }
  ~MemPool() { ; }
};



//*************************************************************
//
//      以下定义
//
//*************************************************************


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
BPlusTreeNode<KEYTYPE>* MemPool<KEYTYPE>::NewNode(std::string path)
{
  BPlusTreeNode<KEYTYPE> *p;
  if (nodelist.empty()){
    blocklist.push_back(p = new BPlusTreeNode<KEYTYPE>[1000]);
    for (int i = 0; i < 1000; i++, p++){
      nodelist.push_back(p);
    }
  }
  p = nodelist.front();
  nodelist.pop_front();
  new(p)BPlusTreeNode<KEYTYPE>(path);
  cachelist.push_back(p);
  return p;
}


#endif
