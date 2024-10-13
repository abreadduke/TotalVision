// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.

#ifndef ASPOSE_CELLS_OBJECT_H
#define ASPOSE_CELLS_OBJECT_H

#include <cstdint>

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/ObjectType.h"
#include "Aspose.Cells/U16String.h"
#include "Aspose.Cells/Date.h"

namespace Aspose { namespace Cells { namespace System {
    class Object;
    typedef Object* ObjPtr;
} } }

namespace Aspose { namespace Cells {
    class ReferredArea;
    class ReferredArea_Impl;
} }

namespace Aspose { namespace Cells {
/// <summary>
/// Object class.
/// </summary>
class ASPOSE_CELLS_API2 Object {
public:
    /// <summary>
    /// Default constructor. Constructs an empty object.
    /// </summary>
    ASPOSE_CELLS_API Object();
    /// <summary>
    /// Constructs from a bool value.
    /// </summary>
    ASPOSE_CELLS_API Object(bool value);
    /// <summary>
    /// Constructs from an int8_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(int8_t value);
    /// <summary>
    /// Constructs from an uint8_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(uint8_t value);
    /// <summary>
    /// Constructs from an int16_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(int16_t value);
    /// <summary>
    /// Constructs from an uint16_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(uint16_t value);
    /// <summary>
    /// Constructs from an int32_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(int32_t value);
    /// <summary>
    /// Constructs from an uint32_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(uint32_t value);
    /// <summary>
    /// Constructs from an int64_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(int64_t value);
    /// <summary>
    /// Constructs from an uint64_t value.
    /// </summary>
    ASPOSE_CELLS_API Object(uint64_t value);
    /// <summary>
    /// Constructs from a float value.
    /// </summary>
    ASPOSE_CELLS_API Object(float value);
    /// <summary>
    /// Constructs from a double value.
    /// </summary>
    ASPOSE_CELLS_API Object(double value);
    /// <summary>
    /// Constructs from a Date value.
    /// </summary>
    ASPOSE_CELLS_API Object(Date value);
    /// <summary>
    /// Constructs from U16String value.
    /// </summary>
    ASPOSE_CELLS_API Object(const U16String& value);
    /// <summary>
    /// Constructs from a const char16_t* string.
    /// </summary>
    ASPOSE_CELLS_API Object(const char16_t* value);
    /// <summary>
    /// Internal use
    /// </summary>
    Object(ReferredArea_Impl* value);
    /// <summary>
    /// Internal use
    /// </summary>
    Object(System::ObjPtr value);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API Object(const Object& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~Object();

    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API Object& operator=(const Object& src);
    /// <summary>
    /// Internal use
    /// </summary>
    operator System::ObjPtr() const;

    /// <summary>
    /// Gets the ObjectType of the object.
    /// </summary>
    ASPOSE_CELLS_API ObjectType GetType() const;
    /// <summary>
    /// Gets the NumberType of the object.
    /// </summary>
    ASPOSE_CELLS_API NumberType GetNumberType() const;
    /// <summary>
    /// Checks whether the object is a bool value.
    /// </summary>
    ASPOSE_CELLS_API bool IsBool() const;
    /// <summary>
    /// Checks whether the object is a number value.
    /// </summary>
    ASPOSE_CELLS_API bool IsNumber() const;
    /// <summary>
    /// Checks whether the object is an int32_t value.
    /// </summary>
    ASPOSE_CELLS_API bool IsInt32() const;
    /// <summary>
    /// Checks whether the object is a double value.
    /// </summary>
    ASPOSE_CELLS_API bool IsDouble() const;
    /// <summary>
    /// Checks whether the object is null.
    /// </summary>
    ASPOSE_CELLS_API bool IsNull() const;
    /// <summary>
    /// Checks whether the object is a Date value.
    /// </summary>
    ASPOSE_CELLS_API bool IsDate() const;
    /// <summary>
    /// Checks whether the object is a String value.
    /// </summary>
    ASPOSE_CELLS_API bool IsString() const;
    /// <summary>
    /// Checks whether the object is a ReferredArea pointer.
    /// </summary>
    ASPOSE_CELLS_API bool IsReferredArea() const;
    /// <summary>
    /// Checks whether the object is an object pointer.
    /// </summary>
    ASPOSE_CELLS_API bool IsObject() const;

    /// <summary>
    /// Gets the bool value.
    /// </summary>
    ASPOSE_CELLS_API bool ToBool() const;
    /// <summary>
    /// Gets the int8_t value.
    /// </summary>
    ASPOSE_CELLS_API int8_t ToInt8() const;
    /// <summary>
    /// Gets the uint8_t value.
    /// </summary>
    ASPOSE_CELLS_API uint8_t ToUInt8() const;
    /// <summary>
    /// Gets the int16_t value.
    /// </summary>
    ASPOSE_CELLS_API int16_t ToInt16() const;
    /// <summary>
    /// Gets the uint16_t value.
    /// </summary>
    ASPOSE_CELLS_API uint16_t ToUInt16() const;
    /// <summary>
    /// Gets the int32_t value.
    /// </summary>
    ASPOSE_CELLS_API int32_t ToInt32() const;
    /// <summary>
    /// Gets the uint32_t value.
    /// </summary>
    ASPOSE_CELLS_API uint32_t ToUInt32() const;
    /// <summary>
    /// Gets the int64_t value.
    /// </summary>
    ASPOSE_CELLS_API int64_t ToInt64() const;
    /// <summary>
    /// Gets the uint64_t value.
    /// </summary>
    ASPOSE_CELLS_API uint64_t ToUInt64() const;
    /// <summary>
    /// Gets the float value.
    /// </summary>
    ASPOSE_CELLS_API float ToFloat() const;
    /// <summary>
    /// Gets the double value.
    /// </summary>
    ASPOSE_CELLS_API double ToDouble() const;
    /// <summary>
    /// Converts the object to an int32_t value, if object is number type.
    /// </summary>
    ASPOSE_CELLS_API int32_t AsInt32() const;
    /// <summary>
    /// Converts the object to a double value, if object is number type.
    /// </summary>
    ASPOSE_CELLS_API double AsDouble() const;
    /// <summary>
    /// Gets the String value.
    /// </summary>
    ASPOSE_CELLS_API U16String ToString() const;
    /// <summary>
    /// Gets the Date value.
    /// </summary>
    ASPOSE_CELLS_API Date ToDate() const;
    /// <summary>
    /// Gets the ReferredArea value.
    /// </summary>
    ASPOSE_CELLS_API ReferredArea ToReferredArea() const;

    /// <summary>
    /// Internal use
    /// </summary>
    System::ObjPtr GetObjPtr() const;

private:
    System::ObjPtr  _value;
    uint32_t        _type;

    void checkType(ObjectType expected) const;
    void checkNumberType(NumberType expected) const;
};

} }

#endif
