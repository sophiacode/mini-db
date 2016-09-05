#ifndef _MEM_POOL_H_
#define _MEM_POOL_H_
#include <deque>
#include "BPlusTreeNode.h"
#include <atomic>
using namespace std;
template<class KEYTYPE>
class MemPool
{
  deque<BPlusTreeNode<KEYTYPE>*>blocklist;
  deque<BPlusTreeNode<KEYTYPE>*>nodelist;
public:
  BPlusTreeNode<KEYTYPE>* NewNode();
  void deleteNode(BPlusTreeNode<KEYTYPE> *p);
  void release();
  MemPool();
  ~MemPool();
};

#endif
