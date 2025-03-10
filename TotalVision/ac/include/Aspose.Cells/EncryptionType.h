﻿// Copyright (c) 2001-2024 Aspose Pty Ltd. All Rights Reserved.
// Powered by Aspose.Cells.
#ifndef ASPOSE_CELLS_ENCRYPTIONTYPE_H
#define ASPOSE_CELLS_ENCRYPTIONTYPE_H

namespace Aspose { namespace Cells {

/// <summary>
/// Encryption Type.
/// Only used by excel2003.
/// We will encrypt 2007/2010 workbook using SHA AES the same as Excel does, and this EncryptionType will be ignored.
/// </summary>
enum class EncryptionType {
    /// <summary>
    /// XOR encryption algorithm.
    /// </summary>
    XOR,
    /// <summary>
    /// Office 97/2000 compatible.
    /// </summary>
    Compatible,
    /// <summary>
    /// Enhanced encryption.
    /// </summary>
    EnhancedCryptographicProviderV1,
    /// <summary>
    /// Strong encryption algorithm.
    /// </summary>
    StrongCryptographicProvider,
};

} }

#endif
