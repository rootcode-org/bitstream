/* Copyright is waived. No warranty is provided. Unrestricted use and modification is permitted. */

#include "bitstream.h"


BitStream::BitStream(unsigned char *_buffer) {
	buffer = _buffer;
	offset = 0;
}


void BitStream::set_offset(int _offset) {
	offset = _offset;
}


int BitStream::read (int num_bits)
{
	if (num_bits > 32) return 0;

	// Convert the offset to a byte index and a bit number
	int next_bit = offset & 7;
	int index    = offset / 8;
	offset += num_bits;

	// Create a mask for the bit data
	unsigned int bit_mask;
	if (num_bits == 32) bit_mask = 0xffffffff;
	else			    bit_mask = (unsigned int) (1 << num_bits) - 1;

	// Initialize the bit accumulator
	unsigned int accumulator = 0;
	int shift = 0;

	// Read the bit data from memory
	while (bit_mask)
	{
		// Create the mask needed to extract the bit data
		unsigned int mask = bit_mask << next_bit;

		// Read the bit data
		unsigned int data = buffer[index] & mask;
		data >>= next_bit;

		// Write the bits into the accumulator
		accumulator |= data << shift;
		shift += 8 - next_bit;

		// Prepare the mask for the next read
		bit_mask >>= 8 - next_bit;

		// Loop until all bits are read
		next_bit = 0;
		index++;
	}

	return (int) accumulator;
}


void BitStream::write (int num_bits, int bit_data)
{
	if (num_bits > 32) return;

	// Convert the offset to a byte index and a bit number
	int next_bit = offset & 7;
	int index    = offset / 8;
	offset += num_bits;

	// Create a mask for the bit data
	unsigned int bit_mask;
	if (num_bits == 32) bit_mask = 0xffffffff;
	else			    bit_mask = (unsigned int) (1 << num_bits) - 1;

	// Write the bit data to memory
	while (bit_mask)
	{
		// Shift the bit data to the correct position and mask it
		unsigned int data = (unsigned int) bit_data << next_bit;
		unsigned int mask = (unsigned int) bit_mask << next_bit;
		data &= mask;

		// Write the bit data to the buffer
		buffer[index] &= ~mask;
		buffer[index] |= data;

		// Shift out the bits we just wrote
		bit_data >>= 8 - next_bit;
		bit_mask >>= 8 - next_bit;

		// Loop until all bits are written
		next_bit = 0;
		index++;
	}
}
