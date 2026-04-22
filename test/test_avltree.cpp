#include "gtest.h"
#include "AVLtree.h"

class AVLTreeTest : public ::testing::Test {
protected:
    AVLTree<int, int> tree;

    void TearDown() override {
        tree.clear();
    }
};

TEST_F(AVLTreeTest, InsertAndFind) {
    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(5, 50);

    const int* val1 = tree.find(10);
    ASSERT_NE(val1, nullptr);
    EXPECT_EQ(*val1, 100);

    const int* val2 = tree.find(20);
    ASSERT_NE(val2, nullptr);
    EXPECT_EQ(*val2, 200);

    const int* val3 = tree.find(5);
    ASSERT_NE(val3, nullptr);
    EXPECT_EQ(*val3, 50);

    EXPECT_EQ(tree.find(15), nullptr);
}

TEST_F(AVLTreeTest, UpdateExistingKey) {
    tree.insert(1, 10);
    tree.insert(1, 20);

    const int* val = tree.find(1);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 20);
}

TEST_F(AVLTreeTest, EraseExistingAndMissingKey) {
    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(5, 50);

    EXPECT_TRUE(tree.erase(20));
    EXPECT_EQ(tree.find(20), nullptr);

    EXPECT_FALSE(tree.erase(15));
}

TEST_F(AVLTreeTest, ClearAndEmpty) {
    EXPECT_TRUE(tree.empty());

    tree.insert(1, 10);
    EXPECT_FALSE(tree.empty());

    tree.clear();
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.find(1), nullptr);
}

TEST_F(AVLTreeTest, IteratorReturnsSortedKeys) {
    tree.insert(3, 30);
    tree.insert(1, 10);
    tree.insert(4, 40);
    tree.insert(2, 20);

    int expectedKey = 1;
    int count = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        auto [k, v] = *it;
        EXPECT_EQ(k, expectedKey);
        EXPECT_EQ(v, expectedKey * 10);
        ++expectedKey;
        ++count;
    }
    EXPECT_EQ(count, 4);
}

TEST_F(AVLTreeTest, InsertPatternsTriggerRotationsButPreserveSearch) {
    // LL
    tree.insert(30, 300);
    tree.insert(20, 200);
    tree.insert(10, 100);

    // RR
    tree.insert(40, 400);
    tree.insert(50, 500);

    // LR
    tree.insert(25, 250);

    // RL
    tree.insert(45, 450);

    for (int k : {10, 20, 25, 30, 40, 45, 50}) {
        const int* v = tree.find(k);
        ASSERT_NE(v, nullptr);
        EXPECT_EQ(*v, k * 10);
    }
}

TEST_F(AVLTreeTest, EraseKeepsTreeSearchableAcrossCases) {
    for (int k : {20, 10, 30, 5, 15, 25, 35, 13, 17}) tree.insert(k, k);

    EXPECT_TRUE(tree.erase(5));
    EXPECT_TRUE(tree.erase(15));
    EXPECT_TRUE(tree.erase(30));

    EXPECT_EQ(tree.find(5), nullptr);
    EXPECT_EQ(tree.find(15), nullptr);
    EXPECT_EQ(tree.find(30), nullptr);

    for (int k : {10, 13, 17, 20, 25, 35}) {
        const int* v = tree.find(k);
        ASSERT_NE(v, nullptr);
        EXPECT_EQ(*v, k);
    }
}
