﻿// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_ICONFILTER_H
#define ASPOSE_CELLS_ICONFILTER_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/Object.h"

#include "Aspose.Cells/IconSetType.h"

namespace Aspose { namespace Cells {

class IconFilter_Impl;

/// <summary>
/// Represents icon filter.
/// </summary>
class IconFilter {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    IconFilter_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API IconFilter(IconFilter_Impl* impl);
    /// <summary>
    /// Constructs from an Object convertible to this.
    /// </summary>
    ASPOSE_CELLS_API IconFilter(const Aspose::Cells::Object& obj);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API IconFilter(const IconFilter& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~IconFilter();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API IconFilter& operator=(const IconFilter& src);
    /// <summary>
    /// operator bool()
    /// </summary>
    /// <returns>Returns true if the implementation object is not nullptr. Otherwise, returns false</returns>
    ASPOSE_CELLS_API explicit operator bool() const { return _impl != nullptr; }
    /// <summary>
    /// Checks whether the implementation object is nullptr.
    /// </summary>
    /// <returns>Returns true if the implementation object is nullptr. Otherwise, returns false</returns>
    ASPOSE_CELLS_API bool IsNull() const { return _impl == nullptr; }
    /// <summary>
    /// Gets the Object.
    /// </summary>
    ASPOSE_CELLS_API Aspose::Cells::Object ToObject();
    
public:
    /// <summary>
    /// Gets and sets which icon set is used in the filter criteria.
    /// </summary>
    ASPOSE_CELLS_API IconSetType GetIconSetType();
    /// <summary>
    /// Gets and sets which icon set is used in the filter criteria.
    /// </summary>
    ASPOSE_CELLS_API void SetIconSetType(IconSetType value);
    /// <summary>
    /// Gets and sets Zero-based index of an icon in an icon set.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetIconId();
    /// <summary>
    /// Gets and sets Zero-based index of an icon in an icon set.
    /// </summary>
    ASPOSE_CELLS_API void SetIconId(int32_t value);
    
};

} }

#endif
