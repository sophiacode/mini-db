#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_
#include "BPlusTreeNode.h"
#include "MemPool.h"
#include <bits/stdc++.h>
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
  BPlusTreeNode<KEYTYPE> *cache_ptr_[BPlusTree_m];//暂时存入内存的节点指针
  fstream cache_fs_;//暂时存入内存的节点文件流
  int cache_num_;//暂时存入内存的节点数量

  /**
  *   \将文件读入节点
  */
  void ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \将节点写入文件
  */
  void WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \释放缓存节点
  */
  void deleteCache();
  /**
  *   \返回儿子的指针
  *
  *   \如果节点没有读入内存，就读入，并返回指针
  */
  BPlusTreeNode<KEYTYPE>* SonPtr(BPlusTreeNode<KEYTYPE> *_p, int _insert_index);
  public:
  BPlusTree();
  ~BPlusTree();
  /**
  *   \是否为主键
  */
  bool isPrimareKey(){ return is_primare_key_ };


  void InsertNode(KEYTYPE _key, int _data_id);//插入关键字
  void DeleteNode(KEYTYPE _key);//删除关键字
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);



};





#endif