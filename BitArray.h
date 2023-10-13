#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <gtest\gtest.h>

class BitArray {

public:
    int num_bits;
    unsigned long* bits;
    BitArray() : bits(nullptr), num_bits(0) {}
    ~BitArray();

    explicit BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& b);

    void swap(BitArray& b);

    BitArray& operator=(const BitArray& b);

    void resize(int num_bits, bool value = false);
    void clear();
    void push_back(bool bit);

    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);

    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    BitArray& set(int n, bool val = true);
    BitArray& set();

    BitArray& reset(int n);
    BitArray& reset();

    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;

    bool operator[](int i) const;

    int size() const;
    bool empty() const;

    std::string to_string() const;
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

// Implementation of the BitArray class

// Implementation of a constructor with explicit specification of the number of bits and default value
BitArray::BitArray(int num_bits, unsigned long value) {
    this->num_bits = num_bits;
    int num_long = (num_bits + sizeof(unsigned long) - 1) / sizeof(unsigned long);

    bits = new unsigned long[num_long];

    // Initialise the first sizeof(unsigned long) bits with values from the value parameter
    for (int i = 0; i < num_long; i++) {
        bits[i] = (i == 0) ? (value & (ULONG_MAX >> (sizeof(unsigned long) * CHAR_BIT - num_bits))) : 0;
        num_bits -= sizeof(unsigned long) * CHAR_BIT;
    }
}

BitArray::~BitArray() {
    delete[] bits;
}

// Implementation of the copy constructor
BitArray::BitArray(const BitArray& b) {
    num_bits = b.num_bits;
    int num_long = (num_bits + sizeof(unsigned long) - 1) / sizeof(unsigned long);

    bits = new unsigned long[num_long];

    // Copy bit values from the b.bits array to the current object
    for (int i = 0; i < num_long; i++) {
        bits[i] = b.bits[i];
    }
}

void BitArray::swap(BitArray& b) {
    // Changing the values of the number of bits
    std::swap(num_bits, b.num_bits);

    // Changing bit array values
    std::swap(bits, b.bits);
}

BitArray& BitArray::operator=(const BitArray& b) {
    if (this == &b) {
        return *this;  // Self-appropriation check
    }

    delete[] bits;  // Free the memory of the current object

    num_bits = b.num_bits;
    int num_long = (num_bits + sizeof(unsigned long) - 1) / sizeof(unsigned long);

    bits = new unsigned long[num_long];

    // Copy bit values from the b.bits array to the current object
    for (int i = 0; i < num_long; i++) {
        bits[i] = b.bits[i];
    }

    return *this;
}

void BitArray::resize(int num_bits, bool value) {
    if (num_bits == this->num_bits) {
        return;  // We don't do anything, the size already matches
    }

    int num_long = (num_bits + sizeof(unsigned long) - 1) / sizeof(unsigned long);

    unsigned long* new_bits = new unsigned long[num_long];

    // Fill new elements with value
    for (int i = this->num_bits; i < num_bits; i++) {
        set(i, value);
    }

    // Copy bit values from the old array to the new array
    int num_copy = std::min(this->num_bits, num_bits);
    std::memcpy(new_bits, bits, num_copy * sizeof(unsigned long));

    delete[] bits;  // Free the memory of the old array

    bits = new_bits;
    this->num_bits = num_bits;
}

void BitArray::clear() {
    std::memset(bits, 0, num_bits * sizeof(unsigned long));
}

void BitArray::push_back(bool bit) {
    resize(num_bits + 1, false);  // Expand the array by 1 bit, filling the new element with the value false
    set(num_bits - 1, bit);  // Set the value of the last bit
}

BitArray& BitArray::operator&=(const BitArray& b) {
    int num_long = std::min(num_bits, b.num_bits);

    for (int i = 0; i < num_long; i++) {
        bits[i] &= b.bits[i];
    }

    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    int num_long = std::min(num_bits, b.num_bits);

    for (int i = 0; i < num_long; i++) {
        bits[i] |= b.bits[i];
    }

    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    int num_long = std::min(num_bits, b.num_bits);

    for (int i = 0; i < num_long; i++) {
        bits[i] ^= b.bits[i];
    }

    return *this;
}

