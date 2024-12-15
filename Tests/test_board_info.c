#include "board_info.h"

#include "unity.h"
#include "unity_fixture.h"

#include "mock_hash_adapter.h"

TEST_GROUP(BoardInfo);

TEST_SETUP(BoardInfo) {
}

TEST_TEAR_DOWN(BoardInfo) {
}

TEST_GROUP_RUNNER(BoardInfo) {
    RUN_TEST_CASE(BoardInfo, BoardInfo_getDataJson);
}

TEST(BoardInfo, BoardInfo_getDataJson) {
    {
        HashAdapter_getBase64HashedBoardId_IgnoreAndReturn(true);

        uint8_t buffer[1000];
        bool success = BoardInfo_getDataJson(buffer, 1000);
        TEST_ASSERT_TRUE(success);
    }
}
