/* -------------------------------------------

  Copyright Amlal El Mahrouss.

------------------------------------------- */

#pragma once

/// @file MathMgr.h
/// @brief Linear interpolation implementation.

namespace UI {
#ifdef NE_CORE_GFX_USE_DOUBLE
typedef double cg_real_t;
#else
typedef float cg_real_t;
#endif

/// @brief Linear interpolation equation solver.
/// @param from where to start
/// @param to to which value.
/// @param stat
/// @return Linear interop value.
inline cg_real_t cg_math_lerp(cg_real_t to, cg_real_t from, cg_real_t stat) {
  return (from) + (to - from) * stat;
}
}  // namespace UI