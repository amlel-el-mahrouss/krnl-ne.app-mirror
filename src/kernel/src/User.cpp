// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/kernel

#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>
#include <KernelKit/KPC.h>
#include <KernelKit/ThreadLocalStorage.h>
#include <KernelKit/UserMgr+User.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

/// @file UserMgr+User.cpp
/// @brief Multi-user support.

namespace Kernel {

namespace Detail {
  ////////////////////////////////////////////////////////////
  /// \brief Constructs a password by hashing the password.
  /// \param password password to hash.
  /// \return the hashed password
  ////////////////////////////////////////////////////////////
  STATIC UInt64 user_fnv_generator(const Char* password, User* user) {
    kout << "user_fnv_generator: Try hashing user password...\r";

    if (!password || !user) return 0;
    if (*password == 0) return 0;

    const UInt64 kFnvOffsetBasis = 0xcbf29ce484222325ULL;
    const UInt64 kFnvPrime       = 0x100000001b3ULL;

    UInt64 hash = kFnvOffsetBasis;

    while (*password) {
      hash ^= (Char) (*password++);
      hash *= kFnvPrime;
    }

    kout << "user_fnv_generator: Hashed user password.\r";

    return hash;
  }
}  // namespace Detail

////////////////////////////////////////////////////////////
/// @brief User ring constructor.
////////////////////////////////////////////////////////////
User::User(const Int32& sel, const Char* user_name) : mUserRing((UserRingKind) sel) {
  MUST_PASS(sel >= 0);
  rt_copy_memory_safe((VoidPtr) user_name, this->mUserName, rt_string_len(user_name),
                      kMaxUserNameLen);
}

////////////////////////////////////////////////////////////
/// @brief User ring constructor.
////////////////////////////////////////////////////////////
User::User(const UserRingKind& ring_kind, const Char* user_name) : mUserRing(ring_kind) {
  rt_copy_memory_safe((VoidPtr) user_name, this->mUserName, rt_string_len(user_name),
                      kMaxUserNameLen);
}

////////////////////////////////////////////////////////////
/// @brief User destructor class.
////////////////////////////////////////////////////////////
User::~User() = default;

Bool User::Save(const UserPublicKey password) {
  if (!password || *password == 0) return No;

  this->mUserFNV = Detail::user_fnv_generator(password, this);

  kout << "User::Save: Saved password successfully...\r";

  return Yes;
}

Bool User::Login(const UserPublicKey password) {
  if (!password || !*password) return No;

  auto ret = this->mUserFNV == Detail::user_fnv_generator(password, this);

  // now check if the password matches.
  kout << (ret ? "User::Login: Password matches.\r" : "User::Login: Password doesn't match.\r");
  return ret;
}

Bool User::operator==(const User& lhs) {
  return lhs.mUserRing == this->mUserRing;
}

Bool User::operator!=(const User& lhs) {
  return lhs.mUserRing != this->mUserRing;
}

////////////////////////////////////////////////////////////
/// @brief Returns the user's name.
////////////////////////////////////////////////////////////

Char* User::Name() {
  return this->mUserName;
}

////////////////////////////////////////////////////////////
/// @brief Returns the user's ring.
/// @return The king of ring the user is attached to.
////////////////////////////////////////////////////////////

const UserRingKind& User::Ring() {
  return this->mUserRing;
}

Bool User::IsStdUser() {
  return this->Ring() == UserRingKind::kRingStdUser;
}

Bool User::IsSuperUser() {
  return this->Ring() == UserRingKind::kRingSuperUser;
}

}  // namespace Kernel
