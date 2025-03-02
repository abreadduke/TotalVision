// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_SORTORDER_H
#define ASPOSE_CELLS_SORTORDER_H

namespace Aspose { namespace Cells {

/// <summary>
/// Represents sort order for the data range.
/// </summary>
enum class SortOrder {
    /// <summary>
    /// Sorts data in ascending order
    /// </summary>
    Ascending = 0,
    /// <summary>
    /// Sorts data in descending order
    /// </summary>
    Descending = 1,
    /// <summary>
    /// Keeps original data order without sorting. Only applies to some special scenarios such as PivotTable.
    /// </summary>
    Natural = 2,
};

} }

#endif
