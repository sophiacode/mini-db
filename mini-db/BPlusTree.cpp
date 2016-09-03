#include "BPlusTree.h"


template<class KEYTYPE>
BPlusTree<KEYTYPE>::~BPlusTree()
{

}

template<class KEYTYPE>
BPlusTree<KEYTYPE>::BPlusTree()
{
  root_ = new BPlusTreeNode();
}


template<class KEYTYPE>
void BPlusTree<KEYTYPE>::deleteNode(KEYTYPE _key)
{

}




template<class KEYTYPE>
void BPlusTree<KEYTYPE>::InsertNode(KEYTYPE _key, int _data_id)
{
  int insert_index;
  BPlusTreeNode<KEYTYPE> *p, *q, *r;
  p = root_;
  if (p->isleaf()){
    if (p->key_num_ == 0){
      p->key_[0] = _key;
      p->key_num_++;
    }
    else{
      p->ArrayInsert(_key, _data_id);
      else{
        //*************************
        //已经存在的错误说明
        //*************************
      }
    }
    //分裂节点
    if (p->key_num_ > BPlusTree_m){
      q = new BPlusTreeNode();
      r = p->father_;
      if (r == nullptr){//建立一个父亲节点
        r = new BPlusTreeNode();
        p->father_ = r;
        p->father_file_ = r->this_file_;
        r->is_leaf_ = false;
        r->key_[0] = p->key_[0];
        r->key_num_++;
        r->sonptr_[0] = p;
        r->son_file_[0] = p->this_file_;
      }
      p->key_num_ = q->key_num_ = p->key_num_ / 2;
      for (int i = 0; i < q.key_num_; i++){
        q->key_[i] = p->key_[i + p->key_num_];
        q->key_data_id[i] = p->key_data_id[i + p->key_num_];
      }
      q->father_ = r;
      q->father_file_ = r->this_file_;
      r->ArrayInsert(q->key_[0]);
    }
  }
  else{//不是叶子节点
    insert_index=p->BinarySearch(_key);

  }

}