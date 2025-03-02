// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_DRAWING_EQUATIONS_ARRAYEQUATIONNODE_H
#define ASPOSE_CELLS_DRAWING_EQUATIONS_ARRAYEQUATIONNODE_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/EquationNode.h"
#include "Aspose.Cells/Object.h"

namespace Aspose { namespace Cells { namespace Drawing { namespace Equations {

using namespace Aspose::Cells;

class ArrayEquationNode_Impl;

/// <summary>
/// Specifies the Equation-Array function, an object consisting of one or more equations.
/// </summary>
class ArrayEquationNode : public EquationNode {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    ArrayEquationNode_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API ArrayEquationNode(ArrayEquationNode_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API ArrayEquationNode(const ArrayEquationNode& src);
    /// <summary>
    /// Constructs from a parent object.
    /// </summary>
    ASPOSE_CELLS_API ArrayEquationNode(const EquationNode& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~ArrayEquationNode();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API ArrayEquationNode& operator=(const ArrayEquationNode& src);
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
    /// Determine whether the current equation node is equal to the specified node
    /// </summary>
    /// <param name="obj">The specified node</param>
    /// <returns></returns>
    ASPOSE_CELLS_API bool Equals(const Aspose::Cells::Object& obj);
    
};

} } } }

#endif
