#ifndef _B_PLUS_TREE_H_
#define _B_PLUS_TREE_H_
#include "BPlusTreeNode.h"
template<class KEYTYPE>
class BPlusTree
{
private:
  BPlusTreeNode<KEYTYPE> *root_;//���ڵ�
  bool is_primare_key_;//�Ƿ���������ϵ���Ƿ��ظ���


  public:
  BPlusTree();
  ~BPlusTree();
  /**
  *   \�Ƿ�Ϊ����
  */
  bool isPrimareKey(){ return is_primare_key_ };


  void InsertNode(KEYTYPE _key, int _data_id);//����ؼ���
  void deleteNode(KEYTYPE _key);//ɾ���ؼ���




};





#endif