#ifndef PEC_PENDING_INTEREST_TABLE_H_
#define PEC_PENDING_INTEREST_TABLE_H_

#include <map>
#include <set>

#include "interest.h"

namespace pec {

class PendingInterestTable {
 public:
  bool Exist(int nonce);
  void AddInterest(Interest interest);
  void UpdateInterest(Interest interest);
  std::set<int> GetIrredundantMetadata(std::set<int> metadata);
  void AddMetadataToAll(std::set<int> metadata);

 private:
  std::map<int, Interest> table_; // nonce -> interest
};

} // namespace pec

#endif // PEC_PENDING_INTEREST_TABLE_H_