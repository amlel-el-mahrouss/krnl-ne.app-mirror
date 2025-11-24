/* ========================================

  Copyright Amlal El Mahrouss.

======================================== */

#ifndef CORE_GFX_ACCESSIBILITY_H
#define CORE_GFX_ACCESSIBILITY_H

#include <ArchKit/ArchKit.h>
#include <KernelKit/KPC.h>
#include <NeKit/NeKit.h>
#include <modules/CoreGfx/CoreGfx.h>
#include <modules/CoreGfx/MathGfx.h>

namespace FB {
using namespace Kernel;

/// @brief common User interface class.
class CGAccessibilty final {
  explicit CGAccessibilty() = default;
  ~CGAccessibilty()         = default;

 public:
  NE_COPY_DELETE(CGAccessibilty)

  static UInt64 Width() noexcept { return kHandoverHeader->f_GOP.f_Width; }

  static UInt64 Height() noexcept { return kHandoverHeader->f_GOP.f_Height; }
};
}  // namespace FB

#endif  // !CORE_GFX_ACCESSIBILITY_H_
