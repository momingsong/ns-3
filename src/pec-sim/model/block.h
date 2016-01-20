#ifndef PEC_BLOCK_H_
#define PEC_BLOCK_H_

#define _CPP11NSUP
#include <cstdlib>
#ifdef _CPP11NSUP
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#endif

namespace pec {

class Block {
 public:
  Block();
  Block(const Block &block);
  Block(uint8_t *buffer, uint32_t buffer_size);
  ~Block();

  uint32_t GetWireLength();
  void GetWire(uint8_t *buffer, uint32_t buffer_size);
  void DecodeFromBuffer(const uint8_t *buffer, uint32_t buffer_size);

  void set_nonce(int nonce) { Reset(); nonce_ = nonce; }
  int nonce() { return nonce_; }

  void set_hop_nonce(int hop_nonce) { Reset(); hop_nonce_ = hop_nonce; }
  int hop_nonce() { return hop_nonce_; }

 protected:
  void Reset();
  virtual void Encode();
  virtual void Decode();

  uint32_t wire_length_;
  uint8_t *wire_begin_;
  bool has_wire_;

  int nonce_;
  int hop_nonce_;
};

} // namespace pec

#endif // PEC_BLOCK_H_
