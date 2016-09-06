#include "IDPool.h"


int IDPool::NewNode()
{
  int p[1000];
  int _p;
  if (nodelist.empty()){
    for (int i = 0; i < 1000; i++){
      p[i] = i + 1000 * blocklist.size();
      nodelist.push_back(p[i]);
    }
    blocklist.push_back(p);
  }
  _p = nodelist.front();
  nodelist.pop_front();
  return _p;
}

void IDPool::deleteNode(int p)
{
  nodelist.push_back(p);
}

void IDPool::release()
{
  nodelist.clear();
  while (!blocklist.empty())
  {
    delete[] blocklist.front();
    blocklist.pop_front();
  }
}

IDPool::IDPool()
{
}


IDPool::~IDPool()
{
}
