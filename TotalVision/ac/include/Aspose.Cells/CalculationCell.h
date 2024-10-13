// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_CALCULATIONCELL_H
#define ASPOSE_CELLS_CALCULATIONCELL_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/Object.h"

namespace Aspose { namespace Cells {
    class Workbook;
    class Worksheet;
    class Cell;
} }

namespace Aspose { namespace Cells {

class CalculationCell_Impl;

/// <summary>
/// Represents the calculation relevant data about one cell which is being calculated.
/// </summary>
/// <remarks>All objects provided by this class are for "read" purpose only.
/// User should not change any data in the Workbook during the formula calculation process,
/// Otherwise unexpected result or Exception may be caused.
/// </remarks>
class CalculationCell {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    CalculationCell_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API CalculationCell(CalculationCell_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API CalculationCell(const CalculationCell& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~CalculationCell();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API CalculationCell& operator=(const CalculationCell& src);
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
    /// Sets the calculated value for the cell.
    /// </summary>
    /// <remarks>User can set the calculated result by this method to ignore the automatic calculation for the cell.</remarks>
    ASPOSE_CELLS_API void SetCalculatedValue(const Aspose::Cells::Object& v);
    /// <summary>
    /// Gets the Workbook object.
    /// </summary>
    ASPOSE_CELLS_API Workbook GetWorkbook();
    /// <summary>
    /// Gets the Worksheet object where the cell is in.
    /// </summary>
    ASPOSE_CELLS_API Worksheet GetWorksheet();
    /// <summary>
    /// Gets the row index of the cell.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetCellRow();
    /// <summary>
    /// Gets the column index of the cell.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetCellColumn();
    /// <summary>
    /// Gets the Cell object which is being calculated.
    /// </summary>
    ASPOSE_CELLS_API Cell GetCell();
    
};

} }

#endif
