#ifndef _B_PLUS_TREE_NODE_H_
#define _B_PLUS_TREE_NODE_H_
#include <bits/stdc++.h>
using namespace std;
const int BPlusTree_m = 3;
const int MinBPlusTree_m = (BPlusTree_m + 1) / 2;
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
  static int name_num_;//������ˮ��

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

__declspec(selectany) template<class KEYTYPE>
int BPlusTreeNode<KEYTYPE>::name_num_ = 0;

#endif