#ifndef _ID_POOL_H_
#define _ID_POOL_H_
#include <deque>
#include <fstream>
#include <iostream>
#include "global.h"
using namespace std;
const int IDSIZE = 1000;
class IDPool
{
  int blocklist;//重复记录的文件位置偏移量
  int nodelist[IDSIZE];//重复记录
  int nodelist_p;//重复的站顶指针;
  int nowid;//当前id
  ofstream *out_file_stream_;//写入文件流
  ifstream *in_file_stream_;//读入文件流
  char _path_[500];//索引的路径
public:
  int NewNode();
  void deleteNode(int p);
  void release();
  void savePool();
  IDPool();
  IDPool(string sPath);
  ~IDPool();
};

#endif