#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_
#include "BPlusTreeNode.h"
#include "MemPool.h"
#include <fstream>
using namespace std;


template<class KEYTYPE>
class BPlusTree
{

private:


  MemPool<KEYTYPE> Pool;//内存池

  bool is_primare_key_;//是否主键（关系到是否重复）

  BPlusTreeNode<KEYTYPE> *root_;//根节点

  fstream rootfs_;//根节点文件流

  fstream file_stream_;//随机文件流


  /**
  *   \检查并返回儿子的指针
  *
  *   \有现成指针则返回，否则返回读取文件后new的指针
  */
  BPlusTreeNode<KEYTYPE>* SonPtr(BPlusTreeNode<KEYTYPE> *_p, int _insert_index);


  /**
  *   \检查并返回父亲节点
  *
  *   \有现成指针则返回，否则返回读取文件后new的指针
  */
  BPlusTreeNode<KEYTYPE> *FatherPtr(BPlusTreeNode<KEYTYPE> *_p);


public:


  /**
  *   \构造函数
  *
  *   \传入数据库路径：库 \ 表 \
  */
  BPlusTree(string _file_name);


  /**
  *   \析构函数
  */
  ~BPlusTree();


  /**
  *   \是否为主键
  */
  bool isPrimareKey(){ return is_primare_key_ };

  /**
  *   \置为主键
  */
  void SetPrimareKey(){ is_primare_key_ = true };


  /**
  *   \将文件读入节点
  *
  *   \返回是否成功
  */
  bool ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p);


  /**
  *   \将节点写入文件
  *
  *   \返回是否成功
  */
  bool WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p);


  /**
  *   \将cache内存中节点全部保存到文件中
  *
  *   \返回保存的文件个数
  *
  *   \失败返回-1
  */
  int DeleteCache();


  /**
  *   \插入关键字
  *
  *   \接口：键值，该键值对应于文件中的id
  */
  bool InsertNode(KEYTYPE _key, int _data_id);

  
  /**
  *   \删除关键字
  *
  *   \接口：键值
  */
  bool DeleteNode(KEYTYPE _key);


  /**
  *   \删除指定关键字
  *
  *   \接口：键值
  */
  bool DeleteNode(KEYTYPE _key, int _data_id);


  /**
  *   \查找关键字的叶子节点
  *
  *   \接口：键值
  */
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);


  /**
  *   \查找第一个关键字的id
  *
  *   \接口：键值
  */
  int SearchID(KEYTYPE _key);


  /**
  *   \更新节点
  *
  *   \接口：键值
  */
  bool UpDataNode(KEYTYPE _old_key, KEYTYPE _new_key);


  /**
  *   \准确更新一个节点
  *
  *   \接口：旧键，新键，id
  */
  bool UpDataNode(KEYTYPE _old_key, KEYTYPE _new_key, int _data_id);
};


//***************************************************************************
//
//        模板类定义
//
//***************************************************************************


template<class KEYTYPE>
BPlusTree<KEYTYPE>::BPlusTree(string _file_name)
{
  root_ = Pool.NewNode(_file_name);
  is_primare_key_ = false;
}


template<class KEYTYPE>
BPlusTree<KEYTYPE>::~BPlusTree()
{

}


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


//*********************************************************************


