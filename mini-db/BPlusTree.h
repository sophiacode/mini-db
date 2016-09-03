#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_
#include "BPlusTreeNode.h"
template<class KEYTYPE>
class BPlusTree
{
private:
  BPlusTreeNode<KEYTYPE> *root_;//根节点
  bool is_primare_key_;//是否主键（关系到是否重复）


  public:
  BPlusTree();
  ~BPlusTree();
  /**
  *   \是否为主键
  */
  bool isPrimareKey(){ return is_primare_key_ };


  void InsertNode(KEYTYPE _key, int _data_id);//插入关键字
  void deleteNode(KEYTYPE _key);//删除关键字




};





#endif