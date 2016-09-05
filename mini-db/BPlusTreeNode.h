#ifndef _B_PLUS_TREE_NODE_H_
#define _B_PLUS_TREE_NODE_H_
#include <string>
//#include <bits/stdc++.h>
using namespace std;
const int BPlusTree_m = 3;
const int MinBPlusTree_m = (BPlusTree_m + 1) / 2;
extern int name_num_;//������ˮ��
template<class KEYTYPE>
class BPlusTreeNode
{
private:
  bool is_leaf_;//�ж��Ƿ���Ҷ�ӽڵ�
  int key_num_;//��ǰ�ؼ��ָ��� <= m-1;
  string this_file_;//��ǰ�ڵ��ļ���ַ
  BPlusTreeNode *father_;//���׽ڵ�ָ��;
  string father_file_;//���׽ڵ��ļ���ַ;
  BPlusTreeNode *brother_;//���ڵ�ָ��;
  string brother_file_;//���ڵ��ļ���ַ;
  BPlusTreeNode *sister_;//���ýڵ�ָ��;
  string sister_file_;//���ýڵ��ļ���ַ;
  BPlusTreeNode *sonptr_[BPlusTree_m + 1];//����ָ��;+1Ϊ������Ӻ���ѣ���ͬ;
  string son_file_[BPlusTree_m + 1];//�ӽڵ��ļ���ַ;
  KEYTYPE key_[BPlusTree_m + 1];//�ؼ���;
  int key_data_id[BPlusTree_m + 1];//�ؼ����������ļ��е�id;

  /**
  *   \�����в���ؼ���,�ؼ��֣��ؼ��ֶ���id
  *
  *   \����Ҷ�ӽڵ㣬���ز���λ��
  */
  int ArrayInsert(KEYTYPE _key, int _data_id);
  /**
  *   \�����ء������в���ؼ���,�ؼ���,��ָ��
  *
  *   \���ڷ�Ҷ�ӽڵ�
  */
  void ArrayInsert(KEYTYPE _key, BPlusTreeNode<KEYTYPE> *_p);
public:
  /**
  *   \�Ƿ�ΪҶ�ӽڵ�
  */
  bool isleaf(){return is_leaf_}

  /**
  *   \���ֲ���
  *
  *   \��������Ϊ����λ�ã�������ʾ�Ѿ����ڣ�����λ�õ��෴��
  *
  *   \λ�ô�1��ʼ
  */
  int BinarySearch(KEYTYPE _key);

  BPlusTreeNode();
  ~BPlusTreeNode();
};


//***************************************************************************
//
//        ���¶���
//
//***************************************************************************



template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>::~BPlusTreeNode()
{

}

template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>::BPlusTreeNode()
{
  int timeflag;
  stringstream ss;
  is_leaf_ = true;
  key_num_ = 0;
  father_ = nullptr;
  brother_ = nullptr;
  for (int i = 0; i < BPlusTree_m + 1; i++){
    sonptr_ = nullptr;
  }
  timeflag = time(0) * 1000;
  name_num_ &= 0x1ff;
  timeflag += ++name_num_;
  ss << timeflag;
  ss >> this_file_;
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
void BPlusTreeNode<KEYTYPE>::ArrayInsert(KEYTYPE _key��BPlusTreeNode<KEYTYPE> *_p)
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
      return -(mid + 1);//����
    }
    if (_key < this->key_[mid]){
      ed = mid - 1;
    }
    if (_key > this->key_[mid]){
      op = mid + 1;
    }
  }
  //δ�������
  if (_key < this->key_[mid]){
    return mid + 1;
  }
  if (_key > this->key_[mid]){
    return mid + 2;
  }
}


#endif