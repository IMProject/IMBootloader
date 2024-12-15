#include "binary_update.h"

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(BinaryUpdate);

TEST_SETUP(BinaryUpdate) {
}

TEST_TEAR_DOWN(BinaryUpdate) {
}

TEST_GROUP_RUNNER(BinaryUpdate) {
    RUN_TEST_CASE(BinaryUpdate, BinaryUpdate_handleDetectedBinary);
}

TEST(BinaryUpdate, BinaryUpdate_handleDetectedBinary) {
    {
        signatureType_E detected_binary = signatureType_FIRMWARE_FLASH;
        bool success = BinaryUpdate_handleDetectedBinary(detected_binary);
        TEST_ASSERT_TRUE(success);
    }
}
