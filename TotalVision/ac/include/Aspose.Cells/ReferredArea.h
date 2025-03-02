// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_REFERREDAREA_H
#define ASPOSE_CELLS_REFERREDAREA_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/U16String.h"
#include "Aspose.Cells/Object.h"

namespace Aspose { namespace Cells {

class ReferredArea_Impl;

/// <summary>
/// Represents a referred area by the formula.
/// </summary>
class ASPOSE_CELLS_API2 ReferredArea {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    ReferredArea_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API ReferredArea(ReferredArea_Impl* impl=nullptr);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API ReferredArea(const ReferredArea& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~ReferredArea();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API ReferredArea& operator=(const ReferredArea& src);
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
    /// Indicates whether this is an external link.
    /// </summary>
    ASPOSE_CELLS_API bool IsExternalLink();
    /// <summary>
    /// Get the external file name if this is an external reference.
    /// </summary>
    ASPOSE_CELLS_API U16String GetExternalFileName();
    /// <summary>
    /// Indicates which sheet this reference is in.
    /// </summary>
    ASPOSE_CELLS_API U16String GetSheetName();
    /// <summary>
    /// Indicates whether this area contains all columns(entire row).
    /// </summary>
    ASPOSE_CELLS_API bool IsEntireRow();
    /// <summary>
    /// Indicates whether this area contains all rows(entire column).
    /// </summary>
    ASPOSE_CELLS_API bool IsEntireColumn();
    /// <summary>
    /// Indicates whether this is an area.
    /// </summary>
    /// <remarks>
    /// If this is not an area, only StartRow and StartColumn effect.
    /// </remarks>
    ASPOSE_CELLS_API bool IsArea();
    /// <summary>
    /// The end column of the area.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetEndColumn();
    /// <summary>
    /// The start column of the area.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetStartColumn();
    /// <summary>
    /// The end row of the area.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetEndRow();
    /// <summary>
    /// The start row of the area.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetStartRow();
    /// <summary>
    /// Gets cell values in this area.
    /// </summary>
    /// <returns>If this area is invalid, "#REF!" will be returned;
    /// If this area is one single cell, then return the cell value object;
    /// Otherwise return one 2D array for all values in this area.</returns>
    ASPOSE_CELLS_API Aspose::Cells::Object GetValues();
    /// <summary>
    /// Gets cell values in this area.
    /// </summary>
    /// <param name="calculateFormulas">In this range, if there are some formulas that have not been calculated,
    /// this flag denotes whether those formulas should be calculated recursively</param>
    /// <returns>If this area is invalid, "#REF!" will be returned;
    /// If this area is one single cell, then return the cell value object;
    /// Otherwise return one 2D array for all values in this area.</returns>
    ASPOSE_CELLS_API Aspose::Cells::Object GetValues(bool calculateFormulas);
    /// <summary>
    /// Gets cell value with given offset from the top-left of this area.
    /// </summary>
    /// <param name="rowOffset">row offset from the start row of this area</param>
    /// <param name="colOffset">column offset from the start row of this area</param>
    /// <returns>"#REF!" if this area is invalid;
    /// "#N/A" if given offset out of this area;
    /// Otherwise return the cell value at given position.</returns>
    ASPOSE_CELLS_API Aspose::Cells::Object GetValue(int32_t rowOffset, int32_t colOffset);
    /// <summary>
    /// Gets cell value with given offset from the top-left of this area.
    /// </summary>
    /// <param name="rowOffset">row offset from the start row of this area</param>
    /// <param name="colOffset">column offset from the start row of this area</param>
    /// <param name="calculateFormulas">Whether calculate it recursively if the specified reference is formula</param>
    /// <returns>"#REF!" if this area is invalid;
    /// "#N/A" if given offset out of this area;
    /// Otherwise return the cell value at given position.</returns>
    ASPOSE_CELLS_API Aspose::Cells::Object GetValue(int32_t rowOffset, int32_t colOffset, bool calculateFormulas);
    /// <summary>
    /// Returns the reference address of this area.
    /// Generally it is the address of the reference which may be used in formula, such as "Sheet1!A1:C3".
    /// </summary>
    /// <returns>the reference address of this area.</returns>
    ASPOSE_CELLS_API U16String ToString();
    
};

} }

#endif
