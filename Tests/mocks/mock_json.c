/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "mock_json.h"

static const char* CMockString_Json_addData = "Json_addData";
static const char* CMockString_Json_endString = "Json_endString";
static const char* CMockString_Json_findByKey = "Json_findByKey";
static const char* CMockString_Json_startString = "Json_startString";
static const char* CMockString_buffer = "buffer";
static const char* CMockString_buffer_size = "buffer_size";
static const char* CMockString_key = "key";
static const char* CMockString_max_value_size = "max_value_size";
static const char* CMockString_value = "value";

typedef struct _CMOCK_Json_startString_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  bool ReturnVal;
  char* Expected_buffer;
  size_t Expected_buffer_size;
  char ReturnThruPtr_buffer_Used;
  char const* ReturnThruPtr_buffer_Val;
  size_t ReturnThruPtr_buffer_Size;
  char IgnoreArg_buffer;
  char IgnoreArg_buffer_size;

} CMOCK_Json_startString_CALL_INSTANCE;

typedef struct _CMOCK_Json_addData_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  bool ReturnVal;
  char* Expected_buffer;
  size_t Expected_buffer_size;
  const char* Expected_key;
  const char* Expected_value;
  char ReturnThruPtr_buffer_Used;
  char const* ReturnThruPtr_buffer_Val;
  size_t ReturnThruPtr_buffer_Size;
  char IgnoreArg_buffer;
  char IgnoreArg_buffer_size;
  char IgnoreArg_key;
  char IgnoreArg_value;

} CMOCK_Json_addData_CALL_INSTANCE;

typedef struct _CMOCK_Json_endString_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  bool ReturnVal;
  char* Expected_buffer;
  size_t Expected_buffer_size;
  char ReturnThruPtr_buffer_Used;
  char const* ReturnThruPtr_buffer_Val;
  size_t ReturnThruPtr_buffer_Size;
  char IgnoreArg_buffer;
  char IgnoreArg_buffer_size;

} CMOCK_Json_endString_CALL_INSTANCE;

typedef struct _CMOCK_Json_findByKey_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  bool ReturnVal;
  const char* Expected_buffer;
  size_t Expected_buffer_size;
  const char* Expected_key;
  char* Expected_value;
  size_t Expected_max_value_size;
  char ReturnThruPtr_value_Used;
  char const* ReturnThruPtr_value_Val;
  size_t ReturnThruPtr_value_Size;
  char IgnoreArg_buffer;
  char IgnoreArg_buffer_size;
  char IgnoreArg_key;
  char IgnoreArg_value;
  char IgnoreArg_max_value_size;

} CMOCK_Json_findByKey_CALL_INSTANCE;

static struct mock_jsonInstance
{
  char Json_startString_IgnoreBool;
  bool Json_startString_FinalReturn;
  char Json_startString_CallbackBool;
  CMOCK_Json_startString_CALLBACK Json_startString_CallbackFunctionPointer;
  int Json_startString_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE Json_startString_CallInstance;
  char Json_addData_IgnoreBool;
  bool Json_addData_FinalReturn;
  char Json_addData_CallbackBool;
  CMOCK_Json_addData_CALLBACK Json_addData_CallbackFunctionPointer;
  int Json_addData_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE Json_addData_CallInstance;
  char Json_endString_IgnoreBool;
  bool Json_endString_FinalReturn;
  char Json_endString_CallbackBool;
  CMOCK_Json_endString_CALLBACK Json_endString_CallbackFunctionPointer;
  int Json_endString_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE Json_endString_CallInstance;
  char Json_findByKey_IgnoreBool;
  bool Json_findByKey_FinalReturn;
  char Json_findByKey_CallbackBool;
  CMOCK_Json_findByKey_CALLBACK Json_findByKey_CallbackFunctionPointer;
  int Json_findByKey_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE Json_findByKey_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void mock_json_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.Json_startString_CallInstance;
  if (Mock.Json_startString_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_Json_startString);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.Json_startString_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.Json_addData_CallInstance;
  if (Mock.Json_addData_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_Json_addData);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.Json_addData_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.Json_endString_CallInstance;
  if (Mock.Json_endString_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_Json_endString);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.Json_endString_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.Json_findByKey_CallInstance;
  if (Mock.Json_findByKey_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_Json_findByKey);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.Json_findByKey_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
}

