#include <gtest/gtest.h>

#include "SimulatedCameraSensor.hpp"


// =============================================================================
TEST(CameraLibTest, ValidInitialization){
    EXPECT_NO_THROW({sensor::SimulatedCameraSensor(sensor::Freq::HZ_10,
                                                  "/ws/data/images");});
}

// =============================================================================
TEST(CameraLibTest, InvalidDirectoryPath){
    EXPECT_THROW(
        {sensor::SimulatedCameraSensor(sensor::Freq::HZ_10,
                                      "/ws/data/donotexist");},
        std::runtime_error);
}

// =============================================================================
TEST(CameraLibTest, LoadValidData){
    sensor::SimulatedCameraSensor camera = 
                    sensor::SimulatedCameraSensor(sensor::Freq::HZ_10,
                                                  "/ws/data/images");
    EXPECT_GT(camera.getHeight() , 0);
    EXPECT_GT(camera.getWidth()  , 0);
    EXPECT_GT(camera.getChannel(), 0);
}

// =============================================================================
TEST(CameraLibTest, LoadEmptyDirectory){
    sensor::SimulatedCameraSensor camera = 
                    sensor::SimulatedCameraSensor(sensor::Freq::HZ_10,
                                                  "/ws/data/no-images");
    EXPECT_EQ(camera.getHeight() , 0);
    EXPECT_EQ(camera.getWidth()  , 0);
    EXPECT_EQ(camera.getChannel(), 0);
}

// =============================================================================
int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
