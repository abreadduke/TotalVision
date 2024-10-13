// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_PIVOT_PIVOTTABLESOURCETYPE_H
#define ASPOSE_CELLS_PIVOT_PIVOTTABLESOURCETYPE_H

namespace Aspose { namespace Cells { namespace Pivot {

/// <summary>
/// Represents data source type of the pivot table.
/// </summary>
enum class PivotTableSourceType : uint8_t {
    /// <summary>
    /// Specifies that the source data is a range.
    /// </summary>
    Sheet = 0x01,
    /// <summary>
    /// Specifies that external source data is used.
    /// </summary>
    External = 0x02,
    /// <summary>
    /// Specifies that multiple consolidation ranges are used as the source data.
    /// </summary>
    Consolidation = 0x04,
    /// <summary>
    /// The source data is populated from a temporary internal structure.
    /// </summary>
    Scenario = 0x08,
    /// <summary>
    /// Unknown data source.
    /// </summary>
    Unknown,
};

} } }

#endif
