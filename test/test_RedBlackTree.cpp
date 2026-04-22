//
// Created by ilya on 09.04.2026.
//

#include "gtest.h"
#include "RedBlackTree.h"
#include "polynomial.h"

TEST(RedBlackTreeTest, CanCreateEmptyTree) {
    RedBlackTree<int, int> tree;
    EXPECT_EQ(tree.find(10), nullptr);
}

TEST(RedBlackTreeTest, CanInsertAndFindOneElement) {
    RedBlackTree<int, int> tree;
    tree.insert(5, 100);

    auto node = tree.find(5);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value, 100);
    EXPECT_EQ(node->key, 5);
}

TEST(RedBlackTreeTest, CanInsertAndFindMultipleElements) {
    RedBlackTree<int, int> tree;
    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(5, 50);

    auto node1 = tree.find(10);
    auto node2 = tree.find(20);
    auto node3 = tree.find(5);
    
    ASSERT_NE(node1, nullptr);
    EXPECT_EQ(node1->value, 100);

    ASSERT_NE(node2, nullptr);
    EXPECT_EQ(node2->value, 200);

    ASSERT_NE(node3, nullptr);
    EXPECT_EQ(node3->value, 50);
}

TEST(RedBlackTreeTest, FindReturnsNullForMissingKey) {
    RedBlackTree<int, int> tree;
    tree.insert(10, 100);
    tree.insert(20, 200);

    EXPECT_EQ(tree.find(30), nullptr);
}

TEST(RedBlackTreeTest, CanEraseLeafNode) {
    RedBlackTree<int, int> tree;
    tree.insert(10, 100);
    tree.insert(20, 200);

    auto node = tree.find(20);
    ASSERT_NE(node, nullptr);

    tree.erase(node);

    EXPECT_EQ(tree.find(20), nullptr);
    EXPECT_NE(tree.find(10), nullptr);
}

TEST(RedBlackTreeTest, CanEraseNodeWithChildren) {
    RedBlackTree<int, int> tree;
    tree.insert(10, 100);
    tree.insert(5, 50);
    tree.insert(20, 200);
    tree.insert(15, 150);
    tree.insert(25, 250);

    auto node = tree.find(20);
    ASSERT_NE(node, nullptr);

    tree.erase(node);

    EXPECT_EQ(tree.find(20), nullptr);
    EXPECT_NE(tree.find(10), nullptr);
    EXPECT_NE(tree.find(5), nullptr);
    EXPECT_NE(tree.find(15), nullptr);
    EXPECT_NE(tree.find(25), nullptr);
}

TEST(RedBlackTreeTest, RootIsAlwaysBlack) {
    RedBlackTree<int, int> tree;
    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(30, 300); // Это должно вызвать балансировку

    auto rootNode = tree.find(20); // 20 должно стать корнем после вращения
    ASSERT_NE(rootNode, nullptr);
    auto c = RedBlackTree<int, int>::Color::BLACK;
    EXPECT_EQ(rootNode->color, c);
}

TEST(RedBlackTreeTest, CanStorePolynomialValues) {
    RedBlackTree<int, Polynomial> tree;

    Polynomial p1;
    p1.add_monom(2, 1, 0, 0); // 2x
    p1.add_monom(3, 0, 1, 0); // 3y

    Polynomial p2;
    p2.add_monom(5, 1, 0, 0); // 5x
    p2.add_monom(-3, 0, 1, 0); // -3y

    tree.insert(1, p1);
    tree.insert(2, p2);

    auto node1 = tree.find(1);
    ASSERT_NE(node1, nullptr);
    EXPECT_EQ(node1->value.to_string(), "2*x + 3*y");

    auto node2 = tree.find(2);
    ASSERT_NE(node2, nullptr);
    EXPECT_EQ(node2->value.to_string(), "5*x - 3*y");

    // Test updating the value
    Polynomial p3 = p1 + p2;
    tree.insert(1, p3);

    auto updatedNode = tree.find(1);
    ASSERT_NE(updatedNode, nullptr);
    EXPECT_EQ(updatedNode->value.to_string(), "7*x");
}

TEST(RedBlackTreeTest, DictionaryOfPolynomials) {
    RedBlackTree<std::string, Polynomial> tree;

    Polynomial p1;
    p1.add_monom(1, 2, 0, 0); // x^2
    p1.add_monom(2, 1, 1, 0); // 2xy
    p1.add_monom(1, 0, 2, 0); // y^2

    Polynomial p2;
    p2.add_monom(1, 2, 0, 0); // x^2
    p2.add_monom(-2, 1, 1, 0); // -2xy
    p2.add_monom(1, 0, 2, 0); // y^2

    tree.insert("SquareSum", p1);
    tree.insert("SquareDiff", p2);

    auto nodeSum = tree.find("SquareSum");
    ASSERT_NE(nodeSum, nullptr);

    auto nodeDiff = tree.find("SquareDiff");
    ASSERT_NE(nodeDiff, nullptr);

    Polynomial p3 = nodeSum->value + nodeDiff->value;
    tree.insert("SumOfSquares", p3);

    auto nodeRes = tree.find("SumOfSquares");
    ASSERT_NE(nodeRes, nullptr);
    EXPECT_EQ(nodeRes->value.to_string(), "2*x^2 + 2*y^2");

    tree.erase(nodeDiff);
    EXPECT_EQ(tree.find("SquareDiff"), nullptr);
}

 
