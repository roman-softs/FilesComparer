#include "choose_directory_widget.h"

#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QFileDialog>

ChooseDirectoryWidget::ChooseDirectoryWidget(QWidget *parent)
    : QWidget(parent)
    , m_dirEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_dirEdit);
    auto chooseDirButton = new QToolButton(this);
    mainLayout->addWidget(chooseDirButton);

    connect(chooseDirButton, &QToolButton::clicked, this, [this] {
        QString path = QFileDialog::getExistingDirectory(this);
        m_dirEdit->setText(path);
        emit directoryChanged(path);
    });

    connect(m_dirEdit, &QLineEdit::editingFinished, this, [this] {
        emit directoryChanged(m_dirEdit->text());
    });
}

QString ChooseDirectoryWidget::currentDirectory() const
{
    return m_dirEdit->text();
}
