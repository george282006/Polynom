#include "gtest.h"
#include "Ordered_tab.h"
#include "polynomial.h"
#include <string>
#include <vector>

TEST(OrderedTabTest, CanCreateEmptyTable) {
    Ordered_tab<int, std::string> table;
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.size(), 0);
}

TEST(OrderedTabTest, InsertsElementsInSortedOrder) {
    Ordered_tab<int, std::string> table;
    table.insert({30, "thirty"});
    table.insert({10, "ten"});
    table.insert({20, "twenty"});

    std::vector<int> keys;
    for (auto it = table.begin(); it != table.end(); ++it) {
        keys.push_back(it->first);
    }

    ASSERT_EQ(keys.size(), 3u);
    EXPECT_EQ(keys[0], 10);
    EXPECT_EQ(keys[1], 20);
    EXPECT_EQ(keys[2], 30);
}

TEST(OrderedTabTest, InsertDoesNotDuplicateExistingKey) {
    Ordered_tab<int, std::string> table;
    table.insert({1, "one"});
    table.insert({1, "uno"});

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.find(1)->second, "one");
}

TEST(OrderedTabTest, CanFindAndUpdateWithOperatorSquareBrackets) {
    Ordered_tab<int, std::string> table;
    table[2] = "two";
    table[2] = "deux";

    ASSERT_EQ(table.size(), 1);
    auto it = table.find(2);
    ASSERT_NE(it, table.end());
    EXPECT_EQ(it->second, "deux");
}

TEST(OrderedTabTest, CanEraseElement) {
    Ordered_tab<int, std::string> table;
    table.insert({1, "one"});
    table.insert({2, "two"});
    table.insert({3, "three"});

    EXPECT_EQ(table.erase(2), 1u);
    EXPECT_EQ(table.size(), 2);
    EXPECT_EQ(table.find(2), table.end());
    EXPECT_NE(table.find(1), table.end());
    EXPECT_NE(table.find(3), table.end());
}

TEST(OrderedTabTest, EraseMissingKeyReturnsZero) {
    Ordered_tab<int, std::string> table;
    table.insert({1, "one"});

    EXPECT_EQ(table.erase(42), 0u);
    EXPECT_EQ(table.size(), 1);
}

TEST(OrderedTabTest, AtThrowsForMissingKey) {
    Ordered_tab<int, std::string> table;
    EXPECT_THROW(table.at(5), std::out_of_range);
}

TEST(OrderedTabTest, CanStorePolynomialValues) {
    Ordered_tab<std::string, Polynomial> table;

    Polynomial a;
    a.add_monom(2, 1, 0, 0);
    a.add_monom(3, 0, 1, 0);

    Polynomial b;
    b.add_monom(5, 2, 0, 0);

    table.insert({"first", a});
    table.insert({"second", b});

    ASSERT_NE(table.find("first"), table.end());
    ASSERT_NE(table.find("second"), table.end());
    EXPECT_EQ(table.at("first").to_string(), "2*x + 3*y");
    EXPECT_EQ(table.at("second").to_string(), "5*x^2");
}

TEST(OrderedTabTest, CanUseMonomAsKeyForPolynomialLikeStorage) {
    Ordered_tab<Monom, double> table;

    table.insert({Monom(1, 1, 0, 0), 10.0});
    table.insert({Monom(2, 0, 2, 0), 20.0});
    table.insert({Monom(3, 0, 1, 1), 30.0});

    std::vector<Monom> keys;
    for (auto it = table.begin(); it != table.end(); ++it) {
        keys.push_back(it->first);
    }

    ASSERT_EQ(keys.size(), 3u);
    EXPECT_TRUE(keys[0] == Monom(1, 1, 0, 0));
    EXPECT_TRUE(keys[1] == Monom(2, 0, 2, 0));
    EXPECT_TRUE(keys[2] == Monom(3, 0, 1, 1));
}
