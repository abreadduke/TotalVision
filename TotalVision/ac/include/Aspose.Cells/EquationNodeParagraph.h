﻿// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_DRAWING_EQUATIONS_EQUATIONNODEPARAGRAPH_H
#define ASPOSE_CELLS_DRAWING_EQUATIONS_EQUATIONNODEPARAGRAPH_H

#include "Aspose.Cells/Export.h"
#include "Aspose.Cells/EquationNode.h"
#include "Aspose.Cells/Object.h"

#include "Aspose.Cells/EquationHorizontalJustificationType.h"

namespace Aspose { namespace Cells { namespace Drawing { namespace Equations {

using namespace Aspose::Cells;

class EquationNodeParagraph_Impl;

/// <summary>
/// This class specifies a mathematical paragraph containing one or more MathEquationNode(OMath) elements.
/// </summary>
class EquationNodeParagraph : public EquationNode {
public:
    /// <summary>
    /// The implementation object.
    /// </summary>
    EquationNodeParagraph_Impl* _impl;
    
public:
    /// <summary>
    /// Constructs from an implementation object.
    /// </summary>
    ASPOSE_CELLS_API EquationNodeParagraph(EquationNodeParagraph_Impl* impl);
    /// <summary>
    /// Copy constructor.
    /// </summary>
    ASPOSE_CELLS_API EquationNodeParagraph(const EquationNodeParagraph& src);
    /// <summary>
    /// Constructs from a parent object.
    /// </summary>
    ASPOSE_CELLS_API EquationNodeParagraph(const EquationNode& src);
    /// <summary>
    /// Destructor.
    /// </summary>
    ASPOSE_CELLS_API ~EquationNodeParagraph();
    /// <summary>
    /// operator=
    /// </summary>
    ASPOSE_CELLS_API EquationNodeParagraph& operator=(const EquationNodeParagraph& src);
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
    /// This specifies justification of the math paragraph (a series of adjacent equations within the same paragraph). A math paragraph can be Left Justified, Right Justified, Centered, or Centered as Group. By default, the math paragraph is Centered as Group. This means that the equations can be aligned with respect to each other, but the entire group of equations is centered as a whole.
    /// </summary>
    ASPOSE_CELLS_API EquationHorizontalJustificationType GetJustification();
    /// <summary>
    /// This specifies justification of the math paragraph (a series of adjacent equations within the same paragraph). A math paragraph can be Left Justified, Right Justified, Centered, or Centered as Group. By default, the math paragraph is Centered as Group. This means that the equations can be aligned with respect to each other, but the entire group of equations is centered as a whole.
    /// </summary>
    ASPOSE_CELLS_API void SetJustification(EquationHorizontalJustificationType value);
    /// <summary>
    /// Determine whether the current equation node is equal to the specified node
    /// </summary>
    /// <param name="obj">The specified node</param>
    /// <returns></returns>
    ASPOSE_CELLS_API bool Equals(const Aspose::Cells::Object& obj);
    
};

} } } }

#endif
