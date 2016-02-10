#ifndef PEC_ERASURE_CODE_H_
#define PEC_ERASURE_CODE_H_

#include <vector>

#include <erasurecode.h>

namespace pec {

class ErasureCode {
 public:
 	static void ConfigErasureCode(int k, int m) {
 		k_ = k;
 		m_ = m;
 	}
 	
 	static void Encode(const uint8_t *data, const uint32_t data_length, 
 		std::vector<std::vector<uint8_t> > &blocks);
 	static void Decode(const std::vector<std::vector<uint8_t> > &blocks, 
 		uint8_t **data, uint32_t &data_length);

 	static int k() { return k_; }
 	static int m() { return m_; }

 private:
 	static int CreateInstance();

 	static int k_;
 	static int m_;

 	static const int MAX_FRAGMENTS = 32;
};

} // namespace pec

#endif // PEC_ERASURE_CODE_H_