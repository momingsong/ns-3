#ifndef PEC_MESSAGE_RECEIVER_INTERFACE_H_
#define PEC_MESSAGE_RECEIVER_INTERFACE_H_

#include "interest.h"
#include "data.h"

namespace pec {

class MessageReceiverInterface {
 public:
  virtual void ReceiveInterest(Interest interest) = 0;
  virtual void ReceiveData(Data data) = 0;
};

} // namespace pec

#endif // PEC_MESSAGE_RECEIVER_INTERFACE_H_