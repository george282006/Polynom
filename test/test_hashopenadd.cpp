#include "gtest.h"
#include "HashOpenAddress.h"
#include <set>


TEST(HashOpenAddressTest, ConstructorDefault) {
    HashOpenAddress<int, std::string> hash;
    ASSERT_TRUE(hash.empty());
    ASSERT_EQ(hash.size(), 0);
}

TEST(HashOpenAddressTest, InsertSingleElement) {
    HashOpenAddress<int, std::string> hash;
    hash.insert(1, "one");
    ASSERT_EQ(hash.size(), 1);
}

TEST(HashOpenAddressTest, InsertMultipleElements) {
    HashOpenAddress<int, int> hash;
    for (int i = 0; i < 20; ++i) {
        hash.insert(i, i * 10);
    }
    ASSERT_EQ(hash.size(), 20);
}

TEST(HashOpenAddressTest, InsertDuplicateKeyUpdate) {
    HashOpenAddress<int, std::string> hash;
    hash.insert(1, "one");
    hash.insert(1, "ONE");
    ASSERT_EQ(hash.size(), 1);
    ASSERT_EQ(*hash.find(1), "ONE");
}

TEST(HashOpenAddressTest, InsertWithCollisions) {
    HashOpenAddress<int, int> hash(4);
    hash.insert(1, 100);
    hash.insert(5, 500);
    hash.insert(9, 900);
    ASSERT_EQ(hash.size(), 3);
}


TEST(HashOpenAddressTest, OperatorSquareBracketInsert) {
    HashOpenAddress<int, int> hash;
    hash[1] = 100;
    hash[2] = 200;
    ASSERT_EQ(hash[1], 100);
    ASSERT_EQ(hash[2], 200);
}

TEST(HashOpenAddressTest, OperatorSquareBracketUpdate) {
    HashOpenAddress<int, int> hash;
    hash[1] = 10;
    hash[1] = 20;
    ASSERT_EQ(hash.size(), 1);
    ASSERT_EQ(hash[1], 20);
}



TEST(HashOpenAddressTest, FindExistingElement) {
    HashOpenAddress<int, int> hash;
    hash.insert(1, 100);
    hash.insert(2, 200);
    ASSERT_NE(hash.find(1), nullptr);
    ASSERT_EQ(*hash.find(1), 100);
}

TEST(HashOpenAddressTest, FindNonExistentElement) {
    HashOpenAddress<int, int> hash;
    hash.insert(1, 100);
    ASSERT_EQ(hash.find(999), nullptr);
}



TEST(HashOpenAddressTest, AtExistingElement) {
    HashOpenAddress<int, int> hash;
    hash.insert(1, 100);
    ASSERT_EQ(hash.at(1), 100);
}

TEST(HashOpenAddressTest, AtNonExistentThrows) {
    HashOpenAddress<int, int> hash;
    ASSERT_THROW(hash.at(1), std::out_of_range);
}


TEST(HashOpenAddressTest, EraseSingleElement) {
    HashOpenAddress<int, int> hash;
    hash.insert(1, 100);
    ASSERT_TRUE(hash.erase(1));
    ASSERT_EQ(hash.size(), 0);
}

TEST(HashOpenAddressTest, EraseNonExistent) {
    HashOpenAddress<int, int> hash;
    ASSERT_FALSE(hash.erase(999));
}

TEST(HashOpenAddressTest, EraseMultipleElements) {
    HashOpenAddress<int, int> hash;
    for (int i = 1; i <= 10; ++i) {
        hash.insert(i, i * 10);
    }
    for (int i = 1; i <= 5; ++i) {
        hash.erase(i);
    }
    ASSERT_EQ(hash.size(), 5);
}

TEST(HashOpenAddressTest, EraseTriggerRehash) {
    HashOpenAddress<int, int> hash(4);
    for (int i = 1; i <= 8; ++i) {
        hash.insert(i, i * 10);
    }
    for (int i = 1; i <= 4; ++i) {
        hash.erase(i);
    }
    ASSERT_EQ(hash.size(), 4);
    for (int i = 5; i <= 8; ++i) {
        ASSERT_NE(hash.find(i), nullptr);
    }
}

TEST(HashOpenAddressTest, ClearEmptyTable) {
    HashOpenAddress<int, int> hash;
    hash.clear();
    ASSERT_TRUE(hash.empty());
}

TEST(HashOpenAddressTest, ClearNonEmptyTable) {
    HashOpenAddress<int, int> hash;
    hash.insert(1, 10);
    hash.insert(2, 20);
    hash.clear();
    ASSERT_EQ(hash.size(), 0);
    ASSERT_EQ(hash.find(1), nullptr);
}


