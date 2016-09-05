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
  void InsertNode(KEYTYPE _key, int _data_id);

  
  /**
  *   \ɾ���ؼ���
  *
  *   \
  */
  void DeleteNode(KEYTYPE _key);

  /**
  *   \���ҹؼ��ֵ�Ҷ�ӽڵ�
  */
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);

  /**
  *   \���ҹؼ��ֵ�Ҷ�ӽڵ�
  */
  BPlusTreeNode<KEYTYPE>* SearchNode(KEYTYPE _key);

};





#endif