// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_MERGEDCELLSSHRINKTYPE_H
#define ASPOSE_CELLS_MERGEDCELLSSHRINKTYPE_H

namespace Aspose { namespace Cells {

/// <summary>
/// Represents the strategy to shrink merged cells for operations such as deleting blank rows/column.
/// </summary>
enum class MergedCellsShrinkType {
    /// <summary>
    /// Leaves the merged cells as it is without any modification.
    /// </summary>
    None,
    /// <summary>
    /// Shrinks the merged area if needed, by removing rows from the bottom or columns from the right,
    /// while ensuring all content remains visible.
    /// </summary>
    ShrinkToFit,
    /// <summary>
    /// Only keeps the header rows/columns of the merged area if needed.
    /// </summary>
    KeepHeaderOnly,
};

} }

#endif
