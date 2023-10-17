#include <gtest\gtest.h>
#include "BitArray.h"

TEST(BitArrayTest, ConstructorWithValue) {
    BitArray arr(8, 0b10101010);
    EXPECT_EQ(8, arr.size());
    EXPECT_FALSE(arr.empty());
    EXPECT_EQ("10101010", arr.to_string());
}

TEST(BitArrayTest, CopyConstructor) {
    BitArray arr1(8, 0b10101010);
    BitArray arr2(arr1);
    EXPECT_EQ(8, arr2.size());
    EXPECT_FALSE(arr2.empty());
    EXPECT_EQ("10101010", arr2.to_string());
}

TEST(BitArrayTest, AssignmentOperator) {
    BitArray arr1(8, 0b10101010);
    BitArray arr2;
    arr2 = arr1;
    EXPECT_EQ(8, arr2.size());
    EXPECT_FALSE(arr2.empty());
    EXPECT_EQ("10101010", arr2.to_string());
}

TEST(BitArrayTest, empty) {
    BitArray b1(8);
    EXPECT_EQ(b1.to_string(), "00000000");
}

TEST(BitArrayTest, setTrue) {
    BitArray b1(8);
    b1.set(0, true);  // Set the first bit to true
    EXPECT_EQ(b1.to_string(), "00000001");
}

TEST(BitArrayTest, setFalse) {
    BitArray b1(8);
    b1.set(0, true);  // Set the first bit to true
    b1.set(0, false);  // Set the first bit to false
    EXPECT_EQ(b1.to_string(), "00000000");
}

TEST(BitArrayTest, swap) {
    BitArray a, c(35, 234563456), b(c);

    a.swap(c);

    EXPECT_EQ(a.to_string(), b.to_string());
    EXPECT_EQ("", c.to_string());
}

TEST(BitArrayTest, operAND) {
    BitArray b1(8);  // Create BitArray object with 8 bits
    b1.set(0, true);  // Set the first bit to true
    b1.set(3, true);  // Set the fourth bit to true
    BitArray b2(8);  // Create BitArray object with 8 bits
    b2.set(2, true);  // Set the third bit to true
    b2.set(3, true);  // Set the fourth bit to true
    BitArray b3 = b1 & b2;  // Perform bitwise AND operation between b1 and b2
    EXPECT_EQ(b3.to_string(), "00001000");
}

TEST(BitArrayTest, operOR) {
    BitArray b1(8);  // Create BitArray object with 8 bits
    b1.set(0, true);  // Set the first bit to true
    b1.set(3, true);  // Set the fourth bit to true
    BitArray b2(8);  // Create BitArray object with 8 bits
    b2.set(2, true);  // Set the third bit to true
    b2.set(3, true);  // Set the fourth bit to true
    BitArray b4 = b1 | b2;  // Perform bitwise OR operation between b1 and b2
    EXPECT_EQ(b4.to_string(), "00001101");
}

TEST(BitArrayTest, operXOR) {
    BitArray b1(8);  // Create BitArray object with 8 bits
    b1.set(0, true);  // Set the first bit to true
    b1.set(3, true);  // Set the fourth bit to true
    BitArray b2(8);  // Create BitArray object with 8 bits
    b2.set(2, true);  // Set the third bit to true
    b2.set(3, true);  // Set the fourth bit to true
    BitArray b5 = b1 ^ b2;  // Perform bitwise XOR operation between b1 and b2
    EXPECT_EQ(b5.to_string(), "00000101");
}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
