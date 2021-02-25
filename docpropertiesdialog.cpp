/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */

#include "docpropertiesdialog.h"
#include "ui_docpropertiesdialog.h"
#include "word.h"
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include "notepad.h"

#include <QFileDialog>
#include "chart.h"


docpropertiesdialog::docpropertiesdialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::docpropertiesdialog)
{
    ui->setupUi(this);

    ChartDisplay= new chart();//creates pointer of type chart();

    connect(this,SIGNAL(initialize()),this,SLOT(setResults()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(close()));

    connect(ui->wordComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setResults()));//connects the wordcombox
    connect(ui->characterComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setResults()));
    connect(this,SIGNAL(chartInfo(int,QList<Word>, int)),ChartDisplay,SLOT(chartinformation(int,QList<Word>,int)) );

    setFixedSize(width(), height());

}

docpropertiesdialog::~docpropertiesdialog()
{
    delete ui;
}

void docpropertiesdialog::settext(QString t,int numOfwords, int uniqueWords)
{
    text=t;
    totalWords=numOfwords;
    numberOfUniqueWords=uniqueWords;
    emit initialize();


}//end of function

void docpropertiesdialog::setResults()
{
    determineNumOFCharacterWithoutSpace();

  int wordindex=ui->wordComboBox->currentIndex();//gets the index from the combobox for word
  int characterIndex = ui->characterComboBox->currentIndex();//gets the index from the combobox for char

  if(wordindex==0)
      ui->wordResultLabel->setText(QString::number(totalWords));//display total number of words
  else
      ui->wordResultLabel->setText(QString::number(numberOfUniqueWords));//display unique words

  if(characterIndex ==0)
      ui->charResultLabel->setText(QString::number(numberOfCharWithoutSpace));
  else
      ui->charResultLabel->setText(QString::number(numOfCharWithSpace));//display total character with space

  ui->linecountResultLabel->setText(QString::number(totalLines));//set the total number of lines

}//end of function

void docpropertiesdialog::determineNumOFCharacterWithoutSpace()
{
    int charnum=0;
    if(!wordlist.isEmpty())//loops if there is words in the list
    {
   charnum= wordlist[0].getLength()*wordlist[0].getFrequency();//gets the first words lenght times its frequency
        for(int i=1; i < wordlist.size(); i++)
        {
            charnum+=wordlist[i].getLength()*wordlist[i].getFrequency();
        }
    }
    numberOfCharWithoutSpace=charnum;

}//end of function

void docpropertiesdialog::on_okButton_clicked()
{
    int chartType= ui->chartTypComboBox->currentIndex();
    int source = ui->dataSourceComboBox->currentIndex();
    int quantityID =ui->quantityComboBox->currentIndex();

    QString fileName;

    if(source==0)//if data need to be obtian from file
    {
        fileName= QFileDialog::getOpenFileName(this,tr("open Statistic File"), QDir::currentPath(),tr("Statistical file(*.sta)"));

       if( !fileName.isEmpty() ) {readstatisticFile(fileName);}else return;
    }
    else//auto generate
    {
        if(wordlist.isEmpty())
        {
            QMessageBox::warning(this,"Chart Error","There is no text in the notepad");
        return;
        }

    }//end of else

    creatChart(quantityID,chartType);//creates object and display the correct chart

}//end of function

void docpropertiesdialog::readstatisticFile(QString fName)
{
    QFile newFile(fName);
    if(!newFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"File Error","Unable to open file for reading");
    return;
    }
    Word w(" ");//create empty word object
    QTextStream in(&newFile);//creat stream to the file
    int i=0;
    while(i < 5){
        in.readLine();//read line of text from the file
        i++;
    }

    QList <Word> list;
    in>>w;
    while(!in.atEnd())//continue reading untill reach the end of the file
    {
        list<<w;//insert a word object in the  list
        in>>w;//get information from the file and initialize the word object
    }
    std::sort(list.begin(), list.end());
    wordlist=list;//set the data member with the list

    newFile.close();//closes the opened file

}//end of function readstatisticfile

void docpropertiesdialog::creatChart(int amountToShow,int Type)
{
    if(!ChartDisplay)
        ChartDisplay=new chart();

        emit chartInfo(amountToShow,wordlist,Type);
    ChartDisplay->show();//display chart window

}//end of function

void docpropertiesdialog::recieveProcessText(QList<Word> list,int w, int u,int withSpaceChar,int line)
{
    //initialize data members
    wordlist=list;
    totalWords=w;
    totalLines=line;
    numberOfUniqueWords=u;
    numOfCharWithSpace=withSpaceChar;

emit initialize();//initialize all labels

}//end of function

