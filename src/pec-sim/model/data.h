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

  void set_nonce(int nonce) { Reset(); nonce_ = nonce; }
  int nonce() { return nonce_; }

  void set_metadata(std::set<int> metadata) { metadata_ = metadata; }
  const std::set<int> &metadata() { return metadata_; }

 private:
  void Encode();
  void Decode();

  int nonce_;
  std::set<int> metadata_;
};

} // namespace pec

#endif // PEC_DATA_H_