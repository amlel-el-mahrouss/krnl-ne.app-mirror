// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef CFKIT_GUIDWIZARD_H
#define CFKIT_GUIDWIZARD_H

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

#endif
