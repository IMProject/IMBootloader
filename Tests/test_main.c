#include "unity.h"
#include "unity_fixture.h"

static void
RunAllTests(void) {
#ifdef BINARY_UPDATE
    RUN_TEST_GROUP(BinaryUpdate);
#elif BOARD_INFO
    RUN_TEST_GROUP(BoardInfo);
#endif
}

int
main(int argc, const char* argv[]) {
    return UnityMain(argc, argv, RunAllTests);
}
