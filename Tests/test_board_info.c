#include "board_info.h"

#include "unity.h"
#include "unity_fixture.h"

#include "mock_hash_adapter.h"
#include "mock_json.h"

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
        uint8_t b64_hashed_board_id_str[HASHED_BOARD_ID_SIZE_BASE64_STR] = {0x00};
        HashAdapter_getBase64HashedBoardId_ExpectAnyArgsAndReturn(true);
        HashAdapter_getBase64HashedBoardId_ReturnArrayThruPtr_b64_hashed_board_id_str(b64_hashed_board_id_str, HASHED_BOARD_ID_SIZE_BASE64_STR);

        uint8_t buffer[1000];
        Json_startString_ExpectAndReturn((char*)buffer, sizeof(buffer), true);

        const char key_board_id[] = "board_id";
        Json_addData_ExpectAndReturn((char*)buffer, sizeof(buffer), key_board_id, (char*)b64_hashed_board_id_str, true);

        const char key_manufacturer_id[] = "manufacturer_id";
        Json_addData_ExpectAndReturn((char*)buffer, sizeof(buffer), key_manufacturer_id, MANUFACTURER_ID_BASE64_STR, true);

        const char key_product_type[] = "product_type";
        Json_addData_ExpectAndReturn((char*)buffer, sizeof(buffer), key_product_type, PRODUCT_TYPE, true);

        Json_endString_ExpectAndReturn((char*)buffer, sizeof(buffer), true);


        bool success = BoardInfo_getDataJson(buffer, sizeof(buffer));
        TEST_ASSERT_TRUE(success);

        HashAdapter_getBase64HashedBoardId_ExpectAnyArgsAndReturn(false);
        success = BoardInfo_getDataJson(buffer, sizeof(buffer));
        TEST_ASSERT_FALSE(success);
    }
}
