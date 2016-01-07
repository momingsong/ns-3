#include "interest.h"
namespace pec {

int Interest::bf_size_max_;
int Interest::bf_size_min_;
double Interest::bf_fpp_;

Interest::Interest() {
  nonce_ = rand();
  has_filter_ = false;
}

Interest::Interest(std::set<int> metadata) {
  if (metadata.size() == 0) {
    Interest();
  } else {
    bloom_parameters paras;
    paras.maximum_size = bf_size_max_ * 8;
    paras.minimum_size = bf_size_min_ * 8;
    paras.false_positive_probability = bf_fpp_;
    paras.projected_element_count = metadata.size();
    paras.compute_optimal_parameters();
    bloom_filter new_filter(paras);
    filter_ = new_filter;
    has_filter_ = true;

    std::set<int>::iterator iter = metadata.begin();
    while (iter != metadata.end()) {
      int m = *iter++;
      filter_.insert((char*)&m, sizeof(int));
    }
    nonce_ = rand();
    
  }
}

bool Interest::HasMetadata(int metadata) {
  if (has_filter_)
    return filter_.contains((char *)&metadata, sizeof(int));
  else
    return false;
}

void Interest::AddMetadata(int metadata) {
  Reset();
  if (!has_filter_) {
    bloom_parameters paras;
    paras.maximum_size = bf_size_max_ * 8;
    paras.minimum_size = bf_size_min_ * 8;
    paras.false_positive_probability = bf_fpp_;
    paras.projected_element_count = 1;
    paras.compute_optimal_parameters();
    bloom_filter new_filter(paras);
    filter_ = new_filter;
    has_filter_ = true;
  }
  filter_.insert((char *)&metadata, sizeof(int));
}

void Interest::AddMetadata(std::set<int> metadata) {
  Reset();
  if (!has_filter_) {
    bloom_parameters paras;
    paras.maximum_size = bf_size_max_ * 8;
    paras.minimum_size = bf_size_min_ * 8;
    paras.false_positive_probability = bf_fpp_;
    paras.projected_element_count = metadata.size();
    paras.compute_optimal_parameters();
    bloom_filter new_filter(paras);
    filter_ = new_filter;
    has_filter_ = true;
  }
  std::set<int>::iterator iter = metadata.begin();
  while (iter != metadata.end()) {
    int m = *iter++;
    filter_.insert((char *)&m, sizeof(int));
  }
}

void Interest::Encode() {
  Reset();

  wire_length_ = sizeof(TlvType) + sizeof(uint32_t) * 2 + sizeof(int) * 2 + sizeof(bool);
  if (has_filter_)
    wire_length_ += filter_.GetLength();
  wire_begin_ = new uint8_t[wire_length_];

  uint8_t *p = wire_begin_;
  *((TlvType *)p) = kTlvInterest;
  p += sizeof(TlvType);
  *((uint32_t *)p) = wire_length_;
  p += sizeof(uint32_t);
  *((int *)p) = nonce_;
  p += sizeof(int);
  *((int *)p) = hop_nonce_;
  p += sizeof(int);
  *((uint32_t *)p) = sender_;
  p += sizeof(uint32_t);
  *((bool *)p) = has_filter_;
  p += sizeof(bool);
  if (has_filter_)
    filter_.Encode(p);
}

void Interest::Decode() {
  uint8_t *p = wire_begin_;
  p += sizeof(TlvType);
  p += sizeof(uint32_t);
  nonce_ = *((int *)p);
  p += sizeof(int);
  hop_nonce_ = *((int *)p);
  p += sizeof(int);
  sender_ = *((uint32_t *)p);
  p += sizeof(uint32_t);
  has_filter_ = *((bool *)p);
  p += sizeof(bool);
  if (has_filter_) {
    filter_.Decode(p);
  }
}

} // namespace pec