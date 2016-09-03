#include "BPlusTreeNode.h"


template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>::~BPlusTreeNode()
{

}

template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>::BPlusTreeNode()
{
  is_leaf_ = true;
  key_num_ = 0;
  father_ = nullptr;
  for (int i = 0; i < BPlusTree_m + 1; i++){
    sonptr_ = nullptr;
  }
}




template<class KEYTYPE>
void BPlusTreeNode<KEYTYPE>::ArrayInsert(KEYTYPE _key, int _data_id)
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
void BPlusTreeNode<KEYTYPE>::ArrayInsert(KEYTYPE _key，BPlusTreeNode<KEYTYPE> *_p)
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
int BPlusTreeNode<KEYTYPE>::BinarySearch(KEYTYPE _key)
{
  int op, ed, mid;
  op = 0;
  ed = this->key_num_ - 1;
  while (op <= ed){
    mid = (op + ed) / 2;
    if (this->key_[mid] == _key){
      return -(mid+1);//命中
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