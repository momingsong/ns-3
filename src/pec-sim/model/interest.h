#ifndef PEC_INTEREST_H_
#define PEC_INTEREST_H_

#include <string>
#include <vector>
#include <set>

#include "tlv.h"
#include "block.h"

namespace pec {

class Interest : public Block {
 public:
  static const uint32_t kSizeWithoutBloomFilter = 48;
  static const uint32_t kSizeWithBloomFilter = 4144;
  Interest();
  Interest(std::set<int> metadata);

  bool HasMetadata(int metadata);

  void AddMetadata(int metadata);

  void AddMetadata(std::set<int> metadata);

  void set_nonce(int nonce) { Reset(); nonce_ = nonce; }
  int nonce() { return nonce_; }

  void set_sender(int sender) { Reset(); sender_ = sender; }
  int sender() { return sender_; }

  const std::set<int> &metadata() { return metadata_; }

 private:
  void Encode();
  void Decode();

  int nonce_;
  int sender_;
  std::set<int> metadata_;
};

} // namespace pec

#endif // PEC_INTEREST_H_