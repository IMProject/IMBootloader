#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "mock_boards_config.h"


static struct mock_boards_configInstance
{
  unsigned char placeHolder;
} Mock;

extern jmp_buf AbortFrame;

void mock_boards_config_Verify(void)
{
}

void mock_boards_config_Init(void)
{
  mock_boards_config_Destroy();
}

void mock_boards_config_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

