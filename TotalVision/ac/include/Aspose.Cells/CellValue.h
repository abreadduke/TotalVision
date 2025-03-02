// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_CELLVALUE_H
#define ASPOSE_CELLS_CELLVALUE_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/Object.h"

#include "Aspose.Cells/CellValueType.h"

namespace Aspose { namespace Cells {

class CellValue_Impl;

/// <summary>
/// Represents the cell value and corresponding type.
/// </summary>
class CellValue {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    CellValue_Impl* _impl;
    
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    ASPOSE_CELLS_API CellValue();
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API CellValue(CellValue_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API CellValue(const CellValue& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~CellValue();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API CellValue& operator=(const CellValue& src);
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
    /// Gets/sets the type of cell value.
    /// </summary>
    ASPOSE_CELLS_API CellValueType GetType();
    /// <summary>
    /// Gets/sets the type of cell value.
    /// </summary>
    ASPOSE_CELLS_API void SetType(CellValueType value);
    /// <summary>
    /// Gets/sets the cell value.
    /// </summary>
    /// <remarks>
    /// The value must be of the correct type of object corresponding to the <see cref="Type"/>:
    /// <list type="table">
    /// <listheader>
    /// <description>Type</description>
    /// <description>Value</description>
    /// </listheader>
    /// <item>
    /// <description><see cref="CellValueType.IsNull"/></description>
    /// <description>null, any other object will be ignored</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsNumeric"/></description>
    /// <description>double</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsDateTime"/></description>
    /// <description>DateTime</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsString"/></description>
    /// <description>string</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsBool"/></description>
    /// <description>bool</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsError"/></description>
    /// <description>error string such as "#VALUE!", "#NAME?", ...</description>
    /// </item>
    /// </list>
    /// </remarks>
    ASPOSE_CELLS_API Aspose::Cells::Object GetValue();
    /// <summary>
    /// Gets/sets the cell value.
    /// </summary>
    /// <remarks>
    /// The value must be of the correct type of object corresponding to the <see cref="Type"/>:
    /// <list type="table">
    /// <listheader>
    /// <description>Type</description>
    /// <description>Value</description>
    /// </listheader>
    /// <item>
    /// <description><see cref="CellValueType.IsNull"/></description>
    /// <description>null, any other object will be ignored</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsNumeric"/></description>
    /// <description>double</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsDateTime"/></description>
    /// <description>DateTime</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsString"/></description>
    /// <description>string</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsBool"/></description>
    /// <description>bool</description>
    /// </item>
    /// <item>
    /// <description><see cref="CellValueType.IsError"/></description>
    /// <description>error string such as "#VALUE!", "#NAME?", ...</description>
    /// </item>
    /// </list>
    /// </remarks>
    ASPOSE_CELLS_API void SetValue(const Aspose::Cells::Object& value);
    
};

} }

#endif
