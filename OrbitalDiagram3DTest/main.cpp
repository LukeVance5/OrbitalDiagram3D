#include "pch.h"
#include "tests/simulationTest.h"
#include "tests/trajectoryTest.h"


int main(int argc, char** argv) {
    // This function initializes the Google Test framework
    ::testing::InitGoogleTest(&argc, argv);

    // This runs all tests discovered by the framework
    return RUN_ALL_TESTS();
}