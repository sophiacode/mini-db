#include "BPlusTree.h"

template<class KEYTYPE>
BPlusTree<KEYTYPE>::~BPlusTree()
{

}

template<class KEYTYPE>
BPlusTree<KEYTYPE>::BPlusTree()
{
  root_ = Pool.NewNode();
}


template<class KEYTYPE>
void BPlusTree<KEYTYPE>::DeleteNode(KEYTYPE _key)
{

}


template<class KEYTYPE>
void BPlusTree<KEYTYPE>::ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p)
{
  if (_p->this_file_.empty()){
    cerr << "没有指定文件名" << endl;//错误警告保护
    return;
  }
  if (cache_num_ >= BPlusTree_m){//释放缓存节点
    deleteCache();
  }
  if (_p->cache_index != -1){
    cache_fs_[_p->cache_index].read((char*)(_p), sizeof(*_p));
  }
  else{
    cache_ptr_[cache_num_] = _p;
    cache_fs_[cache_num_].open(_p->this_file_, ios::in | ios::oct | ios::binary);
    cache_fs_[cache_num_].read((char*)(_p), sizeof(*_p));
    cache_num_++;
  }
}


template<class KEYTYPE>
void BPlusTree<KEYTYPE>::WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p)
{
  if (_p->this_file_.empty()){
    cerr << "没有指定文件名" << endl;//错误警告保护
    return;
  }
  if (cache_num_ >= BPlusTree_m){//释放缓存节点
    deleteCache();
  }
  if (_p->cache_index != -1){
    cache_fs_[_p->cache_index].write((char*)(_p), sizeof(*_p));
  }
  else{
    cache_ptr_[cache_num_] = _p;
    cache_fs_[cache_num_].open(_p->this_file_, ios::in | ios::oct | ios::binary);
    cache_fs_[cache_num_].write((char*)(_p), sizeof(*_p));
    cache_num_++;
  }
}

template<class KEYTYPE>
void BPlusTree<KEYTYPE>::deleteCache()
{
  cache_ptr_[0]->cache_index = -1;
  Pool.deleteNode(cache_ptr_[0]);
  cache_ptr_[0] = cache_ptr_[cache_num_ - 1];
  cache_fs_[0] = cache_fs_[cache_num_ - 1];
  for (int i = 1; i < cache_num_ - 1; i++){
    cache_ptr_[i]->cache_ptr_ = -1;
    Pool.deleteNode(cache_ptr_[i]);
    cache_fs_[i].clear();
  }
  cache_num_ = 1;
}

template<class KEYTYPE>
BPlusTreeNode<KEYTYPE>* BPlusTree<KEYTYPE>::SonPtr(BPlusTreeNode<KEYTYPE> *_p, int _insert_index)
{
  if (_p->sonptr_[_insert_index] != nullptr){
    return _p->sonptr_[_insert_index];
  }
  if (_p->son_file_[_insert_index].empty()){
    cerr << "没有指定文件名" << endl;//错误警告保护
    return;
  }
  if (cache_num_ >= BPlusTree_m){//释放缓存节点
    deleteCache();
  }
  BPlusTreeNode<KEYTYPE>* p = Pool.NewNode();
  cache_ptr_[cache_num_] = p;
  cache_fs_[cache_num_].open(_p->son_file_[_insert_index], ios::in | ios::oct | ios::binary);
  cache_fs_[cache_num_].read((char*)(p), sizeof(*p));
  cache_num_++;
  return p;
}


template<class KEYTYPE>
void BPlusTree<KEYTYPE>::InsertNode(KEYTYPE _key, int _data_id)
{
  int insert_index;
  BPlusTreeNode<KEYTYPE> *p, *q, *r;
  p = SearchNode(_key);
  if (p->key_num_ == 0){//第一次数据输入特殊处理
    p->key_[0] = _key;
    p->key_num_++;
  }
  else{
    insert_index = p->BinarySearch(_key);
    if (!isPrimareKey()){
      insert_index = abs(-insert_index);
    }
    if (insert_index >= 0){
      for (int i = p->key_num_; i >= insert_index; i--){
        p->key_[i] = p->key_[i - 1];
        p->key_data_id[i] = p->key_data_id[i - 1];
      }
      p->key_[insert_index - 1] = _key;
      p->key_data_id[insert_index - 1] = _data_id;
      p->key_num_++;
    }
    else{
      //*************************
      //已经存在的错误说明
      //*************************
    }
  }
  while (true){
    if (!p->isleaf()){
      insert_index = p->BinarySearch(_key);
      if (!isPrimareKey()){
        insert_index = abs(insert_index);
      }
      if (insert_index > 0){
        for (int i = p->key_num_; i >= insert_index; i--){
          p->key_[i] = p->key_[i - 1];
          p->son_file_[i] = p->son_file_[i - 1];
          p->sonptr_[i] = p->sonptr_[i - 1];
        }
        p->key_[insert_index - 1] = _key;

        ///////////不对，明天改
        p->son_file_[insert_index - 1] = q->this_file_;
        p->sonptr_[insert_index - 1] = q;
        //////////////

        p->key_num_++;
      }
    }
    if (p->key_num_ > BPlusTree_m){
      q = Pool.NewNode();//分裂一个兄弟
      r = p->father_;
      if (r == nullptr){//没有父亲就建立一个父亲
        r = Pool.NewNode();
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
      q->father_ = r;
      q->father_file_ = r->this_file_;
      r->ArrayInsert(q->key_[0]);
      WriteNodeToFile(p);
      WriteNodeToFile(q);
      WriteNodeToFile(r);
    }
    else{
      break;
    }
  }
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