#ifndef PEC_PENDING_INTEREST_TABLE_H_
#define PEC_PENDING_INTEREST_TABLE_H_

#include <map>
#include <set>

#include "interest.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

namespace pec {

class PendingInterestTable {
 public:
  bool Exist(int nonce);
  void AddInterest(Interest interest);
  void UpdateInterest(Interest interest);
  std::set<int> GetIrredundantMetadata(std::set<int> metadata, std::set<int> &receivers);
  void AddMetadataToAll(std::set<int> metadata);
  std::set<int> GetAllReceivers();

 private:
  std::map<int, Interest> table_; // nonce -> interest
};

} // namespace pec

#endif // PEC_PENDING_INTEREST_TABLE_H_
