// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_DRAWING_EQUATIONS_EQUATIONNODETYPE_H
#define ASPOSE_CELLS_DRAWING_EQUATIONS_EQUATIONNODETYPE_H

namespace Aspose { namespace Cells { namespace Drawing { namespace Equations {

/// <summary>
/// Equation node type.
/// Notice:
/// (1)[1-99] Currently there is only one node in the scope, and its enumeration value is 1. The node it specifies is used to store mathematical text.
/// (2)[100-199] Indicates that the node is a component of some special function nodes.
/// (3)[200-] Indicates that the node has some special functions.
/// </summary>
enum class EquationNodeType {
    /// <summary>
    /// UnKnow
    /// </summary>
    UnKnow = 0,
    /// <summary>
    /// specifies a node that stores math text
    /// </summary>
    Text = 1,
    /// <summary>
    /// Specifies a Base component
    /// </summary>
    Base = 100,
    /// <summary>
    /// Specifies a Denominator component
    /// </summary>
    Denominator = 101,
    /// <summary>
    /// Specifies a Numerator component
    /// </summary>
    Numerator = 102,
    /// <summary>
    /// Specifies a FunctionName component
    /// </summary>
    FunctionName = 103,
    /// <summary>
    /// Specifies a Subscript component
    /// </summary>
    Subscript = 104,
    /// <summary>
    /// Specifies a Superscript component
    /// </summary>
    Superscript = 105,
    /// <summary>
    /// Specifies a Degree component
    /// </summary>
    Degree = 106,
    /// <summary>
    /// Specifies a MatrixRow component.A single row of the matrix
    /// </summary>
    MatrixRow = 107,
    /// <summary>
    /// Represents a sub-object of Lower-Limit function or Upper-Limit function
    /// </summary>
    Limit = 108,
    /// <summary>
    /// Specifies a mathematical paragraph(oMathPara).
    /// </summary>
    EquationParagraph = 200,
    /// <summary>
    /// Specifies an equation or mathematical expression(OMath).
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Mathematical.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Mathematical instead.
    MathematicalEquation [[deprecated]] = 201,
    /// <summary>
    /// Specifies fractional equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Fraction.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Fraction instead.
    FractionEquation [[deprecated]] = 202,
    /// <summary>
    /// Specifies function equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Function.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Function instead.
    FunctionEquation [[deprecated]] = 203,
    /// <summary>
    /// Specifies delimiter equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Delimiter.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Delimiter instead.
    DelimiterEquation [[deprecated]] = 204,
    /// <summary>
    /// Specifies n-ary operator equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Nary.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Nary instead.
    NaryEquation [[deprecated]] = 205,
    /// <summary>
    /// Specifies the radical equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Radical.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Radical instead.
    RadicalEquation [[deprecated]] = 206,
    /// <summary>
    /// Specifies superscript equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Sup.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Sup instead.
    SuperscriptEquation [[deprecated]] = 207,
    /// <summary>
    /// Specifies subscript equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Sub.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Sub instead.
    SubscriptEquation [[deprecated]] = 208,
    /// <summary>
    /// Specifies an equation with superscripts and subscripts to the right of the operands.
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.SubSup.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.SubSup instead.
    SubSupEquation [[deprecated]] = 209,
    /// <summary>
    /// Specifies an equation with superscripts and subscripts to the left of the operands.
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.PreSubSup.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.PreSubSup instead.
    PreSubSupEquation [[deprecated]] = 210,
    /// <summary>
    /// Specifies accent equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Accent.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Accent instead.
    AccentEquation [[deprecated]] = 211,
    /// <summary>
    /// Specifies bar equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Bar.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Bar instead.
    BarEquation [[deprecated]] = 212,
    /// <summary>
    /// Specifies border box equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.BorderBox.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.BorderBox instead.
    BorderBoxEquation [[deprecated]] = 213,
    /// <summary>
    /// Specifies box equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Box.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Box instead.
    BoxEquation [[deprecated]] = 214,
    /// <summary>
    /// Specifies Group-Character equation
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.GroupChr.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.GroupChr instead.
    GroupCharacterEquation [[deprecated]] = 215,
    /// <summary>
    /// Specifies the Matrix equation,
    /// </summary>
    /// <remarks>NOTE: This member is now obsolete. Instead, please use EquationNodeType.Matrix.
    /// This property will be removed 12 months later since September 2024.
    /// Aspose apologizes for any inconvenience you may have experienced.</remarks>
    /// @deprecated Use EquationNodeType.Matrix instead.
    MatrixEquation [[deprecated]] = 216,
    /// <summary>
    /// Specifies the Lower-Limit function
    /// </summary>
    LowerLimit = 217,
    /// <summary>
    /// Specifies the Upper-Limit function
    /// </summary>
    UpperLimit = 218,
    /// <summary>
    /// Specifies an equation or mathematical expression(OMath).
    /// </summary>
    Mathematical = 201,
    /// <summary>
    /// Specifies fractional equation
    /// </summary>
    Fraction = 202,
    /// <summary>
    /// Specifies function equation
    /// </summary>
    Function = 203,
    /// <summary>
    /// Specifies delimiter equation
    /// </summary>
    Delimiter = 204,
    /// <summary>
    /// Specifies n-ary operator equation
    /// </summary>
    Nary = 205,
    /// <summary>
    /// Specifies the radical equation
    /// </summary>
    Radical = 206,
    /// <summary>
    /// Specifies superscript equation
    /// </summary>
    Sup = 207,
    /// <summary>
    /// Specifies subscript equation
    /// </summary>
    Sub = 208,
    /// <summary>
    /// Specifies an equation with superscripts and subscripts to the right of the operands.
    /// </summary>
    SubSup = 209,
    /// <summary>
    /// Specifies an equation with superscripts and subscripts to the left of the operands.
    /// </summary>
    PreSubSup = 210,
    /// <summary>
    /// Specifies accent equation
    /// </summary>
    Accent = 211,
    /// <summary>
    /// Specifies bar equation
    /// </summary>
    Bar = 212,
    /// <summary>
    /// Specifies border box equation
    /// </summary>
    BorderBox = 213,
    /// <summary>
    /// Specifies box equation
    /// </summary>
    Box = 214,
    /// <summary>
    /// Specifies Group-Character equation
    /// </summary>
    GroupChr = 215,
    /// <summary>
    /// Specifies the Matrix equation,
    /// </summary>
    Matrix = 216,
    /// <summary>
    /// Specifies the Equation-Array function. The function consists of one or more equations.
    /// </summary>
    ArrayEquation = 317,
};

} } } }

#endif
