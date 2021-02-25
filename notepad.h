/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */
#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QFileInfo>
#include <QTextStream>
#include "word.h"
class finddialog;
class docpropertiesdialog;


namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(const QString & fileName = QString(), QWidget *parent = 0);
    ~Notepad();
signals:
    //new line of code
    void opendocProperties(QString text,int word,int unique);
    void sendprocessedText(QList<Word>,int, int,int,int);

    void sendwAU(int);

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionAbout_triggered();
    void documentModified();

    bool saveFile();
    bool saveFileAs();

    void on_actionSelect_Font_triggered();

    void on_actionPrint_triggered();

    void on_actiontoolBar_triggered();

    void on_actiontoUpperCase_triggered();

    void on_actiontoLowerCase_triggered();

    void on_actionword_count_triggered();

    void on_actionFind_triggered();

    void on_actionReverse_Text_triggered();

    void on_actioncapitalize_triggered();

    void on_actionUnique_Word_count_triggered();

    bool on_actionSave_as_PDF_triggered();

    void on_actionDocument_Properties_triggered();

    void on_actionGenerate_Statistical_File_triggered();


protected:
    void closeEvent(QCloseEvent *e);


private:
    Ui::Notepad *ui;
    QString m_fileName;
    finddialog *findDialog;
    void loadFile(const QString & fileName);
    void setFileName(const QString & fileName);
    int wordCount();
    int determineUniqueWords();

    docpropertiesdialog *docPropertiesDialog;
    void writeStatisticFile(QFile &,QFileInfo);//write to the sta file
    QList<Word> processString();//creates the Qlist<Word>
    int findNumOfCharwithspace();
    int determineLines();

};//end of class

#endif // NOTEPAD_H
