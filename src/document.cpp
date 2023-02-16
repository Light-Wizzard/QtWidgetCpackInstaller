// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "document.h"
/** ***************************************************************************
 *  \brief Document::setText
 *  \param thisText
 */
void Document::setText(const QString &thisText)
{
    if (thisText == myText) { return; }
    myText = thisText;
    emit textChanged(myText);
}
// ***************************** End of File **********************************
