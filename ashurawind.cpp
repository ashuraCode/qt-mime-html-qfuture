#include "ashurawind.h"
#include "ui_ashurawind.h"
#include "email.h"
#include "html.h"

volatile int MIME_PROGRESS;
volatile int HTML_PROGRESS;


AshuraWind::AshuraWind(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AshuraWind)
{
    ui->setupUi(this);

    startTimer(50);
}

AshuraWind::~AshuraWind()
{
    delete ui;
}

void AshuraWind::timerEvent(QTimerEvent */*event*/)
{
    ui->qprogressmime->setValue(MIME_PROGRESS);
    ui->qprogresshtml->setValue(HTML_PROGRESS);
}

void AshuraWind::on_actionUruchom_triggered()
{
    ContentThread content;
    content.folder=ui->qfolderdir->text();

    if (ui->qcheckcontent->isChecked()) { content.folderContent=ui->qfoldercontent->text(); }
    if (ui->qcheckfiles->isChecked()) { content.folderFiles=ui->qfolderattachments->text(); }
    if (ui->qchecktxt->isChecked()) { content.folderTxt=ui->qfoldertext->text(); }

    content.progressmime = ui->qprogressmime;
    content.progresshtml = ui->qprogresshtml;
    mf = QtConcurrent::run(runInThread, content);
}

void AshuraWind::on_qpickfolder_clicked()
{
    QString folder = QFileDialog::getExistingDirectory ( this, QStringLiteral(""), QDir::currentPath(),
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog );
    ui->qfolderdir->setText(folder);
}

void runInThread(ContentThread content) {
    std::list<std::string> listFiles;
    MIME_PROGRESS = 0;
    HTML_PROGRESS = 0;

    if (!content.folderContent.isEmpty()) {
        ListOfFiles(content.folder.toStdString(), listFiles);

        QMetaObject::invokeMethod(content.progresshtml, "setMaximum", Qt::QueuedConnection, Q_ARG(int, listFiles.size()));
        QMetaObject::invokeMethod(content.progressmime, "setMaximum", Qt::QueuedConnection, Q_ARG(int, listFiles.size()));

        if (!listFiles.empty()) {
            QFileInfo file(listFiles.front().c_str());
            QDir dircontent(file.path() + "/" + content.folderContent);
            if (!dircontent.exists())
                dircontent.mkdir(dircontent.path());

            if (!content.folderFiles.isEmpty()) {
                QDir dirattachment(file.path() + "/" + content.folderFiles);
                if (!dirattachment.exists())
                    dirattachment.mkdir(dirattachment.path());
            }

            for (auto &s : listFiles) {
                MIME_PROGRESS++;
                QFileInfo file(s.c_str());
                std::map<std::string, std::string> email;
                ReadEmail(s, email);
                std::string filename = (dircontent.path() + "/" + file.fileName() + ".html").toStdString();
                CorrectThePath(filename);
                Save(filename, email["Text"]);

                if (!content.folderFiles.isEmpty()) {

                }
            }

            if (!content.folderTxt.isEmpty()) {
                listFiles.clear();
                ListOfFiles(dircontent.path().toStdString(), listFiles);
                QDir dirtxt(file.path() + "/" + content.folderTxt);
                if (!dirtxt.exists())
                    dirtxt.mkdir(dirtxt.path());

                for (auto &s : listFiles) {
                    HTML_PROGRESS++;
                    QFileInfo file(s.c_str());
                    if (file.exists()) {
                        std::string txt;
                        if (ReadHtmlToText(s, txt)) {
                            std::string filename = (dirtxt.path() + "/" + file.fileName() + ".txt").toStdString();
                            CorrectThePath(filename);
                            Save(filename, txt);
                        }
                    }
                }
            }
        }
    }
}
