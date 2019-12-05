#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <findmdnfclass.h>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void buildTable();
    Ui::MainWindow *ui;

signals:
    void mySignal();

private slots:
    void on_btnApply_clicked();

    void on_btnApply2_clicked();
    void changeCellValue(int row, int col);


private:
QThread * thread;
};

#endif // MAINWINDOW_H
