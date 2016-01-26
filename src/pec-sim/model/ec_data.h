#ifndef PEC_EC_DATA_H_
#define PEC_EC_DATA_H_

#include "block.h"
#include "data.h"

namespace pec {

class ECData : public Block {
 public:
 	ECData() { }
 	ECData(const ECData &ec_data) : Block(ec_data) {
 		k_ = ec_data.k_;
 		m_ = ec_data.m_;
 		idx_ = ec_data.idx_;
 		block_ = ec_data.block_;
 	}

 	static std::vector<ECData> EncodeData(Data &data);
 	static Data DecodeData(std::vector<ECData> &ec_data);
 	
 	void set_k(int k) { Reset(); k_ = k; }
 	int k() { return k_; }
 	void set_m(int m) { Reset(); m_ = m; }
 	int m() { return m_; }
 	void set_idx(int idx) { Reset(); idx_ = idx; }
 	int idx() { return idx_; }

 	void set_block(std::vector<uint8_t> block) { Reset(); block_ = block; }
 	const std::vector<uint8_t> block() { return block_; }

 private:
 	void Encode();
 	void Decode();

 	int k_;
 	int m_;
 	int idx_;

 	std::vector<uint8_t> block_;
};

} // namespace pec

#endif // PEC_EC_DATA_H_