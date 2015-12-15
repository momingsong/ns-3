#ifndef PEC_BLOCK_H_
#define PEC_BLOCK_H_

#include <cstdlib>

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

 protected:
  void Reset();
  virtual void Encode() = 0;
  virtual void Decode() = 0;

  uint32_t wire_length_;
  uint8_t *wire_begin_;
  bool has_wire_;
};

} // namespace pec

#endif // PEC_BLOCK_H_