#include "erasure_code.h"

#include <iostream>

namespace pec {

int ErasureCode::block_size_;
double ErasureCode::redundancy_rate_;

void ErasureCode::ComputeBlockNum(uint32_t data_length, int &k, int &m) {
	k = data_length / block_size_ + 1;
	m = (k - 1) * redundancy_rate_ + 1;
	if (k + m > MAX_FRAGMENTS) {
		k = MAX_FRAGMENTS / (1 + redundancy_rate_);
		m = MAX_FRAGMENTS - k;
	}
}

void ErasureCode::Encode(const int k, const int m, const uint8_t *data, 
 		const uint32_t data_length, std::vector<std::vector<uint8_t> > &blocks) {
	int desc = CreateInstance(k, m);
	char **encoded_data;
	char **encoded_parity;
	uint64_t fragment_length;
	liberasurecode_encode(desc, (char *)data, (uint64_t)data_length, &encoded_data, 
		&encoded_parity, &fragment_length);
	blocks.clear();
	for (int i = 0; i < k; ++i) {
		std::vector<uint8_t> v;
		for (unsigned int j = 0; j < fragment_length; ++j) {
			v.push_back((uint8_t)encoded_data[i][j]);
		}
		blocks.push_back(v);
	}
	for (int i = 0; i < m; ++i) {
		std::vector<uint8_t> v;
		for (unsigned int j = 0; j < fragment_length; ++j) {
			v.push_back((uint8_t)encoded_parity[i][j]);
		}
		blocks.push_back(v);
	}
	liberasurecode_encode_cleanup(desc, encoded_data, encoded_parity);
	liberasurecode_instance_destroy(desc);
}

void ErasureCode::Decode(const int k, const int m, 
	const std::vector<std::vector<uint8_t> > &blocks, uint8_t **data,
	uint32_t &data_length) {
  int desc = CreateInstance(k, m);
  char **available_fragments = new char *[blocks.size()];
  for (unsigned int i = 0; i < blocks.size(); ++i) {
  	available_fragments[i] = new char [blocks[i].size()];
  	for (unsigned int j = 0; j < blocks[i].size(); ++j) {
  		available_fragments[i][j] = (char)blocks[i][j];
  	}
  }
  char *out_data;
  uint64_t out_data_len;
  liberasurecode_decode(desc, available_fragments, blocks.size(), 
  	blocks[0].size(), 0, &out_data, &out_data_len);
  for (unsigned int i = 0; i < out_data_len; ++i) {
  	(*data)[i] = (uint8_t)out_data[i];
  }
  data_length = (uint32_t)out_data_len;
  liberasurecode_decode_cleanup(desc, out_data);
  liberasurecode_instance_destroy(desc);
  for (unsigned int i = 0; i < blocks.size(); ++i) {
  	delete [] available_fragments[i];
  }
  delete [] available_fragments;
}

int ErasureCode::CreateInstance(int k, int m) {
	ec_args args;
	args.k = k;
	args.m = m;
	args.hd = m + 1;
	args.w = 8;
	args.ct = CHKSUM_NONE;
	return liberasurecode_instance_create(EC_BACKEND_JERASURE_RS_VAND, &args);
}

} // namespace pec