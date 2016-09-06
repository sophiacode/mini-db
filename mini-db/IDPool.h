#ifndef _ID_POOL_H_
#define _ID_POOL_H_
#include <deque>
#include "global.h"
using namespace std;
class IDPool
{
  deque<int*>blocklist;
  deque<int>nodelist;
public:
  int NewNode();
  void deleteNode(int p);
  void release();
  IDPool();
  ~IDPool();
};

#endif