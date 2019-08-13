//
// Created by guido on 13/08/2019.
//

#include <gtest/gtest.h>

#include "../Cell.h"

TEST(Cell, DefaultConstructor) {
    Cell c;

    ASSERT_EQ(c.getX(), 0);
    ASSERT_EQ(c.getY(), 0);
    ASSERT_EQ(c.getValue(), 0);
    ASSERT_EQ(c.getParent(), nullptr);
    ASSERT_EQ(c.isVisited(), false);

}

TEST(Cell, Constructor) {
    Cell c(5, -2, 1, nullptr, true);

    ASSERT_EQ(c.getX(), 5);
    ASSERT_EQ(c.getY(), 0);
    ASSERT_EQ(c.getValue(), 1);
    ASSERT_EQ(c.getParent(), nullptr);
    ASSERT_EQ(c.isVisited(), true);

}