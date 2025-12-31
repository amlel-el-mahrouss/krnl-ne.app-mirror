/// \file scheduler.test.cc
/// \brief Scheduler and debugging API tests.
/// \author Amlal El Mahrouss (amlal at nekernel dot org)

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// \note SchedGetCurrentProcessID tests
KT_DECL_TEST(SchedGetCurrentPIDValid, []() -> bool {
  UIntPtr pid = SchedGetCurrentProcessID();
  return pid > 0;
});

/// \note SchedSetAffinity/SchedGetAffinity tests
KT_DECL_TEST(SchedSetAffinityValid, []() -> bool {
  UIntPtr     pid = SchedGetCurrentProcessID();
  AffinityRef affinity;

  SInt32 result = SchedSetAffinity(pid, 1, &affinity);
  return result >= 0;
});

KT_DECL_TEST(SchedGetAffinityValid, []() -> bool {
  UIntPtr     pid = SchedGetCurrentProcessID();
  AffinityRef affinity;

  SInt32 result = SchedGetAffinity(pid, &affinity);
  return result >= 0;
});

KT_DECL_TEST(SchedSetAffinityInvalid, []() -> bool {
  AffinityRef affinity;
  SInt32      result = SchedSetAffinity(0, 1, &affinity);
  return result < 0;
});

KT_DECL_TEST(SchedGetAffinityInvalid, []() -> bool {
  AffinityRef affinity;
  SInt32      result = SchedGetAffinity(0, &affinity);
  return result < 0;
});

KT_DECL_TEST(SchedAffinityRoundTrip, []() -> bool {
  UIntPtr     pid = SchedGetCurrentProcessID();
  AffinityRef affinity_set;
  AffinityRef affinity_get;

  SchedSetAffinity(pid, 2, &affinity_set);
  SInt32 result = SchedGetAffinity(pid, &affinity_get);

  return result >= 0;
});

/// \note SchedFireSignal tests
KT_DECL_TEST(SchedFireSignalValid, []() -> bool {
  UIntPtr pid    = SchedGetCurrentProcessID();
  SInt32  result = SchedFireSignal(pid, 1);
  return result >= 0;
});

KT_DECL_TEST(SchedFireSignalInvalid, []() -> bool {
  SInt32 result = SchedFireSignal(0, 1);
  return result < 0;
});

/// \note SchedReadMemory/SchedWriteMemory tests
KT_DECL_TEST(SchedReadMemoryValid, []() -> bool {
  UIntPtr pid    = SchedGetCurrentProcessID();
  SInt32  result = SchedReadMemory(pid, 0x1000, 64);
  return result >= 0;
});

KT_DECL_TEST(SchedWriteMemoryValid, []() -> bool {
  UIntPtr pid    = SchedGetCurrentProcessID();
  SInt32  result = SchedWriteMemory(pid, 0x1000, 0x42);
  return result >= 0;
});

KT_DECL_TEST(SchedReadMemoryInvalid, []() -> bool {
  SInt32 result = SchedReadMemory(0, 0x1000, 64);
  return result < 0;
});

KT_DECL_TEST(SchedWriteMemoryInvalid, []() -> bool {
  SInt32 result = SchedWriteMemory(0, 0x1000, 0x42);
  return result < 0;
});

/// \brief Run scheduler tests.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(SchedGetCurrentPIDValid);

  KT_RUN_TEST(SchedSetAffinityValid);
  KT_RUN_TEST(SchedGetAffinityValid);
  KT_RUN_TEST(SchedSetAffinityInvalid);
  KT_RUN_TEST(SchedGetAffinityInvalid);
  KT_RUN_TEST(SchedAffinityRoundTrip);

  KT_RUN_TEST(SchedFireSignalValid);
  KT_RUN_TEST(SchedFireSignalInvalid);

  KT_RUN_TEST(SchedReadMemoryValid);
  KT_RUN_TEST(SchedWriteMemoryValid);
  KT_RUN_TEST(SchedReadMemoryInvalid);
  KT_RUN_TEST(SchedWriteMemoryInvalid);

  return KT_TEST_SUCCESS;
}
