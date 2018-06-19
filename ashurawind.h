#ifndef ASHURAWIND_H
#define ASHURAWIND_H

#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
//#include <milestone32_global.h> // Own lib


namespace Ui {
class AshuraWind;
}

class AshuraWind : public QMainWindow
{
    Q_OBJECT

public:
    explicit AshuraWind(QWidget *parent = 0);
    ~AshuraWind();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_actionUruchom_triggered();
    void on_qpickfolder_clicked();

private:
    Ui::AshuraWind *ui;
    QFuture<void> mf;

};


struct ContentThread
{
    QString folder;
    QString folderContent;
    QString folderFiles;
    QString folderTxt;

    QProgressBar *progressmime;
    QProgressBar *progresshtml;
};
void runInThread(ContentThread content);


//extern ListOfFiles(std::string in, std::list<std::string> out); // Get list of files from folder
//extern CorrectThePath(std::string in_out); // Removes extra backslashes
//extern Save(std::string fname, std::string content); // Write text to file

#endif // ASHURAWIND_H
