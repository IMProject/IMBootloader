/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "mock_board_info.h"

static const char* CMockString_BoardInfo_getBase64ManufacturerId = "BoardInfo_getBase64ManufacturerId";
static const char* CMockString_BoardInfo_getDataJson = "BoardInfo_getDataJson";
static const char* CMockString_b64_manufacturer_id = "b64_manufacturer_id";
static const char* CMockString_buffer = "buffer";
static const char* CMockString_size = "size";

typedef struct _CMOCK_BoardInfo_getDataJson_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  bool ReturnVal;
  uint8_t* Expected_buffer;
  size_t Expected_size;
  int Expected_buffer_Depth;
  char ReturnThruPtr_buffer_Used;
  uint8_t const* ReturnThruPtr_buffer_Val;
  size_t ReturnThruPtr_buffer_Size;
  char IgnoreArg_buffer;
  char IgnoreArg_size;

} CMOCK_BoardInfo_getDataJson_CALL_INSTANCE;

typedef struct _CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  bool ReturnVal;
  uint8_t* Expected_b64_manufacturer_id;
  int Expected_b64_manufacturer_id_Depth;
  char ReturnThruPtr_b64_manufacturer_id_Used;
  uint8_t const* ReturnThruPtr_b64_manufacturer_id_Val;
  size_t ReturnThruPtr_b64_manufacturer_id_Size;
  char IgnoreArg_b64_manufacturer_id;

} CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE;

static struct mock_board_infoInstance
{
  char BoardInfo_getDataJson_IgnoreBool;
  bool BoardInfo_getDataJson_FinalReturn;
  char BoardInfo_getDataJson_CallbackBool;
  CMOCK_BoardInfo_getDataJson_CALLBACK BoardInfo_getDataJson_CallbackFunctionPointer;
  int BoardInfo_getDataJson_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE BoardInfo_getDataJson_CallInstance;
  char BoardInfo_getBase64ManufacturerId_IgnoreBool;
  bool BoardInfo_getBase64ManufacturerId_FinalReturn;
  char BoardInfo_getBase64ManufacturerId_CallbackBool;
  CMOCK_BoardInfo_getBase64ManufacturerId_CALLBACK BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer;
  int BoardInfo_getBase64ManufacturerId_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE BoardInfo_getBase64ManufacturerId_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void mock_board_info_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.BoardInfo_getDataJson_CallInstance;
  if (Mock.BoardInfo_getDataJson_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_BoardInfo_getDataJson);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.BoardInfo_getDataJson_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.BoardInfo_getBase64ManufacturerId_CallInstance;
  if (Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_BoardInfo_getBase64ManufacturerId);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
}

void mock_board_info_Init(void)
{
  mock_board_info_Destroy();
}

void mock_board_info_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

