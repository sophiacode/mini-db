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
  MemPool<KEYTYPE> Pool;//�ڴ��
  bool is_primare_key_;//�Ƿ���������ϵ���Ƿ��ظ���
  BPlusTreeNode<KEYTYPE> *root_;//���ڵ�
  fstream rootfs_;//���ڵ��ļ���
  BPlusTreeNode<KEYTYPE> *mem_ptr_[BPlusTree_m];//��Ҫ��פ�ڴ�Ľڵ�ָ��
  fstream mem_fs_;//��Ҫ��פ�ڴ�Ľڵ��ļ���
  fstream file_stream_;//����ļ���
  int cache_num_;//��ʱ�����ڴ�Ľڵ�����

  /**
  *   \���ļ�����ڵ�
  */
  //void ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \���ڵ�д���ļ�
  */
  //void WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \���ض��ӵ�ָ��
  *
  *   \���ֳ�ָ���򷵻أ����򷵻ض�ȡ�ļ���new��ָ��
  */
  BPlusTreeNode<KEYTYPE>* SonPtr(BPlusTreeNode<KEYTYPE> *_p, int _insert_index);

  /**
  *   \���ظ��׽ڵ�
  *
  *   \���ֳ�ָ���򷵻أ����򷵻ض�ȡ�ļ���new��ָ��
  */
  BPlusTreeNode<KEYTYPE> *FatherPtr(BPlusTreeNode<KEYTYPE> *_p);

  public:
  BPlusTree();
  ~BPlusTree();
  /**
  *   \�Ƿ�Ϊ����
  */
  bool isPrimareKey(){ return is_primare_key_ };

  /**
  *   \����ؼ���
  *
  *   \�ӿڣ���ֵ���ü�ֵ��Ӧ���ļ��е�id
  */
  bool InsertNode(KEYTYPE _key, int _data_id);

  
  /**
  *   \ɾ���ؼ���
  *
  *   \
  */
  bool DeleteNode(KEYTYPE _key);

  /**
  *   \���ҹؼ��ֵ�Ҷ�ӽڵ�
  */
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);

  /**
  *   \���ҹؼ��ֵ�id
  */
  int SearchID(KEYTYPE _key);

};


//***************************************************************************
//
//        ���¶���
//
//***************************************************************************




template<class KEYTYPE>
BPlusTree<KEYTYPE>::~BPlusTree()
{

}

template<class KEYTYPE>
BPlusTree<KEYTYPE>::BPlusTree()
{
  root_ = Pool.NewNode();
}





//template<class KEYTYPE>
//void BPlusTree<KEYTYPE>::ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p)
//{
//  if (_p->this_file_.empty()){
//    cerr << "û��ָ���ļ���" << endl;//���󾯸汣��
//    return;
//  }
//  if (cache_num_ >= BPlusTree_m){//�ͷŻ���ڵ�
//    deleteCache();
//  }
//  if (_p->cache_index != -1){
//    cache_fs_[_p->cache_index].read((char*)(_p), sizeof(*_p));
//  }
//  else{
//    _p->cache_index = cache_num_;
//    cache_ptr_[cache_num_] = _p;
//    cache_fs_[cache_num_].open(_p->this_file_, ios::in | ios::oct | ios::binary);
//    cache_fs_[cache_num_].read((char*)(_p), sizeof(*_p));
//    cache_num_++;
//  }
//}

//
//template<class KEYTYPE>
//void BPlusTree<KEYTYPE>::WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p)
//{
//  if (_p->this_file_.empty()){
//    cerr << "û��ָ���ļ���" << endl;//���󾯸汣��
//    return;
//  }
//  if (cache_num_ >= BPlusTree_m){//�ͷŻ���ڵ�
//    deleteCache();
//  }
//  if (_p->cache_index != -1){
//    cache_fs_[_p->cache_index].write((char*)(_p), sizeof(*_p));
//  }
//  else{
//    _p->cache_index = cache_num_;
//    cache_ptr_[cache_num_] = _p;
//    cache_fs_[cache_num_].open(_p->this_file_, ios::in | ios::oct | ios::binary);
//    cache_fs_[cache_num_].write((char*)(_p), sizeof(*_p));
//    cache_num_++;
//  }
//}




template<class KEYTYPE>
BPlusTreeNode<KEYTYPE> * BPlusTree<KEYTYPE>::FatherPtr(BPlusTreeNode<KEYTYPE> *_p)
{
  if (_p->father_ != nullptr){
    return _p->father_;
  }
  if (_p->father_file_.empty()){
    return nullptr;
  }
  else{
    BPlusTreeNode<KEYTYPE>* p = Pool.NewNode();
    file_stream_.clear();
    file_stream_.open(_p->father_file_, ios::in | ios::oct | ios::binary);
    file_stream_.read((char*)(p), sizeof(*p));
    return p;
  }
}

