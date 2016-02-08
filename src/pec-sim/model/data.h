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
  Data(const Data &data) : Block(data) {
    receivers_ = data.receivers_;
    metadata_ = data.metadata_;
  }

  static void ConfigMetadata(int metadata_entry_size) { 
    metadata_entry_size_ = metadata_entry_size;
  }

  static std::vector<Data> WrapMetadata(std::set<int> metadata, int num);

  void set_metadata(std::set<int> metadata) { Reset(); metadata_ = metadata; }
  const std::set<int> &metadata() { return metadata_; }

  void set_receivers(std::set<uint32_t> receivers) { Reset(); receivers_ = receivers; }
  const std::set<uint32_t> &receivers() { return receivers_; }

 private:
  void Encode();
  void Decode();

  static int metadata_entry_size_;

  std::set<uint32_t> receivers_;
  std::set<int> metadata_;

};

} // namespace pec

#endif // PEC_DATA_H_
