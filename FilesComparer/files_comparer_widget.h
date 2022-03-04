#pragma once

#include <QWidget>
#include "types.h"

namespace Ui {
class FilesComparerWidget;
}

class FilesComparer;
template <typename T> class QFutureWatcher;

class FilesComparerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilesComparerWidget(QWidget *parent = nullptr);
    ~FilesComparerWidget();

private:
    void disableStartButton(bool disable);

private:
    Ui::FilesComparerWidget *ui;
    FilesComparer* m_filesComparer;
    QFutureWatcher<types::IdenticalFilesList>* m_result = nullptr;
};