TEST(HashOpenAddressTest, SizeEmpty) {
    HashOpenAddress<int, int> hash;
    ASSERT_EQ(hash.size(), 0);
    ASSERT_TRUE(hash.empty());
}

TEST(HashOpenAddressTest, SizeAfterOperations) {
    HashOpenAddress<int, int> hash;
    hash.insert(1, 10);
    hash.insert(2, 20);
    ASSERT_EQ(hash.size(), 2);
    hash.erase(1);
    ASSERT_EQ(hash.size(), 1);
}



TEST(HashOpenAddressTest, StringKeyIntValue) {
    HashOpenAddress<std::string, int> hash;
    hash.insert("apple", 1);
    hash.insert("banana", 2);
    ASSERT_EQ(*hash.find("apple"), 1);
    ASSERT_EQ(*hash.find("banana"), 2);
}


TEST(HashOpenAddressStress, InsertLargeNumber) {
    HashOpenAddress<int, int> hash;
    const int N = 10000;

    for (int i = 0; i < N; ++i) {
        hash.insert(i, i * 2);
    }

    ASSERT_EQ(hash.size(), N);

    for (int i = 0; i < 100; ++i) {
        int idx = (i * 1247) % N;
        const int* val = hash.find(idx);
        ASSERT_NE(val, nullptr);
        ASSERT_EQ(*val, idx * 2);
    }
}

TEST(HashOpenAddressStress, InsertEraseRandom) {
    HashOpenAddress<int, int> hash;
    const int N = 5000;
    std::set<int> present;

    for (int i = 0; i < N; ++i) {
        hash.insert(i, i * 10);
        present.insert(i);
    }

    int removed = 0;
    for (int idx = 0; idx < N && removed < N / 2; idx += 2) {
        hash.erase(idx);
        present.erase(idx);
        removed++;
    }

    ASSERT_EQ(hash.size(), N / 2);

    for (int key : present) {
        const int* val = hash.find(key);
        ASSERT_NE(val, nullptr);
        ASSERT_EQ(*val, key * 10);
    }
}

TEST(HashOpenAddressStress, MultipleRehashes) {
    HashOpenAddress<int, int> hash(2);
    const int N = 500;

    for (int i = 0; i < N; ++i) {
        hash[i] = i * i;
    }

    ASSERT_EQ(hash.size(), N);

    for (int i = 0; i < N; ++i) {
        const int* val = hash.find(i);
        ASSERT_NE(val, nullptr);
        ASSERT_EQ(*val, i * i);
    }
}

TEST(HashOpenAddressStress, HeavyCollisions) {
    HashOpenAddress<int, int> hash(8);

    for (int i = 0; i < 100; ++i) {
        hash.insert(i * 8, i);
    }

    ASSERT_EQ(hash.size(), 100);

    for (int i = 0; i < 100; ++i) {
        const int* val = hash.find(i * 8);
        ASSERT_NE(val, nullptr);
        ASSERT_EQ(*val, i);
    }
}

TEST(HashOpenAddressStress, AlternatingInsertErase) {
    HashOpenAddress<int, int> hash;
    const int N = 2000;

    for (int i = 0; i < N; ++i) {
        hash.insert(i, i);
        if (i > 100) {
            hash.erase(i - 100);
        }
    }

    ASSERT_EQ(hash.size(), 100);
}

TEST(HashOpenAddressStress, UpdateExistingElements) {
    HashOpenAddress<int, int> hash;
    const int N = 1000;

    for (int i = 0; i < N; ++i) {
        hash.insert(i, 0);
    }

    for (int round = 1; round <= 10; ++round) {
        for (int i = 0; i < N; ++i) {
            hash.insert(i, hash.at(i) + 1);
        }
    }

    ASSERT_EQ(hash.size(), N);

    for (int i = 0; i < N; ++i) {
        const int* val = hash.find(i);
        ASSERT_EQ(*val, 10);
    }
}

TEST(HashOpenAddressStress, MixedOperations) {
    HashOpenAddress<int, int> hash;
    const int N = 1000;

    for (int i = 0; i < N; ++i) {
        hash[i] = i;
    }

    for (int i = 0; i < N; i += 2) {
        hash[i] = i * 2;
    }

    for (int i = 0; i < N; i += 3) {
        hash.erase(i);
    }

    for (int i = 0; i < N; i += 3) {
        hash.insert(i + 10000, i);
    }

    for (int i = 0; i < N; ++i) {
        auto val = hash.find(i);
        if (i % 3 == 0) {
            ASSERT_EQ(val, nullptr);
        } else {
            ASSERT_NE(val, nullptr);
        }
    }
}