bool BoardInfo_getDataJson(uint8_t* buffer, size_t size)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_BoardInfo_getDataJson);
  cmock_call_instance = (CMOCK_BoardInfo_getDataJson_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.BoardInfo_getDataJson_CallInstance);
  Mock.BoardInfo_getDataJson_CallInstance = CMock_Guts_MemNext(Mock.BoardInfo_getDataJson_CallInstance);
  if (Mock.BoardInfo_getDataJson_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.BoardInfo_getDataJson_FinalReturn;
    Mock.BoardInfo_getDataJson_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.BoardInfo_getDataJson_CallbackBool &&
      Mock.BoardInfo_getDataJson_CallbackFunctionPointer != NULL)
  {
    bool cmock_cb_ret = Mock.BoardInfo_getDataJson_CallbackFunctionPointer(buffer, size, Mock.BoardInfo_getDataJson_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_buffer)
  {
    UNITY_SET_DETAILS(CMockString_BoardInfo_getDataJson,CMockString_buffer);
    if (cmock_call_instance->Expected_buffer == NULL)
      { UNITY_TEST_ASSERT_NULL(buffer, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(cmock_call_instance->Expected_buffer, buffer, cmock_call_instance->Expected_buffer_Depth, cmock_line, CMockStringMismatch); }
  }
  if (!cmock_call_instance->IgnoreArg_size)
  {
    UNITY_SET_DETAILS(CMockString_BoardInfo_getDataJson,CMockString_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_size), (void*)(&size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  if (Mock.BoardInfo_getDataJson_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.BoardInfo_getDataJson_CallbackFunctionPointer(buffer, size, Mock.BoardInfo_getDataJson_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_buffer_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(buffer, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)buffer, (const void*)cmock_call_instance->ReturnThruPtr_buffer_Val,
      cmock_call_instance->ReturnThruPtr_buffer_Size);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_BoardInfo_getDataJson(CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance, uint8_t* buffer, int buffer_Depth, size_t size);
void CMockExpectParameters_BoardInfo_getDataJson(CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance, uint8_t* buffer, int buffer_Depth, size_t size)
{
  cmock_call_instance->Expected_buffer = buffer;
  cmock_call_instance->Expected_buffer_Depth = buffer_Depth;
  cmock_call_instance->IgnoreArg_buffer = 0;
  cmock_call_instance->ReturnThruPtr_buffer_Used = 0;
  memcpy((void*)(&cmock_call_instance->Expected_size), (void*)(&size),
         sizeof(size_t[sizeof(size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_size = 0;
}

void BoardInfo_getDataJson_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_BoardInfo_getDataJson_CALL_INSTANCE));
  CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getDataJson_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.BoardInfo_getDataJson_CallInstance = CMock_Guts_MemChain(Mock.BoardInfo_getDataJson_CallInstance, cmock_guts_index);
  Mock.BoardInfo_getDataJson_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.BoardInfo_getDataJson_IgnoreBool = (char)1;
}

void BoardInfo_getDataJson_CMockStopIgnore(void)
{
  if(Mock.BoardInfo_getDataJson_IgnoreBool)
    Mock.BoardInfo_getDataJson_CallInstance = CMock_Guts_MemNext(Mock.BoardInfo_getDataJson_CallInstance);
  Mock.BoardInfo_getDataJson_IgnoreBool = (char)0;
}

void BoardInfo_getDataJson_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* buffer, size_t size, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_BoardInfo_getDataJson_CALL_INSTANCE));
  CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getDataJson_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.BoardInfo_getDataJson_CallInstance = CMock_Guts_MemChain(Mock.BoardInfo_getDataJson_CallInstance, cmock_guts_index);
  Mock.BoardInfo_getDataJson_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_BoardInfo_getDataJson(cmock_call_instance, buffer, size, size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void BoardInfo_getDataJson_AddCallback(CMOCK_BoardInfo_getDataJson_CALLBACK Callback)
{
  Mock.BoardInfo_getDataJson_IgnoreBool = (char)0;
  Mock.BoardInfo_getDataJson_CallbackBool = (char)1;
  Mock.BoardInfo_getDataJson_CallbackFunctionPointer = Callback;
}

void BoardInfo_getDataJson_Stub(CMOCK_BoardInfo_getDataJson_CALLBACK Callback)
{
  Mock.BoardInfo_getDataJson_IgnoreBool = (char)0;
  Mock.BoardInfo_getDataJson_CallbackBool = (char)0;
  Mock.BoardInfo_getDataJson_CallbackFunctionPointer = Callback;
}

void BoardInfo_getDataJson_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* buffer, int buffer_Depth, size_t size, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_BoardInfo_getDataJson_CALL_INSTANCE));
  CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getDataJson_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.BoardInfo_getDataJson_CallInstance = CMock_Guts_MemChain(Mock.BoardInfo_getDataJson_CallInstance, cmock_guts_index);
  Mock.BoardInfo_getDataJson_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_BoardInfo_getDataJson(cmock_call_instance, buffer, buffer_Depth, size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void BoardInfo_getDataJson_CMockReturnMemThruPtr_buffer(UNITY_LINE_TYPE cmock_line, uint8_t const* buffer, size_t cmock_size)
{
  CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getDataJson_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.BoardInfo_getDataJson_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_buffer_Used = 1;
  cmock_call_instance->ReturnThruPtr_buffer_Val = buffer;
  cmock_call_instance->ReturnThruPtr_buffer_Size = cmock_size;
}

void BoardInfo_getDataJson_CMockIgnoreArg_buffer(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getDataJson_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.BoardInfo_getDataJson_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer = 1;
}

void BoardInfo_getDataJson_CMockIgnoreArg_size(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_BoardInfo_getDataJson_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getDataJson_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.BoardInfo_getDataJson_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_size = 1;
}

bool BoardInfo_getBase64ManufacturerId(uint8_t* b64_manufacturer_id)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_BoardInfo_getBase64ManufacturerId);
  cmock_call_instance = (CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.BoardInfo_getBase64ManufacturerId_CallInstance);
  Mock.BoardInfo_getBase64ManufacturerId_CallInstance = CMock_Guts_MemNext(Mock.BoardInfo_getBase64ManufacturerId_CallInstance);
  if (Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.BoardInfo_getBase64ManufacturerId_FinalReturn;
    Mock.BoardInfo_getBase64ManufacturerId_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.BoardInfo_getBase64ManufacturerId_CallbackBool &&
      Mock.BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer != NULL)
  {
    bool cmock_cb_ret = Mock.BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer(b64_manufacturer_id, Mock.BoardInfo_getBase64ManufacturerId_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_b64_manufacturer_id)
  {
    UNITY_SET_DETAILS(CMockString_BoardInfo_getBase64ManufacturerId,CMockString_b64_manufacturer_id);
    if (cmock_call_instance->Expected_b64_manufacturer_id == NULL)
      { UNITY_TEST_ASSERT_NULL(b64_manufacturer_id, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(cmock_call_instance->Expected_b64_manufacturer_id, b64_manufacturer_id, cmock_call_instance->Expected_b64_manufacturer_id_Depth, cmock_line, CMockStringMismatch); }
  }
  if (Mock.BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer(b64_manufacturer_id, Mock.BoardInfo_getBase64ManufacturerId_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_b64_manufacturer_id_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(b64_manufacturer_id, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)b64_manufacturer_id, (const void*)cmock_call_instance->ReturnThruPtr_b64_manufacturer_id_Val,
      cmock_call_instance->ReturnThruPtr_b64_manufacturer_id_Size);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_BoardInfo_getBase64ManufacturerId(CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance, uint8_t* b64_manufacturer_id, int b64_manufacturer_id_Depth);
void CMockExpectParameters_BoardInfo_getBase64ManufacturerId(CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance, uint8_t* b64_manufacturer_id, int b64_manufacturer_id_Depth)
{
  cmock_call_instance->Expected_b64_manufacturer_id = b64_manufacturer_id;
  cmock_call_instance->Expected_b64_manufacturer_id_Depth = b64_manufacturer_id_Depth;
  cmock_call_instance->IgnoreArg_b64_manufacturer_id = 0;
  cmock_call_instance->ReturnThruPtr_b64_manufacturer_id_Used = 0;
}

void BoardInfo_getBase64ManufacturerId_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE));
  CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.BoardInfo_getBase64ManufacturerId_CallInstance = CMock_Guts_MemChain(Mock.BoardInfo_getBase64ManufacturerId_CallInstance, cmock_guts_index);
  Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool = (char)1;
}

void BoardInfo_getBase64ManufacturerId_CMockStopIgnore(void)
{
  if(Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool)
    Mock.BoardInfo_getBase64ManufacturerId_CallInstance = CMock_Guts_MemNext(Mock.BoardInfo_getBase64ManufacturerId_CallInstance);
  Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool = (char)0;
}

void BoardInfo_getBase64ManufacturerId_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* b64_manufacturer_id, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE));
  CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.BoardInfo_getBase64ManufacturerId_CallInstance = CMock_Guts_MemChain(Mock.BoardInfo_getBase64ManufacturerId_CallInstance, cmock_guts_index);
  Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_BoardInfo_getBase64ManufacturerId(cmock_call_instance, b64_manufacturer_id, 1);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void BoardInfo_getBase64ManufacturerId_AddCallback(CMOCK_BoardInfo_getBase64ManufacturerId_CALLBACK Callback)
{
  Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool = (char)0;
  Mock.BoardInfo_getBase64ManufacturerId_CallbackBool = (char)1;
  Mock.BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer = Callback;
}

void BoardInfo_getBase64ManufacturerId_Stub(CMOCK_BoardInfo_getBase64ManufacturerId_CALLBACK Callback)
{
  Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool = (char)0;
  Mock.BoardInfo_getBase64ManufacturerId_CallbackBool = (char)0;
  Mock.BoardInfo_getBase64ManufacturerId_CallbackFunctionPointer = Callback;
}

void BoardInfo_getBase64ManufacturerId_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* b64_manufacturer_id, int b64_manufacturer_id_Depth, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE));
  CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.BoardInfo_getBase64ManufacturerId_CallInstance = CMock_Guts_MemChain(Mock.BoardInfo_getBase64ManufacturerId_CallInstance, cmock_guts_index);
  Mock.BoardInfo_getBase64ManufacturerId_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_BoardInfo_getBase64ManufacturerId(cmock_call_instance, b64_manufacturer_id, b64_manufacturer_id_Depth);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void BoardInfo_getBase64ManufacturerId_CMockReturnMemThruPtr_b64_manufacturer_id(UNITY_LINE_TYPE cmock_line, uint8_t const* b64_manufacturer_id, size_t cmock_size)
{
  CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.BoardInfo_getBase64ManufacturerId_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_b64_manufacturer_id_Used = 1;
  cmock_call_instance->ReturnThruPtr_b64_manufacturer_id_Val = b64_manufacturer_id;
  cmock_call_instance->ReturnThruPtr_b64_manufacturer_id_Size = cmock_size;
}

void BoardInfo_getBase64ManufacturerId_CMockIgnoreArg_b64_manufacturer_id(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE* cmock_call_instance = (CMOCK_BoardInfo_getBase64ManufacturerId_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.BoardInfo_getBase64ManufacturerId_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_b64_manufacturer_id = 1;
}
