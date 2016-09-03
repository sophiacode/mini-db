#ifndef _B_PLUS_TREE_NODE_H_
#define _B_PLUS_TREE_NODE_H_
#include <bits/stdc++.h>
using namespace std;
const int BPlusTree_m = 3;
template<class KEYTYPE>
class BPlusTreeNode
{
private:
  bool is_leaf_;//判断是否是叶子节点
  int key_num_;//当前关键字个数 <= m-1;
  string this_file_;//当前节点文件地址
  BPlusTreeNode *father_;//父亲节点指针;
  string father_file_;//父亲节点文件地址;
  BPlusTreeNode *sonptr_[BPlusTree_m + 1];//子树指针;+1为了先添加后分裂，下同;
  string son_file_[BPlusTree_m + 1];//子节点文件地址;
  KEYTYPE key_[BPlusTree_m + 1];//关键字;
  int key_data_id[BPlusTree_m + 1];//关键字在数据文件中的id：

  /**
  *   \数组中插入关键字,关键字，关键字对于id
  *
  *   \用于叶子节点
  */
  void ArrayInsert(KEYTYPE _key, int _data_id);
  /**
  *   \【重载】数组中插入关键字,关键字,子指针
  *
  *   \用于非叶子节点
  */
  void ArrayInsert(KEYTYPE _key, BPlusTreeNode<KEYTYPE> *_p);
public:
  /**
  *   \是否为叶子节点
  */
  bool isleaf(){return is_leaf_}

  /**
  *   \二分查找
  *
  *   \返回正数为插入位置，负数表示已经存在，命中位置的相反数
  *
  *   \位置从1开始
  */
  int BinarySearch(KEYTYPE _key);

  BPlusTreeNode();
  ~BPlusTreeNode();
};


#endif