/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */
#ifndef CHART_H
#define CHART_H
#include "word.h"
#include <QMainWindow>
#include <QLabel>
#include <QList>

namespace Ui {
class chart;
}

class chart : public QMainWindow
{
    Q_OBJECT

public:
    explicit chart(QWidget *parent = 0);
    ~chart();
signals:
    void displaychart();
private slots:
    void piechart();
    void lineChart();
    void barChart();
    void chartinformation(int,QList<Word>,int);//GET information
private:
    Ui::chart *ui;

    int amountToShow;
    QList<Word> wordlist;
};

#endif // CHART_H
