﻿// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_DRAWING_EQUATIONS_EQUATIONCOMPONENTNODE_H
#define ASPOSE_CELLS_DRAWING_EQUATIONS_EQUATIONCOMPONENTNODE_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/EquationNode.h"
#include "Aspose.Cells/Object.h"

namespace Aspose { namespace Cells { namespace Drawing { namespace Equations {

using namespace Aspose::Cells;

class EquationComponentNode_Impl;

/// <summary>
/// This class specifies the components of an equation or mathematical expression.
/// Different types of components combined into different equations.
/// For example, a fraction consists of two parts, a numerator component and a denominator component.
/// For more component types, please refer to 'EquationNodeType'.
/// </summary>
class EquationComponentNode : public EquationNode {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    EquationComponentNode_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API EquationComponentNode(EquationComponentNode_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API EquationComponentNode(const EquationComponentNode& src);
    /// <summary>
    /// Constructs from a parent object.
    /// </summary>
    ASPOSE_CELLS_API EquationComponentNode(const EquationNode& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~EquationComponentNode();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API EquationComponentNode& operator=(const EquationComponentNode& src);
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
