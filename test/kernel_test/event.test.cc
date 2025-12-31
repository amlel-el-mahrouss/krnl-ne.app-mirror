/// \file event.test.cc
/// \brief Event handling API tests.
/// \author Amlal El Mahrouss (amlal at nekernel dot org)

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// \note Mock event listener
static Bool event_listener_called = NO;

static Void mock_event_listener(VoidPtr data) {
  event_listener_called = YES;
}

/// \note EvtAddListener tests
KT_DECL_TEST(EvtAddListenerValid, []() -> bool {
  REF_TYPE listener_ref;
  listener_ref.__hash = 0x1234;
  listener_ref.__self = (VoidPtr) mock_event_listener;

  EvtAddListener("test_event", (Ref) &listener_ref);
  return YES;
});

KT_DECL_TEST(EvtAddListenerNull, []() -> bool {
  EvtAddListener(nullptr, nullptr);
  return YES;
});

/// \note EvtRemoveListener tests
KT_DECL_TEST(EvtRemoveListenerValid, []() -> bool {
  REF_TYPE listener_ref;
  listener_ref.__hash = 0x1234;
  listener_ref.__self = (VoidPtr) mock_event_listener;

  EvtAddListener("remove_test", (Ref) &listener_ref);
  EvtRemoveListener("remove_test", (Ref) &listener_ref);
  return YES;
});

KT_DECL_TEST(EvtRemoveListenerNull, []() -> bool {
  EvtRemoveListener(nullptr, nullptr);
  return YES;
});

/// \note EvtDispatchEvent tests
KT_DECL_TEST(EvtDispatchEventValid, []() -> bool {
  REF_TYPE listener_ref;
  listener_ref.__hash = 0x5678;
  listener_ref.__self = (VoidPtr) mock_event_listener;

  event_listener_called = NO;

  EvtAddListener("dispatch_test", (Ref) &listener_ref);

  char data[] = "event_data";
  EvtDispatchEvent("dispatch_test", (VoidPtr) data);

  return YES;
});

KT_DECL_TEST(EvtDispatchEventNull, []() -> bool {
  VoidPtr result = EvtDispatchEvent(nullptr, nullptr);
  return result == nullptr;
});

KT_DECL_TEST(EvtDispatchEventNotExist, []() -> bool {
  VoidPtr result = EvtDispatchEvent("nonexistent_event", nullptr);
  return YES;
});

/// \note Event lifecycle test
KT_DECL_TEST(EvtLifecycleTest, []() -> bool {
  REF_TYPE listener_ref;
  listener_ref.__hash = 0xABCD;
  listener_ref.__self = (VoidPtr) mock_event_listener;

  EvtAddListener("lifecycle_event", (Ref) &listener_ref);

  char data[] = "test";
  EvtDispatchEvent("lifecycle_event", (VoidPtr) data);

  EvtRemoveListener("lifecycle_event", (Ref) &listener_ref);

  return YES;
});

/// \brief Run event tests.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(EvtAddListenerValid);
  KT_RUN_TEST(EvtAddListenerNull);

  KT_RUN_TEST(EvtRemoveListenerValid);
  KT_RUN_TEST(EvtRemoveListenerNull);

  KT_RUN_TEST(EvtDispatchEventValid);
  KT_RUN_TEST(EvtDispatchEventNull);
  KT_RUN_TEST(EvtDispatchEventNotExist);

  KT_RUN_TEST(EvtLifecycleTest);

  return KT_TEST_SUCCESS;
}
