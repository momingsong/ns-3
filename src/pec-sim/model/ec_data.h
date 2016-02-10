#ifndef PEC_EC_DATA_H_
#define PEC_EC_DATA_H_

#include "block.h"
#include "data.h"

namespace pec {

class ECData : public Block {
 public:
 	ECData() { }
 	ECData(const ECData &ec_data) : Block(ec_data) {
 		idx_ = ec_data.idx_;
 		receivers_ = ec_data.receivers_;
 		block_ = ec_data.block_;
 	}

 	static std::vector<ECData> EncodeData(Data &data);
 	static Data DecodeData(std::vector<ECData> &ec_data);
 	
 	void set_idx(int idx) { Reset(); idx_ = idx; }
 	int idx() { return idx_; }

 	void set_receivers(std::set<uint32_t> receivers) { Reset(); receivers_ = receivers; }
 	const std::set<uint32_t> &receivers() { return receivers_; } 

 	void set_block(std::vector<uint8_t> block) { Reset(); block_ = block; }
 	const std::vector<uint8_t> block() { return block_; }

 private:
 	void Encode();
 	void Decode();

 	int idx_;
 	std::set<uint32_t> receivers_;

 	std::vector<uint8_t> block_;
};

} // namespace pec

#endif // PEC_EC_DATA_H_