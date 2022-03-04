#pragma once

#include "types.h"

#include <QDir>
#include <QObject>
#include <QFuture>

class FilesComparer : public QObject
{
    Q_OBJECT
public:
    explicit FilesComparer(QObject *parent = nullptr);
    bool setFirstDirectory(const QString& path);
    bool setSecondDirectory(const QString& path);
    bool canStart() const;

    QFuture<types::IdenticalFilesList> startCompare();
    types::IdenticalFilesList runCompare();

private:
    QDir m_firstDir;
    QDir m_secondDir;

};


