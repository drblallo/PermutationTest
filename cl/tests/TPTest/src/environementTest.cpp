//
// Created by massimo on 18/03/18.
//

#include "gtest/gtest.h"
#include "environement.hpp"

using namespace pt;
TEST(basic_check, test_environement_set_up)
{
    EXPECT_EQ(Environement::initExecuted(), false);

    ContextList contextList;
    EXPECT_EQ(contextList.size(), 0);
    Environement::initEnvironement(contextList);
    EXPECT_EQ(Environement::initExecuted(), false);

    contextList.push_back(std::vector<int>());
    contextList[0].push_back(0);

	EXPECT_EQ(Environement::getContextDevQue() == NULL, true);
    EXPECT_EQ(Environement::getContextSize(), -1);
    EXPECT_EQ(Environement::getPlatforms() == NULL, true);
    EXPECT_EQ(Environement::getQueuesSize(), -1);

    Environement::initEnvironement(contextList);
    EXPECT_EQ(Environement::initExecuted(), true);

	EXPECT_EQ(Environement::getContextDevQue()->size(), 1);
	EXPECT_EQ(Environement::getContextSize(), 1);
	EXPECT_EQ(Environement::getPlatforms()->size(), 1);
	EXPECT_EQ(Environement::getQueuesSize(), 1);

	EXPECT_EQ(Environement::getContextDevQue()->at(0).getDevQueueSize(), 1);
    Environement::clearEnvironement();
    EXPECT_EQ(Environement::initExecuted(), false);
}

TEST(basic_check, test_print_all_devices)
{
    Environement::printAllDevices();
    EXPECT_EQ(0, 0);
}
