#include "ec_data.h"

#include "erasure_code.h"

namespace pec {

std::vector<ECData> ECData::EncodeData(Data &data) {
	uint32_t wire_len = data.GetWireLength();
	uint8_t *wire = new uint8_t[wire_len];
	data.GetWire(wire, wire_len);

	std::vector<std::vector<uint8_t> > blocks;
	ErasureCode::Encode(wire, wire_len, blocks);

	int num = blocks.size();
	std::vector<ECData> ret;
	for (int i = 0; i < num; ++i) {
		ECData ed;
		ed.set_nonce(data.nonce());
		ed.set_hop_nonce(data.hop_nonce());
		ed.set_idx(i);
		ed.set_receivers(data.receivers());
		ed.set_block(blocks[i]);
		ret.push_back(ed);
	}
	delete [] wire;
	return ret;
}

Data ECData::DecodeData(std::vector<ECData> &ec_data) {
	std::vector<std::vector<uint8_t> > blocks;
	for (unsigned int i = 0; i < ec_data.size(); ++i) {
		blocks.push_back(ec_data[i].block());
	}

	uint32_t wire_len = blocks[0].size() * ErasureCode::k();
	uint8_t *wire = new uint8_t[wire_len];
	ErasureCode::Decode(blocks, &wire, wire_len);

	Data data;
	data.DecodeFromBuffer(wire, wire_len);
	// data.set_nonce(ec_data[0].nonce());
	// data.set_hop_nonce(ec_data[0].hop_nonce());
	return data;
}

void ECData::Encode() {
	Reset();

	wire_length_ = sizeof(TlvType) + sizeof(uint32_t) * (1 + receivers_.size()) 
				+ sizeof(int) * 5 + sizeof(uint8_t) * block_.size();
	wire_begin_ = new uint8_t[wire_length_];

	uint8_t *p = wire_begin_;
	*((TlvType *)p) = kTlvECData;
	p += sizeof(TlvType);
	*((uint32_t *)p) = wire_length_;
	p += sizeof(uint32_t);
	*((int *)p) = nonce_;
  p += sizeof(int);
  *((int *)p) = hop_nonce_;
  p += sizeof(int);
  *((int *)p) = idx_;
  p += sizeof(int);
  *((int *)p) = receivers_.size();
  p += sizeof(int);
  std::set<uint32_t>::iterator it = receivers_.begin();
  while (it != receivers_.end()) {
  	*((uint32_t *)p) = *it;
  	p += sizeof(uint32_t);
  	++it;
  }
  *((int *)p) = block_.size();
  p += sizeof(int);
  std::vector<uint8_t>::iterator iter = block_.begin();
  while (iter != block_.end()) {
    *((uint8_t *)p) = *iter;
    p += sizeof(uint8_t);
    ++iter;
  }
}

void ECData::Decode() {
  uint8_t *p = wire_begin_;
  p += sizeof(TlvType);
  p += sizeof(uint32_t);
  nonce_ = *((int *)p);
  p += sizeof(int);
  hop_nonce_ = *((int *)p);
  p += sizeof(int);
  idx_ = *((int *)p);
  p += sizeof(int);
  int num_receivers = *((int *)p);
  p += sizeof(int);
  receivers_.clear();
  for (int i = 0; i < num_receivers; ++i) {
  	receivers_.insert(*((uint32_t *)p));
  	p += sizeof(uint32_t);
  }
  int block_size = *((int *)p);
  p += sizeof(int);
  block_.clear();
  for (int i = 0; i < block_size; ++i) {
  	block_.push_back(*((uint8_t *)p));
  	p += sizeof(uint8_t);
  }
}

} // namespace pec