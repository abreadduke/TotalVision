// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.

#ifndef ASPOSE_CELLS_OBJECTTYPE_H
#define ASPOSE_CELLS_OBJECTTYPE_H

namespace Aspose { namespace Cells {
    /// <summary>
    /// Represents the type of the object.
    /// </summary>
    enum class ObjectType {
        Null = 0,
        Bool,
        Number,
        Date,
        String,
        ReferredArea,
        Array2D,
        Object
    };

    /// <summary>
    /// Represents the type of the number.
    /// </summary>
    enum class NumberType {
        None = 0,
        Int8,
        UInt8,
        Int16,
        UInt16,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Float,
        Double
    };

} }

#endif
