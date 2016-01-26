#ifndef PEC_ERASURE_CODE_H_
#define PEC_ERASURE_CODE_H_

#include <vector>

#include <erasurecode.h>

namespace pec {

class ErasureCode {
 public:
 	static void ConfigErasureCode(int block_size, double redundancy_rate) {
 		block_size_ = block_size;
 		redundancy_rate_ = redundancy_rate;
 	}

 	static void ComputeBlockNum(uint32_t data_length, int &k, int &m);

 	static void Encode(const int k, const int m, const uint8_t *data, 
 		const uint32_t data_length, std::vector<std::vector<uint8_t> > &blocks);
 	static void Decode(const int k, const int m, 
 		const std::vector<std::vector<uint8_t> > &blocks, uint8_t **data, 
 		uint32_t &data_length);

 private:
 	static int CreateInstance(int k, int m);

 	static int block_size_;
 	static double redundancy_rate_;
};

} // namespace pec

#endif // PEC_ERASURE_CODE_H_