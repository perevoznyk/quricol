//===============================================================================
// Copyright (c) Serhiy Perevoznyk.  All rights reserved.
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE.
//===============================================================================

using System;
using System.Collections.Generic;
using System.Text;

namespace Quricol.Barcode
{
    /// <summary>
    /// The possible values for the error correction level
    /// </summary>
    public enum ErrorCorrectionLevel : int
    {
        LowQuality,
        MediumQuality,
        StandardQuality,
        HighQuality
    }
}
