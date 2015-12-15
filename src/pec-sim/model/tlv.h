#ifndef PEC_TLV_H_
#define PEC_TLV_H_

#include <algorithm>

namespace pec {

typedef uint32_t TlvLength;

enum TlvType {
  kUndefined = 0,

  kTlvInterest = 1,
  kTlvData = 2,
};

const uint32_t kTlvTypeSize = sizeof(TlvType);
const uint32_t kTlvLengthSize = sizeof(TlvLength);
const uint32_t kTlvTypeLengthSize = kTlvTypeSize + kTlvLengthSize;


inline TlvType PeekType(const uint8_t *buffer, const uint32_t buffer_size) {
  TlvType type = kUndefined;
  std::copy(buffer, buffer + kTlvTypeSize, (uint8_t *)&type);
  return type;
}

} // namespace pec

#endif // PEC_TLV_H_