#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_
#include "BPlusTreeNode.h"
#include "MemPool.h"
#include <fstream>
//#include <bits/stdc++.h>
using namespace std;
template<class KEYTYPE>
class BPlusTree
{
private:
  MemPool<KEYTYPE> Pool;//内存池
  bool is_primare_key_;//是否主键（关系到是否重复）
  BPlusTreeNode<KEYTYPE> *root_;//根节点
  fstream rootfs_;//根节点文件流
  BPlusTreeNode<KEYTYPE> *mem_ptr_[BPlusTree_m];//需要常驻内存的节点指针
  fstream mem_fs_;//需要常驻内存的节点文件流
  fstream file_stream_;//随机文件流
  int cache_num_;//暂时存入内存的节点数量

  /**
  *   \将文件读入节点
  */
  //void ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \将节点写入文件
  */
  //void WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \返回儿子的指针
  *
  *   \有现成指针则返回，否则返回读取文件后new的指针
  */
  BPlusTreeNode<KEYTYPE>* SonPtr(BPlusTreeNode<KEYTYPE> *_p, int _insert_index);

  /**
  *   \返回父亲节点
  *
  *   \有现成指针则返回，否则返回读取文件后new的指针
  */
  BPlusTreeNode<KEYTYPE> *FatherPtr(BPlusTreeNode<KEYTYPE> *_p);

  public:
  BPlusTree();
  ~BPlusTree();
  /**
  *   \是否为主键
  */
  bool isPrimareKey(){ return is_primare_key_ };

  /**
  *   \插入关键字
  *
  *   \接口：键值，该键值对应于文件中的id
  */
  void InsertNode(KEYTYPE _key, int _data_id);

  
  /**
  *   \删除关键字
  *
  *   \
  */
  void DeleteNode(KEYTYPE _key);

  /**
  *   \查找关键字的叶子节点
  */
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);

  /**
  *   \查找关键字的叶子节点
  */
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);

};





#endif