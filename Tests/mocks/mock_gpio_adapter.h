/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_GPIO_ADAPTER_H
#define _MOCK_GPIO_ADAPTER_H

#include "unity.h"
#include "gpio_adapter.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void mock_gpio_adapter_Init(void);
void mock_gpio_adapter_Destroy(void);
void mock_gpio_adapter_Verify(void);




#define GpioAdapter_init_Ignore() GpioAdapter_init_CMockIgnore()
void GpioAdapter_init_CMockIgnore(void);
#define GpioAdapter_init_StopIgnore() GpioAdapter_init_CMockStopIgnore()
void GpioAdapter_init_CMockStopIgnore(void);
#define GpioAdapter_init_Expect() GpioAdapter_init_CMockExpect(__LINE__)
void GpioAdapter_init_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_GpioAdapter_init_CALLBACK)(int cmock_num_calls);
void GpioAdapter_init_AddCallback(CMOCK_GpioAdapter_init_CALLBACK Callback);
void GpioAdapter_init_Stub(CMOCK_GpioAdapter_init_CALLBACK Callback);
#define GpioAdapter_init_StubWithCallback GpioAdapter_init_Stub
#define GpioAdapter_led1Toggle_Ignore() GpioAdapter_led1Toggle_CMockIgnore()
void GpioAdapter_led1Toggle_CMockIgnore(void);
#define GpioAdapter_led1Toggle_StopIgnore() GpioAdapter_led1Toggle_CMockStopIgnore()
void GpioAdapter_led1Toggle_CMockStopIgnore(void);
#define GpioAdapter_led1Toggle_Expect() GpioAdapter_led1Toggle_CMockExpect(__LINE__)
void GpioAdapter_led1Toggle_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_GpioAdapter_led1Toggle_CALLBACK)(int cmock_num_calls);
void GpioAdapter_led1Toggle_AddCallback(CMOCK_GpioAdapter_led1Toggle_CALLBACK Callback);
void GpioAdapter_led1Toggle_Stub(CMOCK_GpioAdapter_led1Toggle_CALLBACK Callback);
#define GpioAdapter_led1Toggle_StubWithCallback GpioAdapter_led1Toggle_Stub
#define GpioAdapter_led1On_Ignore() GpioAdapter_led1On_CMockIgnore()
void GpioAdapter_led1On_CMockIgnore(void);
#define GpioAdapter_led1On_StopIgnore() GpioAdapter_led1On_CMockStopIgnore()
void GpioAdapter_led1On_CMockStopIgnore(void);
#define GpioAdapter_led1On_Expect() GpioAdapter_led1On_CMockExpect(__LINE__)
void GpioAdapter_led1On_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_GpioAdapter_led1On_CALLBACK)(int cmock_num_calls);
void GpioAdapter_led1On_AddCallback(CMOCK_GpioAdapter_led1On_CALLBACK Callback);
void GpioAdapter_led1On_Stub(CMOCK_GpioAdapter_led1On_CALLBACK Callback);
#define GpioAdapter_led1On_StubWithCallback GpioAdapter_led1On_Stub
#define GpioAdapter_led1Off_Ignore() GpioAdapter_led1Off_CMockIgnore()
void GpioAdapter_led1Off_CMockIgnore(void);
#define GpioAdapter_led1Off_StopIgnore() GpioAdapter_led1Off_CMockStopIgnore()
void GpioAdapter_led1Off_CMockStopIgnore(void);
#define GpioAdapter_led1Off_Expect() GpioAdapter_led1Off_CMockExpect(__LINE__)
void GpioAdapter_led1Off_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_GpioAdapter_led1Off_CALLBACK)(int cmock_num_calls);
void GpioAdapter_led1Off_AddCallback(CMOCK_GpioAdapter_led1Off_CALLBACK Callback);
void GpioAdapter_led1Off_Stub(CMOCK_GpioAdapter_led1Off_CALLBACK Callback);
#define GpioAdapter_led1Off_StubWithCallback GpioAdapter_led1Off_Stub

#ifdef __cplusplus
}
#endif

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif
