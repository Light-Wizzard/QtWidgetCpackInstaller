// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include <QWebEnginePage>
/** ***************************************************************************
 *  \brief class PreviewPage
 */
class PreviewPage : public QWebEnginePage
{
        Q_OBJECT
    public:
        using QWebEnginePage::QWebEnginePage;

    protected:
        bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
}; // end class PreviewPage
#endif // PREVIEWPAGE_H
// ***************************** End of File **********************************