template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::InsertNode(KEYTYPE _key, int _data_id)
{
  int insert_index;
  bool flag_first = false;//是否插入到第一个位置
  KEYTYPE last_first_key;//查找之前的第一个key;
  BPlusTreeNode<KEYTYPE> *p, *q, *r, *t;
  p = SearchNode(_key);
  if (p->key_num_ == 0){//第一次数据输入特殊处理(这里p应该==root_)
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
      //键值已经存在的 错误 说明
      //*************************
    }
  }
  r = p;
  while (flag_first == true){//更新第一个节点
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
    q = Pool.NewNode();//分裂一个兄弟
    r = FatherPtr(p);
    if (r == nullptr){//没有父亲就建立一个父亲(新根节点)
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
        t = SonPtr(q);
        t->father_ = q;
        t->father_file_ = q->this_file_;
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

    insert_index = r->BinarySearch(q->key_[0]);//重复可能有问题
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
bool BPlusTree<KEYTYPE>::DeleteNode(KEYTYPE _key)
{
  bool borrow_flag;//借节点成功标记;
  int insert_index;
  bool flag_first = false;//是否删除了第一个位置
  KEYTYPE next_first_key;//查找下一个的第一个key;
  BPlusTreeNode<KEYTYPE> *p, *q, *r, *t;
  p = SearchNode(_key);
  if (p->key_num_ == 0){//树为空
    //*************************
    //树为空（键值不存在）的 错误 说明
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
    //键值不存在的 错误 说明
    //*************************
  }
  r = p;
  while (flag_first == true){//更新第一个节点
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
  while (p->key_num_ < MinBPlusTree_m){
    if (p == root_){
      if (p->key_num_ == 1){
        root_ = SonPtr(root_, 0);
        root_->father_ = nullptr;
        root_->father_file_.clear();
      }
      break;
    }
    borrow_flag = false;
    r = FatherPtr(p);
    insert_index = -(r->BinarySearch(p->key_[0]));
    if (insert_index > 1){//向左边借
      q = SonPtr(r, insert_index - 2);
      if (q->key_num_ > MinBPlusTree_m){
        borrow_flag = true;
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
        p->key_num_++;
        q->key_num_--;
        p->key_[0] = q->key_[q->key_num_];
        if (p->isleaf()){
          p->key_data_id[0] = q->key_data_id[q->key_num_];
        }
        else{
          p->sonptr_[0] = q->sonptr_[q->key_num_];
          p->son_file_[0] = q->son_file_[q->key_num_];
        }
        t = SonPtr(p, 0);
        t->father_ = p;
        t->father_file_ = p->this_file_;
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
    else if (insert_index < r->key_num_){//向右边借
      q = SonPtr(r, insert_index);
      if (q->key_num_>MinBPlusTree_m){
        borrow_flag = true;
        p->key_[p->key_num_] = q->key_[0];
        if (p->isleaf()){
          p->key_data_id[p->key_num_] = q->key_data_id[0];
        }
        else{
          p->sonptr_[p->key_num_] = q->sonptr_[0];
          p->son_file_[p->key_num_] = q->son_file_[0];
        }
        t = SonPtr(p, p->key_num_);
        t->father_ = p;
        t->father_file_ = p->this_file_;
        p->key_num_++;
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
        //应该完了
      }
    }
    if (borrow_flag == true){//两边都借不到  orz
      if (insert_index > 1){//向左合并
        q = SonPtr(r, insert_index - 2);
        for (int i = 0; i < p->key_num_; i++){
          q->key_[q->key_num_ + i] = p->key_[i];
          if (q->isleaf()){
            q->key_data_id[q->key_num_ + i] = p->key_data_id[i];
          }
          else{
            q->sonptr_[q->key_num_ + i] = p->sonptr_[i];
            q->son_file_[q->key_num_ + i] = p->son_file_[i];
            t = SonPtr(p, i);
            t->father_ = q;
            t->father_file_ = q->this_file_;
          }
        }
        q->key_num_ += p->key_num_;
        p->key_num_ = 0;
        r->key_num_--;
        for (int i = insert_index - 1; i < r->key_num_; i++){
          r->key_[i] = r->key_[i + 1];
          r->sonptr_[i] = r->sonptr_[i + 1];
          r->son_file_[i] = r->son_file_[i + 1];
        }
        if (p->isleaf()){
          if (p->brother_ != nullptr){
            p->brother_->sister_ = p->sister_;
          }
          if (p->sister_ != nullptr){
            p->sister_->brother_ = p->brother_;
          }
        }
        Pool.deleteNode(p);
        borrow_flag = false;
      }
      else if (insert_index < r->key_num_){//向右合并
        q = SonPtr(r, insert_index);
        for (int i = q->key_num_ + p->key_num_ - 1; i >= p.key_num_; i--){
          q->key_[i] = q->[i - q->key_num_ + 1];
          if (q->isleaf()){
            q->key_data_id[i] = q->key_data_id[i - q->key_num_ + 1];
          }
          else{
            q->sonptr_[i] = q->sonptr_[i - q->key_num_ + 1];
            q->son_file_[i] = q->son_file_[i - q->key_num_ + 1];
          }
        }
        for (int i = 0; i < p.key_num_; i++){
          q->key_[i] = p->key_[i];
          if (q->isleaf()){
            q->key_[i] = p->key_[i];
          }
          else{
            q->sonptr_[i] = p->sonptr_[i];
            q->son_file_[i] = p->son_file_[i];
            t = SonPtr(q, i);
            t->father_ = q;
            t->father_file_ = t->this_file_;
          }
        }
        q->key_num_ += p->key_num_;
        p->key_num_ = 0;
        r->key_num_--;
        r->key_[insert_index] = q->key_[0];
        for (int i = insert_index - 1; i < r->key_num_; i++){
          r->key_[i] = r->key_[i + 1];
          r->sonptr_[i] = r->sonptr_[i + 1];
          r->son_file_[i] = r->son_file_[i + 1];
        }
        if (p->isleaf()){
          if (p->brother_ != nullptr){
            p->brother_->sister_ = p->sister_;
          }
          if (p->sister_ != nullptr){
            p->sister_->brother_ = p->brother_;
          }
        }
        Pool.deleteNode(p);
        borrow_flag = false;
      }
    }
    p = r;
  }
  return true;
}


template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::DeleteNode(KEYTYPE _key, int _data_id)
{

}


template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::UpDataNode(KEYTYPE _old_key, KEYTYPE _new_key, int _data_id)
{

}


template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::UpDataNode(KEYTYPE _old_key, KEYTYPE _new_key)
{
  int insert_index;
  BPlusTreeNode<KEYTYPE> *p;
  p = SearchNode(_old_key);
  if (p = nullptr){
    return false;//ERROR：没有找到
  }
  insert_index = -(p->BinarySearch(_key));
  if (insert_index < 0){
    return false;
  }
  p->key_[insert_index] = _new_key;
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
    return -1;//ERROR：没有找到
  }
  insert_index = p->BinarySearch(_key);
  if (insert_index > 0){
    return -1;
  }
  return p->key_data_id[-insert_index];
}


template<class KEYTYPE>
int BPlusTree<KEYTYPE>::DeleteCache()
{
  int num = Pool.cachelist.size();
  for (auto x : Pool.cachelist){
    if (x->this_file_.empty()){
      return -1;
    }
    file_stream_.clear();
    file_stream_.open(x->this_file_, ios::in | ios::out || ios::binary);
    x->brother_ = nullptr;
    x->sister_ = nullptr;
    x->father_ = nullptr;
    for (int i = 0; i < _p.key_num_; i++){
      x->sonptr_[i] = nullptr;
    }
    file_stream_.write((char*)(x), sizeof(*x));
  }
  Pool.cachelist.clear();
  return num;
}


template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p)
{
  if (_p->this_file_.empty()){
    return false;
  }
  file_stream_.clear();
  file_stream_.open(_p->this_file_, ios::in | ios::out || ios::binary);
  file_stream_.read((char*)(_p), sizeof(*_p));
  return true;
}


template<class KEYTYPE>
bool BPlusTree<KEYTYPE>::WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p)
{
  if (_p->this_file_.empty()){
    return false;
  }
  file_stream_.clear();
  file_stream_.open(_p->this_file_, ios::in | ios::out || ios::binary);
  _p->brother_ = nullptr;
  _p->sister_ = nullptr;
  _p->father_ = nullptr;
  for (int i = 0; i < _p.key_num_; i++){
    _p->sonptr_[i] = nullptr;
  }
  file_stream_.write((char*)(_p), sizeof(*_p));
  return true;
}


//*********************************************************************



#endif