#include "pending_interest_table.h"

namespace pec {

bool PendingInterestTable::Exist(int nonce) {
  return table_.find(nonce) != table_.end();
}

void PendingInterestTable::AddInterest(Interest interest) {
  table_.insert(std::pair<int, Interest>(interest.nonce(), interest));
}

void PendingInterestTable::UpdateInterest(Interest interest) {
  table_.find(interest.nonce())->second = interest;
}

std::set<int> PendingInterestTable::GetIrredundantMetadata(std::set<int> metadata) {
  std::set<int> irredundant_metadata;
  for (std::set<int>::iterator m_iter = metadata.begin(); 
       m_iter != metadata.end(); ++m_iter) {
    for (std::map<int, Interest>::iterator i_iter = table_.begin(); 
         i_iter != table_.end(); ++i_iter) {
      if (!i_iter->second.HasMetadata(*m_iter)) {
        irredundant_metadata.insert(*m_iter);
        break;
      }
    }
  }
  return irredundant_metadata;
}

void PendingInterestTable::AddMetadataToAll(std::set<int> metadata) {
  for (std::set<int>::iterator m_iter = metadata.begin(); 
       m_iter != metadata.end(); ++m_iter) {
    for (std::map<int, Interest>::iterator i_iter = table_.begin(); 
         i_iter != table_.end(); ++i_iter) {
      if (!i_iter->second.HasMetadata(*m_iter)) {
        i_iter->second.AddMetadata(*m_iter);
      }
    }
  }
}

} // namespace pec