void mock_json_Init(void)
{
  mock_json_Destroy();
}

void mock_json_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

bool Json_startString(char* buffer, size_t buffer_size)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_Json_startString);
  cmock_call_instance = (CMOCK_Json_startString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Json_startString_CallInstance);
  Mock.Json_startString_CallInstance = CMock_Guts_MemNext(Mock.Json_startString_CallInstance);
  if (Mock.Json_startString_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.Json_startString_FinalReturn;
    Mock.Json_startString_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.Json_startString_CallbackBool &&
      Mock.Json_startString_CallbackFunctionPointer != NULL)
  {
    bool cmock_cb_ret = Mock.Json_startString_CallbackFunctionPointer(buffer, buffer_size, Mock.Json_startString_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_buffer)
  {
    UNITY_SET_DETAILS(CMockString_Json_startString,CMockString_buffer);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_buffer, buffer, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_buffer_size)
  {
    UNITY_SET_DETAILS(CMockString_Json_startString,CMockString_buffer_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  if (Mock.Json_startString_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.Json_startString_CallbackFunctionPointer(buffer, buffer_size, Mock.Json_startString_CallbackCalls++);
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

void CMockExpectParameters_Json_startString(CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance, char* buffer, size_t buffer_size);
void CMockExpectParameters_Json_startString(CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance, char* buffer, size_t buffer_size)
{
  cmock_call_instance->Expected_buffer = buffer;
  cmock_call_instance->IgnoreArg_buffer = 0;
  cmock_call_instance->ReturnThruPtr_buffer_Used = 0;
  memcpy((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size),
         sizeof(size_t[sizeof(buffer_size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_buffer_size = 0;
}

void Json_startString_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_startString_CALL_INSTANCE));
  CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_startString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_startString_CallInstance = CMock_Guts_MemChain(Mock.Json_startString_CallInstance, cmock_guts_index);
  Mock.Json_startString_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.Json_startString_IgnoreBool = (char)1;
}

void Json_startString_CMockStopIgnore(void)
{
  if(Mock.Json_startString_IgnoreBool)
    Mock.Json_startString_CallInstance = CMock_Guts_MemNext(Mock.Json_startString_CallInstance);
  Mock.Json_startString_IgnoreBool = (char)0;
}

void Json_startString_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char* buffer, size_t buffer_size, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_startString_CALL_INSTANCE));
  CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_startString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_startString_CallInstance = CMock_Guts_MemChain(Mock.Json_startString_CallInstance, cmock_guts_index);
  Mock.Json_startString_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Json_startString(cmock_call_instance, buffer, buffer_size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void Json_startString_AddCallback(CMOCK_Json_startString_CALLBACK Callback)
{
  Mock.Json_startString_IgnoreBool = (char)0;
  Mock.Json_startString_CallbackBool = (char)1;
  Mock.Json_startString_CallbackFunctionPointer = Callback;
}

void Json_startString_Stub(CMOCK_Json_startString_CALLBACK Callback)
{
  Mock.Json_startString_IgnoreBool = (char)0;
  Mock.Json_startString_CallbackBool = (char)0;
  Mock.Json_startString_CallbackFunctionPointer = Callback;
}

void Json_startString_CMockReturnMemThruPtr_buffer(UNITY_LINE_TYPE cmock_line, char const* buffer, size_t cmock_size)
{
  CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_startString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_startString_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_buffer_Used = 1;
  cmock_call_instance->ReturnThruPtr_buffer_Val = buffer;
  cmock_call_instance->ReturnThruPtr_buffer_Size = cmock_size;
}

void Json_startString_CMockIgnoreArg_buffer(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_startString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_startString_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer = 1;
}

void Json_startString_CMockIgnoreArg_buffer_size(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_startString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_startString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_startString_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer_size = 1;
}

bool Json_addData(char* buffer, size_t buffer_size, const char* key, const char* value)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_Json_addData);
  cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Json_addData_CallInstance);
  Mock.Json_addData_CallInstance = CMock_Guts_MemNext(Mock.Json_addData_CallInstance);
  if (Mock.Json_addData_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.Json_addData_FinalReturn;
    Mock.Json_addData_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.Json_addData_CallbackBool &&
      Mock.Json_addData_CallbackFunctionPointer != NULL)
  {
    bool cmock_cb_ret = Mock.Json_addData_CallbackFunctionPointer(buffer, buffer_size, key, value, Mock.Json_addData_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_buffer)
  {
    UNITY_SET_DETAILS(CMockString_Json_addData,CMockString_buffer);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_buffer, buffer, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_buffer_size)
  {
    UNITY_SET_DETAILS(CMockString_Json_addData,CMockString_buffer_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_key)
  {
    UNITY_SET_DETAILS(CMockString_Json_addData,CMockString_key);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_key, key, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_value)
  {
    UNITY_SET_DETAILS(CMockString_Json_addData,CMockString_value);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_value, value, cmock_line, CMockStringMismatch);
  }
  if (Mock.Json_addData_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.Json_addData_CallbackFunctionPointer(buffer, buffer_size, key, value, Mock.Json_addData_CallbackCalls++);
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

void CMockExpectParameters_Json_addData(CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance, char* buffer, size_t buffer_size, const char* key, const char* value);
void CMockExpectParameters_Json_addData(CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance, char* buffer, size_t buffer_size, const char* key, const char* value)
{
  cmock_call_instance->Expected_buffer = buffer;
  cmock_call_instance->IgnoreArg_buffer = 0;
  cmock_call_instance->ReturnThruPtr_buffer_Used = 0;
  memcpy((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size),
         sizeof(size_t[sizeof(buffer_size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_buffer_size = 0;
  cmock_call_instance->Expected_key = key;
  cmock_call_instance->IgnoreArg_key = 0;
  cmock_call_instance->Expected_value = value;
  cmock_call_instance->IgnoreArg_value = 0;
}

void Json_addData_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_addData_CALL_INSTANCE));
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_addData_CallInstance = CMock_Guts_MemChain(Mock.Json_addData_CallInstance, cmock_guts_index);
  Mock.Json_addData_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.Json_addData_IgnoreBool = (char)1;
}

void Json_addData_CMockStopIgnore(void)
{
  if(Mock.Json_addData_IgnoreBool)
    Mock.Json_addData_CallInstance = CMock_Guts_MemNext(Mock.Json_addData_CallInstance);
  Mock.Json_addData_IgnoreBool = (char)0;
}

void Json_addData_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char* buffer, size_t buffer_size, const char* key, const char* value, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_addData_CALL_INSTANCE));
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_addData_CallInstance = CMock_Guts_MemChain(Mock.Json_addData_CallInstance, cmock_guts_index);
  Mock.Json_addData_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Json_addData(cmock_call_instance, buffer, buffer_size, key, value);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void Json_addData_AddCallback(CMOCK_Json_addData_CALLBACK Callback)
{
  Mock.Json_addData_IgnoreBool = (char)0;
  Mock.Json_addData_CallbackBool = (char)1;
  Mock.Json_addData_CallbackFunctionPointer = Callback;
}

void Json_addData_Stub(CMOCK_Json_addData_CALLBACK Callback)
{
  Mock.Json_addData_IgnoreBool = (char)0;
  Mock.Json_addData_CallbackBool = (char)0;
  Mock.Json_addData_CallbackFunctionPointer = Callback;
}

void Json_addData_CMockReturnMemThruPtr_buffer(UNITY_LINE_TYPE cmock_line, char const* buffer, size_t cmock_size)
{
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_addData_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_buffer_Used = 1;
  cmock_call_instance->ReturnThruPtr_buffer_Val = buffer;
  cmock_call_instance->ReturnThruPtr_buffer_Size = cmock_size;
}

void Json_addData_CMockIgnoreArg_buffer(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_addData_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer = 1;
}

void Json_addData_CMockIgnoreArg_buffer_size(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_addData_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer_size = 1;
}

void Json_addData_CMockIgnoreArg_key(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_addData_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_key = 1;
}

void Json_addData_CMockIgnoreArg_value(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_addData_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_addData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_addData_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_value = 1;
}

bool Json_endString(char* buffer, size_t buffer_size)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_Json_endString);
  cmock_call_instance = (CMOCK_Json_endString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Json_endString_CallInstance);
  Mock.Json_endString_CallInstance = CMock_Guts_MemNext(Mock.Json_endString_CallInstance);
  if (Mock.Json_endString_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.Json_endString_FinalReturn;
    Mock.Json_endString_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.Json_endString_CallbackBool &&
      Mock.Json_endString_CallbackFunctionPointer != NULL)
  {
    bool cmock_cb_ret = Mock.Json_endString_CallbackFunctionPointer(buffer, buffer_size, Mock.Json_endString_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_buffer)
  {
    UNITY_SET_DETAILS(CMockString_Json_endString,CMockString_buffer);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_buffer, buffer, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_buffer_size)
  {
    UNITY_SET_DETAILS(CMockString_Json_endString,CMockString_buffer_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  if (Mock.Json_endString_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.Json_endString_CallbackFunctionPointer(buffer, buffer_size, Mock.Json_endString_CallbackCalls++);
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

void CMockExpectParameters_Json_endString(CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance, char* buffer, size_t buffer_size);
void CMockExpectParameters_Json_endString(CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance, char* buffer, size_t buffer_size)
{
  cmock_call_instance->Expected_buffer = buffer;
  cmock_call_instance->IgnoreArg_buffer = 0;
  cmock_call_instance->ReturnThruPtr_buffer_Used = 0;
  memcpy((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size),
         sizeof(size_t[sizeof(buffer_size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_buffer_size = 0;
}

void Json_endString_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_endString_CALL_INSTANCE));
  CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_endString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_endString_CallInstance = CMock_Guts_MemChain(Mock.Json_endString_CallInstance, cmock_guts_index);
  Mock.Json_endString_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.Json_endString_IgnoreBool = (char)1;
}

void Json_endString_CMockStopIgnore(void)
{
  if(Mock.Json_endString_IgnoreBool)
    Mock.Json_endString_CallInstance = CMock_Guts_MemNext(Mock.Json_endString_CallInstance);
  Mock.Json_endString_IgnoreBool = (char)0;
}

void Json_endString_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char* buffer, size_t buffer_size, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_endString_CALL_INSTANCE));
  CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_endString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_endString_CallInstance = CMock_Guts_MemChain(Mock.Json_endString_CallInstance, cmock_guts_index);
  Mock.Json_endString_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Json_endString(cmock_call_instance, buffer, buffer_size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void Json_endString_AddCallback(CMOCK_Json_endString_CALLBACK Callback)
{
  Mock.Json_endString_IgnoreBool = (char)0;
  Mock.Json_endString_CallbackBool = (char)1;
  Mock.Json_endString_CallbackFunctionPointer = Callback;
}

void Json_endString_Stub(CMOCK_Json_endString_CALLBACK Callback)
{
  Mock.Json_endString_IgnoreBool = (char)0;
  Mock.Json_endString_CallbackBool = (char)0;
  Mock.Json_endString_CallbackFunctionPointer = Callback;
}

void Json_endString_CMockReturnMemThruPtr_buffer(UNITY_LINE_TYPE cmock_line, char const* buffer, size_t cmock_size)
{
  CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_endString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_endString_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_buffer_Used = 1;
  cmock_call_instance->ReturnThruPtr_buffer_Val = buffer;
  cmock_call_instance->ReturnThruPtr_buffer_Size = cmock_size;
}

void Json_endString_CMockIgnoreArg_buffer(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_endString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_endString_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer = 1;
}

void Json_endString_CMockIgnoreArg_buffer_size(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_endString_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_endString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_endString_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer_size = 1;
}

bool Json_findByKey(const char* buffer, size_t buffer_size, const char* key, char* value, size_t max_value_size)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_Json_findByKey);
  cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Json_findByKey_CallInstance);
  Mock.Json_findByKey_CallInstance = CMock_Guts_MemNext(Mock.Json_findByKey_CallInstance);
  if (Mock.Json_findByKey_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.Json_findByKey_FinalReturn;
    Mock.Json_findByKey_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.Json_findByKey_CallbackBool &&
      Mock.Json_findByKey_CallbackFunctionPointer != NULL)
  {
    bool cmock_cb_ret = Mock.Json_findByKey_CallbackFunctionPointer(buffer, buffer_size, key, value, max_value_size, Mock.Json_findByKey_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->IgnoreArg_buffer)
  {
    UNITY_SET_DETAILS(CMockString_Json_findByKey,CMockString_buffer);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_buffer, buffer, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_buffer_size)
  {
    UNITY_SET_DETAILS(CMockString_Json_findByKey,CMockString_buffer_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_key)
  {
    UNITY_SET_DETAILS(CMockString_Json_findByKey,CMockString_key);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_key, key, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_value)
  {
    UNITY_SET_DETAILS(CMockString_Json_findByKey,CMockString_value);
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_value, value, cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_max_value_size)
  {
    UNITY_SET_DETAILS(CMockString_Json_findByKey,CMockString_max_value_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_max_value_size), (void*)(&max_value_size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  if (Mock.Json_findByKey_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.Json_findByKey_CallbackFunctionPointer(buffer, buffer_size, key, value, max_value_size, Mock.Json_findByKey_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_value_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(value, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)value, (const void*)cmock_call_instance->ReturnThruPtr_value_Val,
      cmock_call_instance->ReturnThruPtr_value_Size);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_Json_findByKey(CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance, const char* buffer, size_t buffer_size, const char* key, char* value, size_t max_value_size);
void CMockExpectParameters_Json_findByKey(CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance, const char* buffer, size_t buffer_size, const char* key, char* value, size_t max_value_size)
{
  cmock_call_instance->Expected_buffer = buffer;
  cmock_call_instance->IgnoreArg_buffer = 0;
  memcpy((void*)(&cmock_call_instance->Expected_buffer_size), (void*)(&buffer_size),
         sizeof(size_t[sizeof(buffer_size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_buffer_size = 0;
  cmock_call_instance->Expected_key = key;
  cmock_call_instance->IgnoreArg_key = 0;
  cmock_call_instance->Expected_value = value;
  cmock_call_instance->IgnoreArg_value = 0;
  cmock_call_instance->ReturnThruPtr_value_Used = 0;
  memcpy((void*)(&cmock_call_instance->Expected_max_value_size), (void*)(&max_value_size),
         sizeof(size_t[sizeof(max_value_size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_max_value_size = 0;
}

void Json_findByKey_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_findByKey_CALL_INSTANCE));
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_findByKey_CallInstance = CMock_Guts_MemChain(Mock.Json_findByKey_CallInstance, cmock_guts_index);
  Mock.Json_findByKey_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.Json_findByKey_IgnoreBool = (char)1;
}

void Json_findByKey_CMockStopIgnore(void)
{
  if(Mock.Json_findByKey_IgnoreBool)
    Mock.Json_findByKey_CallInstance = CMock_Guts_MemNext(Mock.Json_findByKey_CallInstance);
  Mock.Json_findByKey_IgnoreBool = (char)0;
}

void Json_findByKey_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* buffer, size_t buffer_size, const char* key, char* value, size_t max_value_size, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Json_findByKey_CALL_INSTANCE));
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.Json_findByKey_CallInstance = CMock_Guts_MemChain(Mock.Json_findByKey_CallInstance, cmock_guts_index);
  Mock.Json_findByKey_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Json_findByKey(cmock_call_instance, buffer, buffer_size, key, value, max_value_size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void Json_findByKey_AddCallback(CMOCK_Json_findByKey_CALLBACK Callback)
{
  Mock.Json_findByKey_IgnoreBool = (char)0;
  Mock.Json_findByKey_CallbackBool = (char)1;
  Mock.Json_findByKey_CallbackFunctionPointer = Callback;
}

void Json_findByKey_Stub(CMOCK_Json_findByKey_CALLBACK Callback)
{
  Mock.Json_findByKey_IgnoreBool = (char)0;
  Mock.Json_findByKey_CallbackBool = (char)0;
  Mock.Json_findByKey_CallbackFunctionPointer = Callback;
}

void Json_findByKey_CMockReturnMemThruPtr_value(UNITY_LINE_TYPE cmock_line, char const* value, size_t cmock_size)
{
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_findByKey_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_value_Used = 1;
  cmock_call_instance->ReturnThruPtr_value_Val = value;
  cmock_call_instance->ReturnThruPtr_value_Size = cmock_size;
}

void Json_findByKey_CMockIgnoreArg_buffer(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_findByKey_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer = 1;
}

void Json_findByKey_CMockIgnoreArg_buffer_size(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_findByKey_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_buffer_size = 1;
}

void Json_findByKey_CMockIgnoreArg_key(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_findByKey_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_key = 1;
}

void Json_findByKey_CMockIgnoreArg_value(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_findByKey_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_value = 1;
}

void Json_findByKey_CMockIgnoreArg_max_value_size(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_Json_findByKey_CALL_INSTANCE* cmock_call_instance = (CMOCK_Json_findByKey_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.Json_findByKey_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_max_value_size = 1;
}
