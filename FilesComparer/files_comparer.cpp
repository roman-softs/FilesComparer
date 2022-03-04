#include "files_comparer.h"

#include <QtConcurrent/QtConcurrent>
#include <QStringList>

FilesComparer::FilesComparer(QObject *parent) : QObject(parent)
{
    m_firstDir.setPath(QString());
    m_secondDir.setPath(QString());
}

bool FilesComparer::setFirstDirectory(const QString &path)
{
    m_firstDir.setPath(path);
    return m_firstDir.exists(path);
}

bool FilesComparer::setSecondDirectory(const QString &path)
{
    m_secondDir.setPath(path);
    return m_secondDir.exists(path);
}

bool FilesComparer::canStart() const
{
    return (m_firstDir.exists() && m_secondDir.exists()) && (m_firstDir.dirName() != m_secondDir.dirName());
}

QFuture<types::IdenticalFilesList> FilesComparer::startCompare()
{
    return QtConcurrent::run(this, &FilesComparer::runCompare);
}

types::IdenticalFilesList FilesComparer::runCompare()
{
    if(!m_firstDir.exists() || !m_secondDir.exists())
        return {};

    QStringList filesList = m_firstDir.entryList(QDir::Files).replaceInStrings(QRegExp("^"), m_firstDir.absolutePath() + '/')
            + m_secondDir.entryList(QDir::Files).replaceInStrings(QRegExp("^"), m_secondDir.absolutePath() + '/');

    types::IdenticalFilesList identicalFilesList;
    QStringList findedComparedFiles;
    for(auto compareIt = filesList.begin(); compareIt != filesList.end()-1; ++compareIt) {
        QFile compareFile(*compareIt);
        if(!compareFile.open(QIODevice::ReadOnly)) {
            qDebug() << "open file error";
        }
        if(!compareFile.exists())
            continue;

        QByteArray compareBin = compareFile.readAll();
        QStringList identicalFiles;

        for(auto firstCompareIt = compareIt+1; firstCompareIt != filesList.end(); ++firstCompareIt) {
            if(findedComparedFiles.contains(*firstCompareIt))
                continue;

            QFile file(*firstCompareIt);
            if(!file.open(QIODevice::ReadOnly)) {
                qDebug() << "open file error";
            }
            if(!file.exists())
                continue;
            QByteArray fileBin = file.readAll();
            if(compareBin == fileBin){
                identicalFiles.append(*firstCompareIt);
                findedComparedFiles.append(*firstCompareIt);
            }
        }
        if(!identicalFiles.isEmpty()) {
            identicalFiles.append(*compareIt);
            identicalFilesList.append(identicalFiles);
        }
    }

    return identicalFilesList;
}