template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>* BPlusTree<KEYTYPE>::SonPtr(BPlusTreeNode<KEYTYPE> *_p, int _insert_index)
{
  if (_p->sonptr_[_insert_index] != nullptr){
    return _p->sonptr_[_insert_index];
  }
  if (_p->son_file_[_insert_index].empty()){
    return nullptr;
  }
  else{
    BPlusTreeNode<KEYTYPE>* p = Pool.NewNode();
    file_stream_.clear();
    file_stream_.open(_p->son_file_[_insert_index], ios::in | ios::oct | ios::binary);
    file_stream_.read((char*)(p), sizeof(*p));
    return p;
  }
}

template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::DeleteNode(KEYTYPE _key)
{
  bool borrow_flag;//��ڵ�ɹ����;
  int insert_index;
  bool flag_first = false;//�Ƿ�ɾ���˵�һ��λ��
  KEYTYPE next_first_key;//������һ���ĵ�һ��key;
  BPlusTreeNode<KEYTYPE> *p, *q, *r;
  p = SearchNode(_key);
  if (p->key_num_ == 0){//��Ϊ��
    //*************************
    //��Ϊ�գ���ֵ�����ڣ��� ���� ˵��
    //*************************
  }
  insert_index = -(p->BinarySearch(_key));
  if (insert_index > 0){
    if (insert_index == 1){
      flag_first == true;
      next_first_key = p->key_[1];
    }
    p->key_num_--;
    for (int i = insert_index - 1; i < (p->key_num_ - 1); i++){
      p->key_[i] = p->key_[i + 1];
      p->key_data_id[i] = p->key_data_id[i - 1];
    }
  }
  else{
    return false;
    //*************************
    //��ֵ�����ڵ� ���� ˵��
    //*************************
  }
  r = p;
  while (flag_first == true){//���µ�һ���ڵ�
    r = FatherPtr(r);
    if (r == nullptr){
      flag_first = false;
    }
    else{
      insert_index = -(r->BinarySearch(key));
      if (insert_index == 1){
        flag_first = true;
        next_first_key = r->key_[1];
      }
      else{
        flag_first = false;
      }
      r->key_[insert_index] = _key;
    }
  }
  while (true){
    if (p->key_num_ < MinBPlusTree_m){
      borrow_flag = false;
      r = FatherPtr(p);
      insert_index = -(r->BinarySearch(p->key_[0]));
      if (insert_index > 1){//����߽�
        borrow_flag = true;
        q = SonPtr(r, insert_index - 2);
        if (q->key_num_ > MinBPlusTree_m){
          for (int i = p->key_num_; i >= 1; i--){
            p->key_[i] = p->key_[i - 1];
            if (p->isleaf()){
              p->key_data_id[i] = p->key_data_id[i - 1];
            }
            else{
              p->sonptr_[i] = p->sonptr_[i - 1];
              p->son_file_[i] = p->son_file_[i - 1];
            }
          }
          q->key_num_--;
          p->key_[0] = q->key_[q->key_num_];
          if (p->isleaf()){
            p->key_data_id[0] = q->key_data_id[q->key_num_];
          }
          else{
            p->sonptr_[0] = q->sonptr_[q->key_num_];
            p->son_file_[0] = q->son_file_[q->key_num_];
          }
          r->key_[insert_index - 1] = p->key_[0];
          r->sonptr_[insert_index - 1] = p;
          r->son_file_[insert_index - 1] = p->this_file_;
        }
      }
      else if (insert_index < r->key_num_){//���ұ߽�
        borrow_flag = true;
        q = SonPtr(r, insert_index);
        if (q->key_num_>MinBPlusTree_m){
          p->key_[p->key_num_] = q->key_[0];
          q->key_num_--;
          for (int i = 0; i <q.key_num_; i--){
            q->key_[i] = q->key_[i + 1];
            if (p->isleaf()){
              q->key_data_id[i] = q->key_data_id[i + 1];
            }
            else{
              q->sonptr_[i] = q->sonptr_[i + 1];
              q->son_file_[i] = q->son_file_[i + 1];
            }
          }
          r->key_[insert_index] = q->key_[0];
          //Ӧ������
        }
      }
      else{//���߶��費��  orz   (ͳһ����ϲ�)
        q = SonPtr(r, insert_index - 2);
        for (int i = 0; i < p->key_num_; i++){
          q->key_[q->key_num_ + i] = p->key_[i];
          if (q->isleaf()){
            q->key_data_id[q->key_num_ + i] = p->key_data_id[i];
          }
          else{
            q->sonptr_[q->key_num_ + i] = p->sonptr_[i];
            q->son_file_[q->key_num_ + i] = p->son_file_[i];
          }
        }
        q
      }
    }
  }
  return true;
}

