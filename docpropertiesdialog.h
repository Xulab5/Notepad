/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */
#ifndef DOCPROPERTIESDIALOG_H
#define DOCPROPERTIESDIALOG_H

#include <QWidget>
#include "word.h"
class chart;

namespace Ui {
class docpropertiesdialog;
}

class docpropertiesdialog : public QWidget
{
    Q_OBJECT

public:
    explicit docpropertiesdialog(QWidget *parent = 0);
    ~docpropertiesdialog();
signals:
    void initialize();
    void getwAU();
    void chartInfo( int, QList <Word>, int);//triggers the piechart to be display
    void autoGenerateCall();//calls the function processString from the notepad

private slots:
    void settext(QString,int,int);//initialize the data members
    void setResults();

    void on_okButton_clicked();
    void recieveProcessText(QList<Word>,int,int,int,int);

private:
    Ui::docpropertiesdialog *ui;


    QString text;
    chart * ChartDisplay;
    int numberOfUniqueWords;
    int numOfCharWithSpace;
    int totalWords;
    int numberOfCharWithoutSpace=0;
    QList<Word> wordlist;
    void readstatisticFile(QString fName);
    void determineNumOFCharacterWithoutSpace();
    int totalLines;
    void creatChart(int top,int Type);
};

#endif // DOCPROPERTIESDIALOG_H
