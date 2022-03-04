#pragma once

#include <QWidget>

class QLineEdit;

class ChooseDirectoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseDirectoryWidget(QWidget *parent = nullptr);
    QString currentDirectory() const;

signals:
    void directoryChanged(const QString& path);

private:
    QLineEdit* m_dirEdit;
};

