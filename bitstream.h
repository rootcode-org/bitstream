/* Copyright is waived. No warranty is provided. Unrestricted use and modification is permitted. */

#pragma once

// Read and write bits in a memory buffer
class BitStream {
public:
	explicit BitStream (unsigned char *buffer);
	void set_offset (int offset);
	int  read      (int num_bits);
	void write     (int num_bits, int bit_data);

private:
	unsigned char *buffer;
	int offset;
};
