// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_STREAM_H
#define NEKIT_STREAM_H

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>
#include <NeKit/Ref.h>

namespace Kernel {

template <typename StreamTrait, typename Kind>
class Stream final {
 public:
  explicit Stream(Ref<Stream> ref) : fStream(ref) {}
  ~Stream() = default;

  NE_COPY_DEFAULT(Stream)

  template <typename Data>
  friend Stream<StreamTrait, Kind>& operator>>(Stream<StreamTrait, Kind>& Ks, Ref<Data>& Buf) {
    Ks.fKind = Ks.fStream->In(Buf);
    return *Ks;
  }

  template <typename Data>
  friend Stream<StreamTrait, Kind>& operator<<(Stream<StreamTrait, Kind>& Ks, Ref<Data>& Buf) {
    *Ks      = Ks.fStream->Out(Buf.Leak());
    Ks.fKind = Buf;
    return *Ks;
  }

  Ref<StreamTrait>& AsStreamTrait() { return fStream; }

  Ref<Kind>& AsType() { return fKind; }

 private:
  Ref<StreamTrait> fStream;
  Ref<Kind>        fKind;
};

template <class ST, class Kind>
using UnbufferedStream = Stream<ST, Kind>;

template <class BST, class BKind>
using BufferedStream = Stream<BST, BKind>;

}  // namespace Kernel

#endif
