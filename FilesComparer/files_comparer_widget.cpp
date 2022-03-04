#include "files_comparer_widget.h"
#include "ui_files_comparer_widget.h"

#include "files_comparer.h"

#include <QFutureWatcher>
#include "types.h"

FilesComparerWidget::FilesComparerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FilesComparerWidget)
    , m_filesComparer(new FilesComparer(this))
{
    ui->setupUi(this);
    disableStartButton(false);

    connect(ui->widgetChooseDirectory1, &ChooseDirectoryWidget::directoryChanged,
            m_filesComparer, &FilesComparer::setFirstDirectory);

    connect(ui->widgetChooseDirectory2, &ChooseDirectoryWidget::directoryChanged,
            m_filesComparer, &FilesComparer::setSecondDirectory);

    connect(ui->pushButtonStart, &QPushButton::clicked,
            this, [this] {
        if(!m_filesComparer->canStart()) {
            ui->textEdit->append("\nwrong directories!\n");
            return;
        }
        if (!m_result) {
            m_result = new QFutureWatcher<types::IdenticalFilesList>(this);
            connect(m_result, &QFutureWatcher<types::IdenticalFilesList>::finished, this, [this] {
                types::IdenticalFilesList identicalFilesList = m_result->result();
                for(const auto& strList : identicalFilesList) {
                    ui->textEdit->append("\nidentical files:");
                    ui->textEdit->append(strList.join('\n'));
                }
                disableStartButton(false);
                m_result->deleteLater();
                m_result = nullptr;
            });

            QObject::connect(ui->pushButtonCancel, &QPushButton::clicked, m_result, &QFutureWatcher<types::IdenticalFilesList>::cancel);

            connect(m_result, &QFutureWatcher<types::IdenticalFilesList>::canceled, this, [this] {
                disableStartButton(false);
                m_result->deleteLater();
                m_result = nullptr;
                ui->textEdit->append("canceled\n");
            });

            m_result->setFuture(m_filesComparer->startCompare());

            disableStartButton(true);
            ui->textEdit->append("in progress..\n");
        }

    });

    connect(ui->pushButtonClear, &QPushButton::clicked,
            ui->textEdit, &QTextEdit::clear);
}

FilesComparerWidget::~FilesComparerWidget()
{
    delete ui;
}

void FilesComparerWidget::disableStartButton(bool disable)
{
    ui->pushButtonStart->setDisabled(disable);
    ui->pushButtonCancel->setDisabled(!disable);
}
