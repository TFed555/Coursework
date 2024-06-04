#ifndef WORKERWINDOW_H
#define WORKERWINDOW_H

#include <QDialog>
#include "workswindow.h"

namespace Ui {
class WorkerWindow;
}

class WorkerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WorkerWindow(QString currentLogin, QWidget *parent = nullptr);
    ~WorkerWindow();

private slots:
    void on_nextButton_clicked();
    void on_backButton_clicked();

signals:
    void AuthoWindow();
private:
    Ui::WorkerWindow *ui;
    std::shared_ptr <WorksWindow> worksUi;
private:
    const QString login;
};

#endif // WORKERWINDOW_H
