#include "data.h"

#include "ns3/log.h"

namespace pec {

int Data::metadata_entry_size_ = 30;

Data::Data() { nonce_ = rand(); }

Data::Data(std::set<int> metadata) {
  metadata_ = metadata;
  nonce_ = rand();
}

std::vector<Data> Data::WrapMetadata(std::set<int> metadata, int max) {
  std::vector<Data> datas;

  if (max <= 0) {
    if (metadata.size() > 0)
      datas.push_back(Data(metadata));

  } else {
    std::set<int> meta;
    std::set<int>::iterator iter = metadata.begin();
    while (iter != metadata.end()) {
      if (meta.size() >= (uint32_t)max) {
        datas.push_back(Data(meta));
        meta.clear();
      } 
      meta.insert(*iter);
      ++iter;
    }
    if (meta.size() > 0)
      datas.push_back(Data(meta));
  }
  
  return datas;
}

void Data::Encode() {
  Reset();

  wire_length_ = sizeof(TlvType) + sizeof(int) * 3
                + metadata_entry_size_ * metadata_.size() 
                + sizeof(uint32_t) * (receivers_.size() + 1);
  wire_begin_ = new uint8_t[wire_length_];

  uint8_t *p = wire_begin_;
  *((TlvType *)p) = kTlvData;
  p += sizeof(TlvType);
  *((uint32_t *)p) = wire_length_;
  p += sizeof(uint32_t);
  *((int *)p) = nonce_;
  p += sizeof(int);
  *((int *)p) = hop_nonce_;
  p += sizeof(int);
  *((int *)p) = receivers_.size();
  p += sizeof(int);
  std::set<uint32_t>::iterator r_iter = receivers_.begin();
  while (r_iter != receivers_.end()) {
    *((uint32_t *)p) = *r_iter;
    p += sizeof(uint32_t);
    ++r_iter;
  }
  std::set<int>::iterator m_iter = metadata_.begin();
  while (m_iter != metadata_.end()) {
    *((int *)p) = *m_iter;
    uint8_t *q = p + sizeof(int);
    while (q != p + metadata_entry_size_) {
      *q = 0;
      ++q;
    }
    p += metadata_entry_size_;
    ++m_iter;
  }
}

void Data::Decode() {
  uint8_t *p = wire_begin_;
  p += sizeof(TlvType);
  p += sizeof(uint32_t);
  nonce_ = *((int *)p);
  p += sizeof(int);
  hop_nonce_ = *((int *)p);
  p += sizeof(int);
  int num_receiver = *((int *)p);
  p += sizeof(int);
  receivers_.clear();
  while (num_receiver-- > 0) {
    receivers_.insert(*((uint32_t *)p));
    p += sizeof(uint32_t);
  }
  metadata_.clear();
  while (p != wire_begin_ + wire_length_) {
    metadata_.insert(*((int *)p));
    p += metadata_entry_size_;
  }
}

} // namespace pec