BitArray& BitArray::operator<<=(int n)
{
    if (n >= num_bits) {
        clear();
    }
    else if (n > 0) {
        int num_blocks = (num_bits + sizeof(unsigned long) * 8 - 1) / (sizeof(unsigned long) * 8);
        int shift_blocks = n / (sizeof(unsigned long) * 8);
        int shift_bits = n % (sizeof(unsigned long) * 8);
        for (int i = num_blocks - 1; i >= shift_blocks; i--) {
            bits[i] = (bits[i - shift_blocks] << shift_bits);
            if (i - shift_blocks - 1 >= 0) {
                bits[i] |= (bits[i - shift_blocks - 1] >> (sizeof(unsigned long) * 8 - shift_bits));
            }
        }
        for (int i = 0; i < shift_blocks; i++) {
            bits[i] = 0UL;
        }
        if (shift_bits > 0) {
            bits[num_blocks - 1] &= ~(~0UL << (num_bits % (sizeof(unsigned long) * 8)));
        }
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n)
{
    if (n >= num_bits) {
        clear();
    }
    else if (n > 0) {
        int num_blocks = (num_bits + sizeof(unsigned long) * 8 - 1) / (sizeof(unsigned long) * 8);
        int shift_blocks = n / (sizeof(unsigned long) * 8);
        int shift_bits = n % (sizeof(unsigned long) * 8);
        for (int i = 0; i < num_blocks - shift_blocks; i++) {
            bits[i] = (bits[i + shift_blocks] >> shift_bits);
            if (i + shift_blocks + 1 < num_blocks) {
                bits[i] |= (bits[i + shift_blocks + 1] << (sizeof(unsigned long) * 8 - shift_bits));
            }
        }
        for (int i = num_blocks - shift_blocks; i < num_blocks; i++) {
            bits[i] = 0UL;
        }
        if (shift_bits > 0) {
            bits[0] &= ~(~0UL << shift_bits);
        }
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool val) {
    if (n >= 0 && n < num_bits) {
        int index = n / (sizeof(unsigned long) * 8);
        int bit_pos = n % (sizeof(unsigned long) * 8);

        if (val) {
            bits[index] |= (1UL << bit_pos);
        }
        else {
            bits[index] &= ~(1UL << bit_pos);
        }
    }

    return *this;
}

BitArray& BitArray::set() {
    for (int i = 0; i < num_bits; i++) {
        set(i, true);
    }

    return *this;
}

BitArray& BitArray::reset(int n) {
    set(n, false);
    return *this;
}

BitArray& BitArray::reset() {
    for (int i = 0; i < num_bits; i++) {
        set(i, false);
    }

    return *this;
}

bool BitArray::any() const
{
    for (int i = 0; i < num_bits; i++) {
        if ((*this)[i]) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    BitArray result(*this); // Create a copy of the current object

    // Invert every bit in the copy
    for (int i = 0; i < num_bits; i++) {
        result.bits[i / (sizeof(unsigned long) * 8)] ^= (1UL << (i % (sizeof(unsigned long) * 8)));
    }

    return result;
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < num_bits; i++) {
        if (bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))) {
            count++;
        }
    }
    return count;
}

bool BitArray::operator[](int i) const
{
    if (i < 0 || i >= num_bits) {
        throw std::out_of_range("Index out of range");
    }
    int index = i / (sizeof(unsigned long) * 8);
    int bit_position = i % (sizeof(unsigned long) * 8);
    return (bits[index] >> bit_position) & 1;
}

int BitArray::size() const {
    return num_bits;
}

bool BitArray::empty() const {
    return num_bits == 0;
}

std::string BitArray::to_string() const
{
    std::string result;
    for (int i = num_bits - 1; i >= 0; i--) {
        result += ((*bits >> i) & 1) ? '1' : '0';
    }
    return result;
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.num_bits != b.num_bits) {
        return false;
    }

    for (int i = 0; i < a.num_bits; i++) {
        if ((a.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))) !=
            (b.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8))))) {
            return false;
        }
    }

    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1.num_bits);

    for (int i = 0; i < b1.num_bits; i++) {
        result.bits[i / (sizeof(unsigned long) * 8)] |= ((b1.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))) & (b2.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))));
    }

    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1.num_bits);

    for (int i = 0; i < b1.num_bits; i++) {
        result.bits[i / (sizeof(unsigned long) * 8)] |=
            ((b1.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))) |
                (b2.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))));
    }

    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1.num_bits);

    for (int i = 0; i < b1.num_bits; i++) {
        result.bits[i / (sizeof(unsigned long) * 8)] |=
            ((b1.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))) ^
                (b2.bits[i / (sizeof(unsigned long) * 8)] & (1UL << (i % (sizeof(unsigned long) * 8)))));
    }

    return result;
}
