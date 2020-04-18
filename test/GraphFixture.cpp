//
// Created by Guido on 11/04/2020.
//


#include <gtest//gtest.h>
#include "../Graph.h"

class GraphFixture : public ::testing::Test {
public:
    ~GraphFixture() override {}

protected:
    void TearDown() override {
        delete g;
    }
    void SetUp() override {
        g = new Graph(4,4);
        //percorso con risoluzione certa
        g->setNoBlock();
        g->setStart(0,0);
        g->setEnd(3, 3);
        g->startPathFinding();
        EXPECT_EQ(g->getCellFromIndex(3,3).isVisited(), true);
        EXPECT_EQ(g->getPathSize(), 7);//numero di celle percorse
        //percorso senza risoluzione
        g->setStart(0,0);
        g->setEnd(3,3);
        g->setBlock(0,1);
        g->setBlock(1,0);
        g->startPathFinding();
        EXPECT_EQ(g->getCellFromIndex(3,3).isVisited(), false);
        EXPECT_EQ(g->getPathSize(),0);//numero di celle percorse
    }
    Graph *g;
};

TEST_F(GraphFixture, Dimension) {
    EXPECT_EQ(g->getWidth(),4);
    EXPECT_EQ(g->getHeight(),4);
}