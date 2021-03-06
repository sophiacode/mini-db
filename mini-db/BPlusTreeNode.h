#ifndef _B_PLUS_TREE_NODE_H_
#define _B_PLUS_TREE_NODE_H_
#include <string>
#include <sstream>
#include <time.h>

#include "global.h"
//#include <bits/stdc++.h>
using namespace std;
const int BPlusTree_m = 3;
const int MinBPlusTree_m = (BPlusTree_m + 1) / 2;



template<class KEYTYPE>
class BPlusTreeNode
{
public:

  //friend class BPlusTree;

  bool is_leaf_;                                     //判断是否是叶子节点

  USER_INT key_num_;                                      //当前关键字个数 <= m-1;

  USER_INT this_file_;                                    //当前节点文件中位置;

  BPlusTreeNode *father_;                            //父亲节点指针;

  USER_INT father_file_;                                  //父亲节点文件中位置;

  BPlusTreeNode *brother_;                           //哥哥节点指针;

  USER_INT brother_file_;                                 //哥哥节点文件中位置;

  BPlusTreeNode *sister_;                            //妹妹节点指针;

  USER_INT sister_file_;                                  //妹妹节点文件中位置;

  BPlusTreeNode *sonptr_[BPlusTree_m + 1];           //子树指针;+1为了先添加后分裂，下同;

  USER_INT son_file_[BPlusTree_m + 1];                    //子节点文件中位置;

  KEYTYPE key_[BPlusTree_m + 1];                     //关键字;

  USER_INT key_data_id[BPlusTree_m + 1];                  //关键字在数据文件中的id;


  /**
  *   \数组中插入关键字,关键字，关键字对于id
  *
  *   \用于叶子节点，返回插入位置
  */
  int ArrayInsert(KEYTYPE _key, int _data_id);
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
	bool isleaf() { return is_leaf_; }

  /**
  *   \二分查找
  *
  *   \返回正数为插入位置，负数表示已经存在，命中位置的相反数
  *
  *   \位置从1开始
  */
  int BinarySearch(KEYTYPE _key);

  /**
  *   \二分查找定位在最后一个命中
  *
  *   \返回正数为插入位置，负数表示已经存在，命中位置的相反数
  *
  *   \位置从1开始
  */
  int BinarySearchBack(KEYTYPE _key);

  /**
  *   \构造函数
  */
  BPlusTreeNode();


  /**
  *   \析构函数
  */
  ~BPlusTreeNode();
};


//***************************************************************************
//
//        以下定义
//
//***************************************************************************




template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>::BPlusTreeNode()
{
  is_leaf_ = true;
  key_num_ = 0;
  father_ = nullptr;
  father_file_ = -1;
  brother_ = nullptr;
  brother_file_ = -1;
  sister_ = nullptr;
  sister_file_ = -1;
  for (int i = 0; i < BPlusTree_m + 1; i++){
    sonptr_[i] = nullptr;
    son_file_[i] = -1;
  }
  this_file_ = -1;
}


template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>::~BPlusTreeNode()
{

}



template<class KEYTYPE>
int BPlusTreeNode<KEYTYPE>::ArrayInsert(KEYTYPE _key, int _data_id)
{
  int insert_index;
  insert_index = this->BinarySearch(_key);
  if (!isPrimareKey()){
    insert_index = -insert_index;
  }
  if (insert_index > 0){
    for (int i = this->key_num_; i >= insert_index; i--){
      this->key_[i] = this->key_[i - 1];
      this->key_data_id[i] = this->key_data_id[i - 1];
    }
    this->key_[insert_index - 1] = _key;
    this->key_data_id[insert_index - 1] = _data_id;
    this->key_num_++;
  }
}


template<class KEYTYPE>
void BPlusTreeNode<KEYTYPE>::ArrayInsert(KEYTYPE _key, BPlusTreeNode<KEYTYPE> *_p)
{
  int insert_index;
  insert_index = this->BinarySearch(_key);
  if (!isPrimareKey()){
    insert_index = -insert_index;
  }
  if (insert_index > 0){
    for (int i = this->key_num_; i >= insert_index; i--){
      this->key_[i] = this->key_[i - 1];
      this->sonptr_[i] = this->sonptr_[i - 1];
      this->son_file_[i] = this->sonptr_[i - 1];
    }
    this->key_[insert_index - 1] = _key;
    this->sonptr_[insert_index - 1] = _p;
    this->son_file_[insert_index - 1] = _p->this_file_;
    this->key_num_++;
  }
}


template<class KEYTYPE>
int BPlusTreeNode<KEYTYPE>::BinarySearchBack(KEYTYPE _key)
{
  int op, ed, mid;
  op = 0;
  ed = this->key_num_ - 1;
  while (op <= ed){
    mid = (op + ed + 1) / 2;
    if (this->key_[mid] == _key){
      if (op <= ed){
        op = mid;
      }
      if (op >= ed){
        return -(mid + 1);//命中
      }
    }
    if (_key < this->key_[mid]){
      ed = mid - 1;
    }
    if (_key > this->key_[mid]){
      op = mid + 1;
    }
  }
  //未命中情况
  if (_key < this->key_[mid]){
    return mid + 1;
  }
  if (_key > this->key_[mid]){
    return mid + 2;
  }
}


template<class KEYTYPE>
int BPlusTreeNode<KEYTYPE>::BinarySearch(KEYTYPE _key)
{
  int op, ed, mid;
  op = 0;
  ed = this->key_num_ - 1;
  while (op <= ed){
    mid = (op + ed) / 2;
    if (this->key_[mid] == _key){
      if (op <= ed){
        ed = mid;
      }
      if (op >= ed){
        return -(mid + 1);//命中
      }
    }
    if (_key < this->key_[mid]){
      ed = mid - 1;
    }
    if (_key > this->key_[mid]){
      op = mid + 1;
    }
  }
  //未命中情况
  if (_key < this->key_[mid]){
    return mid + 1;
  }
  if (_key > this->key_[mid]){
    return mid + 2;
  }
}


#endif