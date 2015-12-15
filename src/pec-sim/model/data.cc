#include "data.h"

#include "ns3/log.h"

namespace pec {

Data::Data() { nonce_ = rand(); }

// Data::Data(std::string message) : message_(message) { nonce_ = rand(); }

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
      if (meta.size() >= max) {
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

  wire_length_ = kTlvTypeLengthSize + sizeof(int) * (1 + metadata_.size());
  wire_begin_ = new uint8_t[wire_length_];

  TlvType type = kTlvData;
  *((TlvType *)wire_begin_) = type;
  *((uint32_t *)(wire_begin_ + kTlvTypeSize)) = wire_length_;
  *((int *)(wire_begin_ + kTlvTypeLengthSize)) = nonce_;
  std::set<int>::iterator iter = metadata_.begin();
  for (int i = 0; i < metadata_.size(); ++i) {
    *((int *)(wire_begin_ + kTlvTypeLengthSize + sizeof(int) * (i + 1))) = *iter;
    ++iter;
  }
}

void Data::Decode() {
  nonce_ = *((int *)(wire_begin_ + kTlvTypeLengthSize));
  int *p = (int *)(wire_begin_ + kTlvTypeLengthSize + sizeof(int));
  metadata_.clear();
  while ((uint8_t *)p != wire_begin_ + wire_length_) {
    metadata_.insert(*p);
    ++p;
  }
}

} // namespace pec