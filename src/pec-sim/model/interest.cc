#include "interest.h"

namespace pec {

Interest::Interest() { nonce_ = rand(); }

Interest::Interest(std::set<int> metadata) {
  std::set<int>::iterator iter = metadata.begin();
  while (iter != metadata.end()) {
    metadata_.insert(*iter);
    ++iter;
  }
  nonce_ = rand();
}

bool Interest::HasMetadata(int metadata) {
  return metadata_.find(metadata) != metadata_.end();
}

void Interest::AddMetadata(int metadata) {
  Reset();
  
  metadata_.insert(metadata);
}

void Interest::AddMetadata(std::set<int> metadata) {
  Reset();

  std::set<int>::iterator iter = metadata_.begin();
  while (iter != metadata_.end()) {
    metadata_.insert(*iter);
    ++iter;
  }
}

void Interest::Encode() {
  Reset();

  wire_length_ = kTlvTypeLengthSize + sizeof(int) * (2 + metadata_.size());
  wire_begin_ = new uint8_t[wire_length_];

  TlvType type = kTlvInterest;
  *((TlvType *)wire_begin_) = type;
  *((uint32_t *)(wire_begin_ + kTlvTypeSize)) = wire_length_;
  *((int *)(wire_begin_ + kTlvTypeLengthSize)) = nonce_;
  *((int *)(wire_begin_ + kTlvTypeLengthSize + sizeof(int))) = sender_;
  std::set<int>::iterator iter = metadata_.begin();
  int i = 0;
  while (iter != metadata_.end()) {
    *((int *)(wire_begin_ + kTlvTypeLengthSize + sizeof(int) * (i++ + 2))) = *iter;
    ++iter;
  }
}

void Interest::Decode() {
  nonce_ = *((int *)(wire_begin_ + kTlvTypeLengthSize));
  sender_ = *((int *)(wire_begin_ + kTlvTypeLengthSize + sizeof(int)));
  int *p = (int *)(wire_begin_ + kTlvTypeLengthSize + sizeof(int) * 2);
  metadata_.clear();
  while ((uint8_t *)p != wire_begin_ + wire_length_) {
    metadata_.insert(*p);
    ++p;
  }
}

} // namespace pec