// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_CUSTOMFILTER_H
#define ASPOSE_CELLS_CUSTOMFILTER_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/Object.h"

#include "Aspose.Cells/FilterOperatorType.h"

namespace Aspose { namespace Cells {

class CustomFilter_Impl;

/// <summary>
/// Represents the custom filter.
/// </summary>
class CustomFilter {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    CustomFilter_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API CustomFilter(CustomFilter_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API CustomFilter(const CustomFilter& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~CustomFilter();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API CustomFilter& operator=(const CustomFilter& src);
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
    /// Gets and sets the filter operator type.
    /// </summary>
    ASPOSE_CELLS_API FilterOperatorType GetFilterOperatorType();
    /// <summary>
    /// Gets and sets the filter operator type.
    /// </summary>
    ASPOSE_CELLS_API void SetFilterOperatorType(FilterOperatorType value);
    /// <summary>
    /// Gets and sets the criteria.
    /// </summary>
    ASPOSE_CELLS_API Aspose::Cells::Object GetCriteria();
    /// <summary>
    /// Gets and sets the criteria.
    /// </summary>
    ASPOSE_CELLS_API void SetCriteria(const Aspose::Cells::Object& value);
    /// <summary>
    /// Sets the filter criteria.
    /// </summary>
    /// <param name="filterOperator">filter operator type</param>
    /// <param name="criteria">filter criteria value</param>
    ASPOSE_CELLS_API void SetCriteria(FilterOperatorType filterOperator, const Aspose::Cells::Object& criteria);
    
};

} }

#endif
