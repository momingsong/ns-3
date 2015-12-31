#ifndef PEC_DATA_H_
#define PEC_DATA_H_

#include <string>
#include <vector>
#include <set>

#include "tlv.h"
#include "block.h"

namespace pec {

class Data : public Block {
 public:
  Data();
  Data(std::set<int> metadata);

  static std::vector<Data> WrapMetadata(std::set<int> metadata, int num);

  uint32_t size() { return GetWireLength() * 5; }

  void set_nonce(int nonce) { Reset(); nonce_ = nonce; }
  int nonce() { return nonce_; }

  void set_hop_nonce(int hop_nonce) { Reset(); hop_nonce_ = hop_nonce; }
  int hop_nonce() { return hop_nonce_; }

  void set_metadata(std::set<int> metadata) { Reset(); metadata_ = metadata; }
  const std::set<int> &metadata() { return metadata_; }

  void set_receivers(std::set<uint32_t> receivers) { Reset(); receivers_ = receivers; }
  const std::set<uint32_t> &receivers() { return receivers_; }

 private:
  void Encode();
  void Decode();
  int nonce_;
  int hop_nonce_;
  std::set<uint32_t> receivers_;
  std::set<int> metadata_;
};

} // namespace pec

#endif // PEC_DATA_H_