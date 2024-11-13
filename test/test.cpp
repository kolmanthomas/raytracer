#include <gtest/gtest.h>

#include "util.hpp"

#include <Eigen/Dense>

TEST(HelloTest, BasicAssertions) {
    seed();
    Eigen::Vector3d v = Eigen::Vector3d::Random() * 5;
    std::cout << v << std::endl;
    std::cout << "End!" << std::endl;
}
