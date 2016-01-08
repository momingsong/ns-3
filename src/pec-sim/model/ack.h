#ifndef PEC_ACK_H_
#define PEC_ACK_H_

#include "block.h"
#include "tlv.h"

namespace pec {
class Ack : public Block {
 public:
  Ack() {}
  Ack(int nonce, int hop_nonce, uint32_t from) 
    : from_(from) {
    nonce_ = nonce;
    hop_nonce_ = hop_nonce;
  }
  Ack(const Ack &ack) : Block(ack) { from_ = ack.from_; }

  uint32_t from() { return from_; }
 
 private:
  void Encode() {
    Reset();
    wire_length_ = sizeof(TlvType) + sizeof(uint32_t) * 2 + sizeof(int) * 2;
    wire_begin_ = new uint8_t[wire_length_];

    uint8_t *p = wire_begin_;
    *((TlvType *)p) = kTlvAck;
    p += sizeof(TlvType);
    *((uint32_t *)p) = wire_length_;
    p += sizeof(uint32_t);
    *((int *)p) = nonce_;
    p += sizeof(int);
    *((int *)p) = hop_nonce_;
    p += sizeof(int);
    *((uint32_t *)p) = from_;
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
  }

  uint32_t from_;
};

} // namespace pec

#endif // PEC_ACK_H_