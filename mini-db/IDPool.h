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
  int blocklist;//�ظ���¼���ļ�λ��ƫ����
  int nodelist[IDSIZE];//�ظ���¼
  int nodelist_p;//�ظ���վ��ָ��;
  int nowid;//��ǰid
  ofstream *out_file_stream_;//д���ļ���
  ifstream *in_file_stream_;//�����ļ���
  char _path_[500];//������·��
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