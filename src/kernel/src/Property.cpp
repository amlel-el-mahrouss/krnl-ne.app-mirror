// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <CFKit/Property.h>

namespace Kernel::CF {
/***********************************************************************************/
/// @brief Destructor.
/***********************************************************************************/
Property::~Property() = default;

/***********************************************************************************/
/// @brief Constructor.
/***********************************************************************************/
Property::Property() = default;

/***********************************************************************************/
/// @brief Check if property's name equals to name.
/// @param name string to check.
/***********************************************************************************/
Bool Property::StringEquals(KBasicString<>& name) {
  return this->fName && this->fName == name;
}

/***********************************************************************************/
/// @brief Gets the key (name) of property.
/***********************************************************************************/
KBasicString<>& Property::GetKey() {
  return this->fName;
}

/***********************************************************************************/
/// @brief Gets the value of the property.
/***********************************************************************************/
PropertyId& Property::GetValue() {
  return fValue;
}
}  // namespace Kernel::CF
