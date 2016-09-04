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
  MemPool<KEYTYPE> Pool;//�ڴ��
  bool is_primare_key_;//�Ƿ���������ϵ���Ƿ��ظ���
  BPlusTreeNode<KEYTYPE> *root_;//���ڵ�
  fstream rootfs_;//���ڵ��ļ���
  BPlusTreeNode<KEYTYPE> *mem_ptr_[BPlusTree_m];//��Ҫ��פ�ڴ�Ľڵ�ָ��
  fstream mem_fs_;//��Ҫ��פ�ڴ�Ľڵ��ļ���
  BPlusTreeNode<KEYTYPE> *cache_ptr_[BPlusTree_m];//��ʱ�����ڴ�Ľڵ�ָ��
  fstream cache_fs_;//��ʱ�����ڴ�Ľڵ��ļ���
  int cache_num_;//��ʱ�����ڴ�Ľڵ�����

  /**
  *   \���ļ�����ڵ�
  */
  void ReadNodeFromFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \���ڵ�д���ļ�
  */
  void WriteNodeToFile(BPlusTreeNode<KEYTYPE> *_p);
  /**
  *   \�ͷŻ���ڵ�
  */
  void deleteCache();
  /**
  *   \���ض��ӵ�ָ��
  *
  *   \����ڵ�û�ж����ڴ棬�Ͷ��룬������ָ��
  */
  BPlusTreeNode<KEYTYPE>* SonPtr(BPlusTreeNode<KEYTYPE> *_p, int _insert_index);
  public:
  BPlusTree();
  ~BPlusTree();
  /**
  *   \�Ƿ�Ϊ����
  */
  bool isPrimareKey(){ return is_primare_key_ };


  void InsertNode(KEYTYPE _key, int _data_id);//����ؼ���
  void DeleteNode(KEYTYPE _key);//ɾ���ؼ���
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);



};





#endif