// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef __KERNEL_KIT_USER_MGR_H__
#define __KERNEL_KIT_USER_MGR_H__

/* ========================================

 Revision History:

 04/03/25: Set users directory as /libSystem/ instead of /usr/

 ======================================== */

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/KPC.h>
#include <NeKit/Config.h>
#include <NeKit/KString.h>

///! We got the MGMT, STD (%s format) and GUEST users,
///! all are used to make authorized operations.
#define kMgmtUser "NEKERNEL/MGMT/%s"
#define kGuestUser "NEKERNEL/GUEST/%s"
#define kStdUser "NEKERNEL/STD/%s"

#define kUsersDir "/users/"

#define kMaxUserNameLen (256U)
#define kMaxUserTokenLen (256U)

namespace Kernel {
class User;

enum struct UserRingKind : Int32 {
  kRingInvalid   = 0,
  kRingStdUser   = 444,
  kRingSuperUser = 666,
  kRingGuestUser = 777,
  kRingCount     = 3,
};

typedef Char* UserPublicKey;
typedef Char  UserPublicKeyType;

/// @brief System User class.
class User final {
 public:
  User() = delete;

  User(const Int32& sel, const Char* username);
  User(const UserRingKind& kind, const Char* username);

  ~User();

 public:
  NE_COPY_DEFAULT(User)

 public:
  bool operator==(const User& lhs);
  bool operator!=(const User& lhs);

 public:
  /// @brief Get software ring
  const UserRingKind& Ring();

  /// @brief Get user name
  Char* Name();

  /// @brief Is he a standard user?
  Bool IsStdUser();

  /// @brief Is she a super user?
  Bool IsSuperUser();

  /// @brief Saves a password from the public key.
  Bool Save(const UserPublicKey password);

  /// @brief Checks if a password matches the **password**.
  /// @param password the password to check.
  Bool Login(const UserPublicKey password);

 private:
  UserRingKind mUserRing{UserRingKind::kRingStdUser};
  Char         mUserName[kMaxUserNameLen] = {0};
  UInt64       mUserFNV{0UL};
};

/// \brief Alias for user ptr.
using UserPtr = User*;

/// \brief Current running user.
inline User* kCurrentUser = nullptr;

/// \brief Super user.
inline User* kRootUser = nullptr;
}  // namespace Kernel

#endif /* ifndef __KERNEL_KIT_USER_MGR_H__ */
