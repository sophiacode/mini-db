#include "IDPool.h"


int IDPool::NewNode()
{
  if (nodelist_p != 0){
    return nodelist[--nodelist_p];
  }
  else if (blocklist != 0){
    in_file_stream_->seekg((blocklist - 1)*sizeof(int)*IDSIZE + sizeof(*this), ios::beg);
    in_file_stream_->read((char*)(nodelist), sizeof(int)*IDSIZE);
    blocklist--;
    nodelist_p = IDSIZE;
    return nodelist[--nodelist_p];
  }
  else{
    return nowid++;
  }
}

void IDPool::deleteNode(int p)
{
  if (nodelist_p == IDSIZE){
    out_file_stream_->seekp(blocklist*sizeof(int)*IDSIZE + sizeof(*this), ios::beg);
    out_file_stream_->write((char*)(nodelist), sizeof(int)*IDSIZE);
    out_file_stream_->flush();
    blocklist++;
    nodelist_p = 0;
  }
  nodelist[nodelist_p++] = p;
}

void IDPool::release()
{

}

void IDPool::savePool()
{

}

IDPool::IDPool()
{
  out_file_stream_ = nullptr;
  in_file_stream_ = nullptr;
}


IDPool::IDPool(string sPath)
{
  blocklist = 0;
  nodelist_p = 0;
  nowid = 0;
  out_file_stream_ = new ofstream();
  in_file_stream_ = new ifstream();
  for (int i = 0; i < sPath.size(); i++){
    _path_[i] = sPath[i];
  }
  _path_[sPath.size()] = '\0';
  out_file_stream_->open(_path_, ios::binary);
  if (!out_file_stream_->is_open()){
    cerr << "打开文件 " << _path_ << " 失败" << endl;
  }
  out_file_stream_->close();

  out_file_stream_->open(_path_, ios::binary|ios::in);
  if (!out_file_stream_->is_open()){
    cerr << "打开文件 " << _path_ << " 失败" << endl;
  }
  in_file_stream_->open(_path_, ios::binary);
  if (!in_file_stream_->is_open()){
    cerr << "打开文件 " << _path_ << " 失败" << endl;
  }
}

IDPool::~IDPool()
{
  if (out_file_stream_ != nullptr){
    delete out_file_stream_;
    out_file_stream_ = nullptr;
  }
  if (in_file_stream_ != nullptr){
    delete in_file_stream_;
    in_file_stream_ = nullptr;
  }
  
}
