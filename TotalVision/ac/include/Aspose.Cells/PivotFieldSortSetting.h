// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_PIVOT_PIVOTFIELDSORTSETTING_H
#define ASPOSE_CELLS_PIVOT_PIVOTFIELDSORTSETTING_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/U16String.h"

#include "Aspose.Cells/PivotLineType.h"
#include "Aspose.Cells/SortOrder.h"

namespace Aspose { namespace Cells { namespace Pivot {

using namespace Aspose::Cells;

class PivotFieldSortSetting_Impl;

/// <summary>
/// Represents the setting of sorting pivot fields.
/// </summary>
class PivotFieldSortSetting {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    PivotFieldSortSetting_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API PivotFieldSortSetting(PivotFieldSortSetting_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API PivotFieldSortSetting(const PivotFieldSortSetting& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~PivotFieldSortSetting();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API PivotFieldSortSetting& operator=(const PivotFieldSortSetting& src);
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
    
public:
    /// <summary>
    /// Represents the <see cref="SortOrder"/>.
    /// </summary>
    ASPOSE_CELLS_API SortOrder GetSortType();
    /// <summary>
    /// Indicates whether sorting the field by itself or data field.
    /// </summary>
    ASPOSE_CELLS_API bool IsSortByLabels();
    /// <summary>
    /// Represents the index of the field sorted by.
    /// -1 means sorting the PivotField by the labels,others means sorting by the data field.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetFieldIndex();
    /// <summary>
    /// The pivot line type sorted by.
    /// </summary>
    /// <remarks>
    /// Only works when not sorting this field by labels.
    /// </remarks>
    ASPOSE_CELLS_API PivotLineType GetLineTypeSortedBy();
    /// <summary>
    /// Indicates whether a simple data sort operation will be applied.
    /// </summary>
    /// <remarks>
    /// The default value is true.
    /// </remarks>
    ASPOSE_CELLS_API bool IsSimpleSort();
    /// <summary>
    /// Sorts by the values in which row or column.
    /// </summary>
    ASPOSE_CELLS_API U16String GetCell();
    
};

} } }

#endif
