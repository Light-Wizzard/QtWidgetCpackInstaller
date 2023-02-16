// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>
/** ***************************************************************************
 *  \brief class Document
 */
class Document : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QString text MEMBER myText NOTIFY textChanged FINAL)
    public:
        explicit Document(QObject *parent = nullptr) : QObject(parent) {}

        void setText(const QString &thisText);

    signals:
        void textChanged(const QString &thisText);

    private:
        QString myText;
}; // end class Document
#endif // DOCUMENT_H
// ***************************** End of File **********************************