template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::InsertNode(KEYTYPE _key, int _data_id)
{
  int insert_index;
  bool flag_first = false;//�Ƿ���뵽��һ��λ��
  KEYTYPE last_first_key;//����֮ǰ�ĵ�һ��key;
  BPlusTreeNode<KEYTYPE> *p, *q, *r;
  p = SearchNode(_key);
  if (p->key_num_ == 0){//��һ�������������⴦��(����pӦ��==root_)
    p->key_[0] = _key;
    p->key_num_++;
  }
  else{
    insert_index = p->BinarySearch(_key);
    if (!isPrimareKey()){
      insert_index = abs(-insert_index);
    }
    if (insert_index > 0){
      if (insert_index == 1){
        flag_first == true;
        last_first_key = p->key_[0];
      }
      for (int i = p->key_num_; i >= insert_index; i--){
        p->key_[i] = p->key_[i - 1];
        p->key_data_id[i] = p->key_data_id[i - 1];
      }
      p->key_[insert_index - 1] = _key;
      p->key_data_id[insert_index - 1] = _data_id;
      p->key_num_++;
    }
    else{
      return false;
      //*************************
      //��ֵ�Ѿ����ڵ� ���� ˵��
      //*************************
    }
  }
  r = p;
  while (flag_first == true){//���µ�һ���ڵ�
    r = FatherPtr(r);
    if (r == nullptr){
      flag_first = false;
    }
    else{
      insert_index = -(r->BinarySearch(last_first_key));
      if (insert_index == 1){
        flag_first = true;
        last_first_key = r->key_[0];
      }
      else{
        flag_first = false;
      }
      r->key_[insert_index] = _key;
    }
  }

  while (p->key_num_ > BPlusTree_m){
    q = Pool.NewNode();//����һ���ֵ�
    r = FatherPtr(p);
    if (r == nullptr){//û�и��׾ͽ���һ������(�¸��ڵ�)
      r = Pool.NewNode();
      root_ = r;
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
      if (p->isleaf()){
        q->key_data_id[i] = p->key_data_id[i + p->key_num_];
      }
      else{
        q->sonptr_[i] = p->sonptr_[i + p->key_num_];
        q->son_file_[i] = p->son_file_[i + p->key_num_];
      }
    }
    if (p->isleaf()){
      p->brother_ = q;
      p->brother_file_ = q->this_file_;
      q->sister_ = p;
      q->sister_file_ = p->this_file_;
    }
    q->father_ = r;
    q->father_file_ = r->this_file_;

    insert_index = r->BinarySearch(q->key_[0]);//�ظ�����������
    if (!isPrimareKey()){
      insert_index = abs(-insert_index);
    }
    if (insert_index > 0){
      for (int i = r->key_num_; i >= insert_index; i--){
        r->key_[i] = r->key_[i - 1];
        r->sonptr_[i] = r->sonptr_[i - 1];
        r->son_file_[i] = r->son_file_[i - 1];
      }
      r->key_[insert_index - 1] = _key;
      r->sonptr_[insert_index - 1] = q;
      r->son_file_[insert_index - 1] = q->this_file_;
      r->key_num_++;
    }
    p = r;
  }
  return true;
}



template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>* BPlusTree<KEYTYPE>::SearchNode(KEYTYPE _key)
{
  int insert_index;
  BPlusTreeNode<KEYTYPE> *p;
  p = root_;
  while (!p->isleaf()){
    insert_index = p->BinarySearch(_key);
    if (insert_index > 0){
      if (insert_index == 1){
        p = SonPtr(p, insert_index - 1);
      }
      else{
        p = SonPtr(p, insert_index - 2);
      }
    }
    else if (insert_index < 0){
      p = SonPtr(p, (-insert_index) - 1);
    }
  }
  return p;
}



template<class KEYTYPE>
int BPlusTree<KEYTYPE>::SearchID(KEYTYPE _key)
{
  int insert_index;
  BPlusTreeNode<KEYTYPE> *p;
  p = SearchNode(_key);
  if (p = nullptr){
    return -1;//ERROR��û���ҵ�
  }
  insert_index = p->BinarySearch(_key);
  if (insert_index > 0){
    return -1;
  }
  return p->key_data_id[-insert_index];
}


#endif