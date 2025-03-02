// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_CALCULATIONDATA_H
#define ASPOSE_CELLS_CALCULATIONDATA_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/Object.h"
#include "Aspose.Cells/U16String.h"

namespace Aspose { namespace Cells {
    class Workbook;
    class Worksheet;
    class Cell;
} }

namespace Aspose { namespace Cells {

class CalculationData_Impl;

/// <summary>
/// Represents the required data when calculating one function, such as function name, parameters, ...etc.
/// </summary>
/// <remarks>All objects provided by this class are for "read" purpose only.
/// User should not change any data in the Workbook during the formula calculation process,
/// Otherwise unexpected result or Exception may be caused.</remarks>
class CalculationData {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    CalculationData_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API CalculationData(CalculationData_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API CalculationData(const CalculationData& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~CalculationData();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API CalculationData& operator=(const CalculationData& src);
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
    /// Gets or sets the calculated value for this function.
    /// </summary>
    /// <remarks>User should set this property in his custom calculation engine for those functions the engine supports,
    /// and the set value will be returned when getting this property later.
    /// The set value may be of possible types of <see cref="Cell.Value"/>,
    /// or array of such kind of values, or a Range, Name, ReferredArea.
    /// Getting this property before setting value to it will make the function be calculated
    /// by the default calculation engine of Aspose.Cells and then the calculated value will
    /// be returned(generally it should be #NAME? for user-defined functions).
    /// </remarks>
    ASPOSE_CELLS_API Aspose::Cells::Object GetCalculatedValue();
    /// <summary>
    /// Gets or sets the calculated value for this function.
    /// </summary>
    /// <remarks>User should set this property in his custom calculation engine for those functions the engine supports,
    /// and the set value will be returned when getting this property later.
    /// The set value may be of possible types of <see cref="Cell.Value"/>,
    /// or array of such kind of values, or a Range, Name, ReferredArea.
    /// Getting this property before setting value to it will make the function be calculated
    /// by the default calculation engine of Aspose.Cells and then the calculated value will
    /// be returned(generally it should be #NAME? for user-defined functions).
    /// </remarks>
    ASPOSE_CELLS_API void SetCalculatedValue(const Aspose::Cells::Object& value);
    /// <summary>
    /// Gets the Workbook object where the function is in.
    /// </summary>
    ASPOSE_CELLS_API Workbook GetWorkbook();
    /// <summary>
    /// Gets the Worksheet object where the function is in.
    /// </summary>
    ASPOSE_CELLS_API Worksheet GetWorksheet();
    /// <summary>
    /// Gets the row index of the cell where the function is in.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetCellRow();
    /// <summary>
    /// Gets the column index of the cell where the function is in.
    /// </summary>
    ASPOSE_CELLS_API int32_t GetCellColumn();
    /// <summary>
    /// Gets the Cell object where the function is in.
    /// </summary>
    /// <remarks>When calculating a formula without setting it to a cell,
    /// such as by <see cref="Worksheet.CalculateFormula(string, CalculationOptions)"/>,
    /// the formula will be calculated just like it has been set to cell A1,
    /// so both <see cref="CellRow"/> and <see cref="CellColumn"/> are 0.
    /// However, cell A1 in the worksheet may has not been instantiated.
    /// So for such kind of situation this property will be null.</remarks>
    ASPOSE_CELLS_API Cell GetCell();
    /// <summary>
    /// Gets the function name to be calculated.
    /// </summary>
    ASPOSE_CELLS_API U16String GetFunctionName();
    /// <summary>
    /// Gets the count of parameters
    /// </summary>
    ASPOSE_CELLS_API int32_t GetParamCount();
    /// <summary>
    /// Gets the represented value object of the parameter at given index.
    /// </summary>
    /// <param name="index">The index of the parameter(0 based)</param>
    /// <returns>The calculated value of the parameter.</returns>
    /// <remarks>
    /// For one parameter:
    /// <p>If it is plain value, then returns the plain value itself;</p>
    /// <p>If it is reference, then returns ReferredArea object;</p>
    /// <p>If it references to dataset(s) with multiple values, then returns array of objects;</p>
    /// <p>
    /// If it is some kind of expression that needs to be calculated, then it will be calculated in value mode
    /// and generally a single value will be returned according to current cell base. For example,
    /// if one parameter of D2's formula is A:A+B:B, then A2+B2 will be calculated and returned.
    /// However, if this parameter has been specified as array mode
    /// (by <see cref="Workbook.UpdateCustomFunctionDefinition(CustomFunctionDefinition)"/>
    /// or <see cref="FormulaParseOptions.CustomFunctionDefinition"/>),
    /// then an array(object[][]) will be returned whose items are A1+B1,A2+B2,....
    /// </p>
    /// </remarks>
    ASPOSE_CELLS_API Aspose::Cells::Object GetParamValue(int32_t index);
    /// <summary>
    /// Gets the literal text of the parameter at given index.
    /// </summary>
    /// <param name="index">index of the parameter(0 based)</param>
    /// <returns>literal text of the parameter</returns>
    ASPOSE_CELLS_API U16String GetParamText(int32_t index);
    
};

} }

#endif
