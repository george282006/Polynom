#include "gtest.h"
#include "../include/HashTableForward.h"
#include <chrono>
#include <iostream>


TEST(HashTableForwardBasic, CreateEmptyTable) {
    HashTableForward<int, int> table;
    ASSERT_TRUE(table.empty());
    ASSERT_EQ(0, table.size());
}

TEST(HashTableForwardBasic, InsertOneElement) {
    HashTableForward<int, int> table;
    table.insert(5, 50);
    
    ASSERT_EQ(1, table.size());
    ASSERT_FALSE(table.empty());
    
    int val;
    ASSERT_TRUE(table.find(5, val));
    ASSERT_EQ(50, val);
}

TEST(HashTableForwardBasic, InsertThreeElements) {
    HashTableForward<int, int> table;
    
    table.insert(1, 10);
    table.insert(2, 20);
    table.insert(3, 30);
    
    ASSERT_EQ(3, table.size());
    
    int val;
    ASSERT_TRUE(table.find(1, val));
    ASSERT_EQ(10, val);
    ASSERT_TRUE(table.find(2, val));
    ASSERT_EQ(20, val);
    ASSERT_TRUE(table.find(3, val));
    ASSERT_EQ(30, val);
}

TEST(HashTableForwardBasic, UpdateValue) {
    HashTableForward<int, int> table;
    
    table.insert(1, 10);
    table.insert(1, 100);  // Update
    
    ASSERT_EQ(1, table.size());
    
    int val;
    ASSERT_TRUE(table.find(1, val));
    ASSERT_EQ(100, val);
}

TEST(HashTableForwardBasic, FindNonExistent) {
    HashTableForward<int, int> table;
    
    table.insert(1, 10);
    
    int val;
    ASSERT_FALSE(table.find(99, val));
}

TEST(HashTableForwardBasic, EraseElement) {
    HashTableForward<int, int> table;
    
    table.insert(1, 10);
    table.insert(2, 20);
    
    ASSERT_TRUE(table.erase(1));
    ASSERT_EQ(1, table.size());
    
    int val;
    ASSERT_FALSE(table.find(1, val));
    ASSERT_TRUE(table.find(2, val));
}

TEST(HashTableForwardBasic, StringKeys) {
    HashTableForward<std::string, int> table;
    
    table.insert("alice", 100);
    table.insert("bob", 200);
    
    int val;
    ASSERT_TRUE(table.find("alice", val));
    ASSERT_EQ(100, val);
    ASSERT_TRUE(table.find("bob", val));
    ASSERT_EQ(200, val);
}


// ==================== Стресс-тесты ====================

TEST(HashTableForwardStress, Insert100kElements) {
    HashTableForward<long long, long long> table;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nInserting 100,000 elements..." << std::endl;
    
    for (long long i = 0; i < 100000; i++) {
        table.insert(i, i * 2LL);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    ASSERT_EQ(100000, table.size());
    
    std::cout << "✓ Inserted 100,000 elements in " << duration.count() << " ms" << std::endl;
    std::cout << "  Average: " << (double)duration.count() / 100000.0 << " μs/element" << std::endl;
}

TEST(HashTableForwardStress, Insert500kElements) {
    HashTableForward<long long, long long> table;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nInserting 500,000 elements..." << std::endl;
    
    for (long long i = 0; i < 500000; i++) {
        table.insert(i, i * 3LL);
        
        if ((i + 1) % 100000 == 0) {
            std::cout << "  " << (i + 1) / 1000 << "K inserted" << std::endl;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    ASSERT_EQ(500000, table.size());
    
    std::cout << "✓ Inserted 500,000 elements in " << duration.count() << " ms" << std::endl;
    std::cout << "  Average: " << (double)duration.count() / 500000.0 << " μs/element" << std::endl;
    
    // Spot check
    long long val;
    ASSERT_TRUE(table.find(0, val));
    ASSERT_EQ(0, val);
    ASSERT_TRUE(table.find(250000, val));
    ASSERT_EQ(750000, val);
}

TEST(HashTableForwardStress, Insert1MillionElements) {
    HashTableForward<long long, long long> table;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nInserting 1,000,000 elements..." << std::endl;
    
    for (long long i = 0; i < 1000000; i++) {
        table.insert(i, i * 5LL);
        
        if ((i + 1) % 200000 == 0) {
            std::cout << "  " << (i + 1) / 1000000.0 << "M inserted" << std::endl;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    ASSERT_EQ(1000000, table.size());
    
    std::cout << "✓ Inserted 1,000,000 elements in " << duration.count() << " ms" << std::endl;
    std::cout << "  Average: " << (double)duration.count() / 1000000.0 << " μs/element" << std::endl;
}

TEST(HashTableForwardStress, Insert2MillionElements) {
    HashTableForward<long long, long long> table;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nInserting 2,000,000 elements..." << std::endl;
    
    for (long long i = 0; i < 2000000; i++) {
        table.insert(i, i * 7LL);
        
        if ((i + 1) % 500000 == 0) {
            std::cout << "  " << (i + 1) / 1000000.0 << "M inserted" << std::endl;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    ASSERT_EQ(2000000, table.size());
    
    std::cout << "✓ Inserted 2,000,000 elements in " << duration.count() << " ms" << std::endl;
    std::cout << "  Average: " << (double)duration.count() / 2000000.0 << " μs/element" << std::endl;
}

TEST(HashTableForwardStress, Insert4MillionElements) {
    HashTableForward<long long, long long> table;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nInserting 4,000,000 elements..." << std::endl;
    
    for (long long i = 0; i < 4000000; i++) {
        table.insert(i, i * 11LL);
        
        if ((i + 1) % 1000000 == 0) {
            std::cout << "  " << (i + 1) / 1000000.0 << "M inserted" << std::endl;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    ASSERT_EQ(4000000, table.size());
    
    std::cout << "\n✓ Successfully inserted 4,000,000 elements" << std::endl;
    std::cout << "  Total time: " << duration.count() << " ms" << std::endl;
    std::cout << "  Average time per insert: " << (double)duration.count() / 4000000.0 << " μs" << std::endl;
    
    // Spot check
    long long val;
    ASSERT_TRUE(table.find(0, val));
    ASSERT_EQ(0, val);
    ASSERT_TRUE(table.find(1000000, val));
    ASSERT_EQ(11000000, val);
    ASSERT_TRUE(table.find(3999999, val));
    ASSERT_EQ(43999989, val);
}

TEST(HashTableForwardStress, Insert8MillionElements) {
    HashTableForward<long long, long long> table;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::cout << "\nInserting 8,000,000 elements..." << std::endl;
    
    for (long long i = 0; i < 8000000; i++) {
        table.insert(i, i * 13LL);
        
        if ((i + 1) % 1000000 == 0) {
            std::cout << "  " << (i + 1) / 1000000.0 << "M inserted" << std::endl;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    ASSERT_EQ(8000000, table.size());
    
    std::cout << "\n✓ Successfully inserted 8,000,000 elements" << std::endl;
    std::cout << "  Total time: " << duration.count() << " ms" << std::endl;
    std::cout << "  Average time per insert: " << (double)duration.count() / 8000000.0 << " μs" << std::endl;
    
    // Spot check
    long long val;
    ASSERT_TRUE(table.find(0, val));
    ASSERT_EQ(0, val);
    ASSERT_TRUE(table.find(4000000, val));
    ASSERT_EQ(52000000, val);
    ASSERT_TRUE(table.find(7999999, val));
    ASSERT_EQ(103999987, val);
}
