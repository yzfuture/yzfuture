#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    static void    onCardReadProgress(void* userData, unsigned int nProgress);
    int     readCardInfo(int nindex);
    void    setLogTxt(char* fmt, ...);

public slots:
void	on_btn_clicked();

private:
    Ui::MainWindow *ui;
    static char			buffer[1024];
};

#endif // MAINWINDOW_H
