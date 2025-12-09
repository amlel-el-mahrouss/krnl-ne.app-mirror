/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CFKit/GUIDWrapper.h>
#include <NeKit/Array.h>
#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/InitializerList.h>
#include <NeKit/KString.h>
#include <NeKit/Ref.h>
#include <NeKit/Stream.h>

namespace Kernel::CF::XRN::Version1 {
Ref<GUIDSequence*>    cf_make_sequence(const Array<UInt32, 10>& seq);
ErrorOr<Ref<KString>> cf_try_guid_to_string(Ref<GUIDSequence*>& guid);
}  // namespace Kernel::CF::XRN::Version1
