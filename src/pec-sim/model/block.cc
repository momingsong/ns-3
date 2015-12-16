#include "block.h"
#include <algorithm>

namespace pec {

Block::Block() : wire_length_(0), wire_begin_(NULL), has_wire_(false) { }

Block::Block(const Block &block)
  : wire_length_(0),
    wire_begin_(NULL),
    has_wire_(false) { }

Block::~Block() {
  if (wire_begin_ != NULL)
    delete [] wire_begin_;
}

uint32_t Block::GetWireLength() {
  if (!has_wire_)
    Encode();
  return wire_length_;
}

void Block::GetWire(uint8_t *buffer, uint32_t buffer_size) {
  if (!has_wire_)
    Encode();

  if (buffer_size != wire_length_) {
    // TODO: handel the error
  }

  std::copy(wire_begin_, wire_begin_ + wire_length_, buffer);
}

void Block::DecodeFromBuffer(const uint8_t *buffer, uint32_t buffer_size) {
  Reset();

  wire_begin_ = new uint8_t[buffer_size];
  std::copy(buffer, buffer + buffer_size, wire_begin_);
  wire_length_ = buffer_size;
  has_wire_ = true;

  Decode();
}

void Block::Reset() {
  if (wire_begin_ != NULL)
    delete [] wire_begin_;

  wire_begin_ = NULL;
  wire_length_ = 0;
  has_wire_ = false;
}

} // namespace pec