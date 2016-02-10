#ifndef PEC_EC_ACK_H_
#define PEC_EC_ACK_H_

#include <vector>

#include "block.h"
#include "tlv.h"
#include "erasure_code.h"

namespace pec{

class ECAck : public Block {
 public:
 	ECAck() {}
/* 	ECAck(int nonce, int hop_nonce, uint32_t from, std::vector<bool> received) {
 		nonce_ = nonce;
 		hop_nonce_ = hop_nonce;
 		from_ = from;
 		received_ = received;
 	}*/
 	ECAck(int nonce, int hop_nonce, uint32_t from, std::vector<int> ridx) {
 		nonce_ = nonce;
 		hop_nonce_ = hop_nonce;
 		from_ = from;
 		received_.clear();
 		for (int i = 0; i < ErasureCode::k() + ErasureCode::m(); ++i) {
 			received_.push_back(false);
 		}
 		for (unsigned int i = 0; i < ridx.size(); ++i) {
 			received_[ridx[i]] = true;
 		}
 	}
 	ECAck(const ECAck &ec_ack) : Block(ec_ack) {
 		from_ = ec_ack.from_;
 		received_ = ec_ack.received_;
 	}

 	uint32_t from() { return from_; }
 	const std::vector<bool> received() { return received_; }

 private:
 	void Encode() {
 		Reset();
 		wire_length_ = sizeof(TlvType) + sizeof(uint32_t) * 2 + sizeof(int) * 2
 									+ sizeof(bool) * (ErasureCode::k() + ErasureCode::m());
 		wire_begin_ = new uint8_t[wire_length_];

 		uint8_t *p = wire_begin_;
    *((TlvType *)p) = kTlvECAck;
    p += sizeof(TlvType);
    *((uint32_t *)p) = wire_length_;
    p += sizeof(uint32_t);
    *((int *)p) = nonce_;
    p += sizeof(int);
    *((int *)p) = hop_nonce_;
    p += sizeof(int);
    *((uint32_t *)p) = from_;
    p += sizeof(uint32_t);
    for (int i = 0; i < ErasureCode::k() + ErasureCode::m(); ++i) {
    	*((bool *)p) = received_[i];
    	p += sizeof(bool);
    }
 	}
 	void Decode() {
 		uint8_t *p = wire_begin_;
    p += sizeof(TlvType);
    p += sizeof(uint32_t);
    nonce_ = *((int *)p);
    p += sizeof(int);
    hop_nonce_ = *((int *)p);
    p += sizeof(int);
    from_ = *((uint32_t *)p);
    p += sizeof(uint32_t);
    received_.clear();
    for (int i = 0; i < ErasureCode::k() + ErasureCode::m(); ++i) {
    	received_.push_back(*((bool *)p));
    	p += sizeof(bool);
    }
 	}

 	uint32_t from_;
 	std::vector<bool> received_;
};

} // namespace pec

#endif // PEC_EC_ACK_H_