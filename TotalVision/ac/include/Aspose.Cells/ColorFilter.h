﻿// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_COLORFILTER_H
#define ASPOSE_CELLS_COLORFILTER_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/Object.h"
#include "Aspose.Cells/Color.h"

namespace Aspose { namespace Cells {
    class WorksheetCollection;
} }

namespace Aspose { namespace Cells {

class ColorFilter_Impl;

/// <summary>
/// Represents filtering the range by color.
/// </summary>
class ColorFilter {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    ColorFilter_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API ColorFilter(ColorFilter_Impl* impl);
    /// <summary>
    /// Constructs from an Object convertible to this.
    /// </summary>
    ASPOSE_CELLS_API ColorFilter(const Aspose::Cells::Object& obj);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API ColorFilter(const ColorFilter& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~ColorFilter();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API ColorFilter& operator=(const ColorFilter& src);
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
    /// Whether filter by the cell's fill color.
    /// </summary>
    /// <remarks>True: cell's fill color; False: cell's font color.</remarks>
    ASPOSE_CELLS_API bool GetFilterByFillColor();
    /// <summary>
    /// Whether filter by the cell's fill color.
    /// </summary>
    /// <remarks>True: cell's fill color; False: cell's font color.</remarks>
    ASPOSE_CELLS_API void SetFilterByFillColor(bool value);
    /// <summary>
    /// Gets the color of this filter.
    /// </summary>
    /// <param name="sheets"></param>
    /// <returns></returns>
    ASPOSE_CELLS_API Aspose::Cells::Color GetColor(const WorksheetCollection& sheets);
    
};

} }

#endif
