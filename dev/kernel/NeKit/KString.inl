/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, all rights reserved.

------------------------------------------- */

/// @file BasicKString.inl
/// @brief Kernel String manipulation file.

namespace Kernel {
template <typename CharKind>
inline void ort_string_append(CharKind* lhs, const CharKind* rhs, Int32 cur) {
  SizeT sz_rhs = ort_string_len<CharKind>(rhs);
  SizeT rhs_i  = 0;

  for (; rhs_i < sz_rhs; ++rhs_i) {
    lhs[rhs_i + cur] = rhs[rhs_i];
  }
}

template <typename CharKind>
inline CharKind* BasicKString<CharKind>::Data() {
  return this->fData;
}

template <typename CharKind>
inline const CharKind* BasicKString<CharKind>::CData() const {
  return const_cast<const CharKind*>(this->fData);
}

template <typename CharKind>
inline SizeT BasicKString<CharKind>::Length() const {
  return this->fDataSz;
}

template <typename CharKind>
inline bool BasicKString<CharKind>::operator==(const BasicKString<CharKind>& rhs) const {
  if (rhs.Length() != this->Length()) return false;

  for (Size index = 0; index < this->Length(); ++index) {
    if (rhs.fData[index] != this->fData[index]) return false;
  }

  return true;
}

template <typename CharKind>
inline bool BasicKString<CharKind>::operator==(const CharKind* rhs) const {
  if (ort_string_len<CharKind>(rhs) != this->Length()) return false;

  for (Size index = 0; index < ort_string_len<CharKind>(rhs); ++index) {
    if (rhs[index] != this->fData[index]) return false;
  }

  return true;
}

template <typename CharKind>
inline bool BasicKString<CharKind>::operator!=(const BasicKString<CharKind>& rhs) const {
  if (rhs.Length() != this->Length()) return false;

  for (Size index = 0; index < rhs.Length(); ++index) {
    if (rhs.fData[index] == this->fData[index]) return false;
  }

  return true;
}

template <typename CharKind>
inline bool BasicKString<CharKind>::operator!=(const CharKind* rhs) const {
  if (ort_string_len<CharKind>(rhs) != this->Length()) return false;

  for (Size index = 0; index < ort_string_len<CharKind>(rhs); ++index) {
    if (rhs[index] == this->fData[index]) return false;
  }

  return true;
}

template <typename CharKind>
inline BasicKString<CharKind>& BasicKString<CharKind>::operator+=(const BasicKString<CharKind>& rhs) {
  if (ort_string_len<CharKind>(rhs.fData) > this->Length()) return *this;

  ort_string_append(this->fData, const_cast<CharKind*>(rhs.fData), this->fCur);
  this->fCur += ort_string_len<CharKind>(const_cast<CharKind*>(rhs.fData));

  return *this;
}

template <typename CharKind>
inline BasicKString<CharKind>& BasicKString<CharKind>::operator+=(const CharKind* rhs) {
  ort_string_append(this->fData, const_cast<CharKind*>(rhs), this->fCur);
  this->fCur += ort_string_len<CharKind>(const_cast<CharKind*>(rhs));

  return *this;
}

template <typename CharKind>
inline ErrorOr<BasicKString<CharKind>> KStringBuilder::Construct(const CharKind* data) {
  if (!data || *data == 0) return ErrorOr<BasicKString<CharKind>>(nullptr);

  BasicKString<CharKind>* view = new BasicKString<CharKind>(ort_string_len<CharKind>(data));
  (*view) += data;

  return ErrorOr<BasicKString<CharKind>>(*view);
}
template <typename CharKind>
inline const CharKind* KStringBuilder::FromBool(const CharKind* fmt, bool i) {
  if (!fmt) return ("?");

  const CharKind* boolean_expr = i ? "YES" : "NO";
  CharKind*       ret          = (CharKind*) RTL_ALLOCA(ort_string_len<CharKind>(boolean_expr) + ort_string_len<CharKind>(fmt));

  if (!ret) return ("?");

  const auto fmt_len = ort_string_len<CharKind>(fmt);
  const auto res_len = ort_string_len<CharKind>(boolean_expr);

  for (Size idx = 0; idx < fmt_len; ++idx) {
    if (fmt[idx] == '%') {
      SizeT result_cnt = idx;

      for (auto y_idx = idx; y_idx < res_len; ++y_idx) {
        ret[result_cnt] = boolean_expr[y_idx];
        ++result_cnt;
      }

      break;
    }

    ret[idx] = fmt[idx];
  }

  return ret;
}
template <typename CharKind>
inline bool KStringBuilder::Equals(const CharKind* lhs, const CharKind* rhs) {
  if (ort_string_len<CharKind>(rhs) != ort_string_len<CharKind>(lhs)) return false;

  for (Size index = 0; index < ort_string_len<CharKind>(rhs); ++index) {
    if (rhs[index] != lhs[index]) return false;
  }

  return true;
}
template <typename CharKind>
inline const CharKind* KStringBuilder::Format(const CharKind* fmt, const CharKind* fmt2) {
  if (!fmt || !fmt2) return ("?");

  CharKind* ret = (CharKind*) RTL_ALLOCA(sizeof(char) * (ort_string_len<CharKind>(fmt2) + ort_string_len<CharKind>(fmt)));

  if (!ret) return ("?");

  const auto len = ort_string_len<CharKind>(fmt);

  for (Size idx = 0; idx < len; ++idx) {
    if (fmt[idx] == '%' && idx < ort_string_len<CharKind>(fmt) && fmt[idx] == 's') {
      Size result_cnt = idx;

      for (Size y_idx = 0; y_idx < ort_string_len<CharKind>(fmt2); ++y_idx) {
        ret[result_cnt] = fmt2[y_idx];
        ++result_cnt;
      }
    }

    ret[idx] = fmt[idx];
  }

  return ret;
}
}  // namespace Kernel
