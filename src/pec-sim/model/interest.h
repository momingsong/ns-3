#ifndef PEC_INTEREST_H_
#define PEC_INTEREST_H_

#include <string>
#include <vector>
#include <set>

#include "bloom_filter.h"
#include "tlv.h"
#include "block.h"

namespace pec {

class Interest : public Block {
 public:
  Interest();
  Interest(std::set<int> metadata);
  Interest(const Interest &interest) : Block(interest) {
    sender_ = interest.sender_;
    has_filter_ = interest.has_filter_;
    filter_ = interest.filter_;
  }

  static void ConfigBloomFilter(int max, int min, double fpp) {
    bf_size_max_ = max;
    bf_size_min_ = min;
    bf_fpp_ = fpp;
  }

  bool HasMetadata(int metadata);

  void AddMetadata(int metadata);

  void AddMetadata(std::set<int> metadata);

  void set_sender(uint32_t sender) { Reset(); sender_ = sender; }
  uint32_t sender() { return sender_; }

  const bloom_filter &filter() { return filter_; }

 private:
  void Encode();
  void Decode();

  static int bf_size_max_;
  static int bf_size_min_;
  static double bf_fpp_;

  uint32_t sender_;

  bloom_filter filter_;
  bool has_filter_;
};

} // namespace pec

#endif // PEC_INTEREST_H_