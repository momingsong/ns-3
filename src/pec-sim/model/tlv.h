#ifndef PEC_TLV_H_
#define PEC_TLV_H_

#include <algorithm>

namespace pec {

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

typedef uint32_t TlvLength;

enum TlvType {
  kUndefined = 0,

  kTlvInterest = 1,
  kTlvData = 2,
  kTlvAck = 3,
  kTlvECData = 4,
  kTlvECAck = 5,
};

inline TlvType PeekType(const uint8_t *buffer, const uint32_t buffer_size) {
  TlvType type = kUndefined;
  std::copy(buffer, buffer + sizeof(TlvType), (uint8_t *)&type);
  return type;
}

} // namespace pec

#endif // PEC_TLV_H_
