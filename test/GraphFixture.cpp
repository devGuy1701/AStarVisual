//
// Created by Guido on 11/04/2020.
//


#include <gtest//gtest.h>
#include "../Graph.h"

class GraphFixture : public ::testing::Test {
public:
    ~GraphFixture() {}

protected:
    void TearDown() override {
        delete g;
    }
    void SetUp() override {
        g = new Graph(4,4);
        g->setStart(0,0);
        g->printMap();
    }
    Graph *g;
};

TEST_F(GraphFixture, Dimension) {
    EXPECT_EQ(g->getWidth(),4);
    EXPECT_EQ(g->getHeight(),4);
}