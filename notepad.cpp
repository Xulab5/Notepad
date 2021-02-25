/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */
#include "notepad.h"
#include "ui_notepad.h"
#include <QFileInfo>
#include<QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include<QCloseEvent>
#include <QFontDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QFont>
#include "finddialog.h"
#include <QStringList>
#include <QList>
#include <QStack>
#include <QString>
#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QDateTime>
#include <QDate>
#include "docpropertiesdialog.h"
#include "word.h"
#include <fstream>


Notepad::Notepad(const QString & fileName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
    docPropertiesDialog =new docpropertiesdialog();

    this->setCentralWidget(ui->textEdit);
    setWindowTitle(tr("TextEditor[*]"));
    ui->textEdit->setFocus();//set the cursor on the textedit widget
    ui->actiontoolBar->setChecked(true);
    connect(ui->actionClose, SIGNAL(triggered(bool)),this, SLOT(close()));
    connect(ui->actionExit, SIGNAL(triggered(bool)),qApp, SLOT(closeAllWindows()));//qApp is the same as QApplicatino::instance
    connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)),qApp, SLOT(aboutQt()));
    connect(ui->actionCut, SIGNAL(triggered(bool)),ui->textEdit, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered(bool)),ui->textEdit, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered(bool)),ui->textEdit, SLOT(paste()));
    connect(ui->actionUndo, SIGNAL(triggered(bool)),ui->textEdit, SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered(bool)),ui->textEdit, SLOT(redo()));
    connect(ui->actionSave, SIGNAL(triggered(bool)),this, SLOT(saveFile()));
    connect(ui->actionSave_as, SIGNAL(triggered(bool)),this, SLOT(saveFileAs()));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(documentModified()));


    connect(this,SIGNAL(opendocProperties(QString,int,int)),docPropertiesDialog,SLOT(settext(QString,int,int)));

   connect(this,SIGNAL(sendprocessedText(QList<Word>,int,int,int,int)),docPropertiesDialog,SLOT(recieveProcessText(QList<Word>,int,int,int,int)));
    findDialog=0;

    loadFile(fileName);

}//end of constructor

Notepad::~Notepad()
{
    delete ui;

}//end of Destructor

void Notepad::on_actionNew_triggered()
{
    Notepad * editor =new Notepad();
    editor->show();

}//end of function

void Notepad::loadFile(const QString &fileName)
{
    //check if fileName is empty the set to setFileName() parameter to null or empty string
    if(fileName.isEmpty())
    {
        setFileName(QString());
        return;
    }//end of if

    QFile newFile(fileName);//building a qFile object, build a file includes the path of the fiele

    if(!newFile.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Load file error"), QString(tr("Unable to load file, %1")).arg(fileName));

        setFileName(QString());
        return;
    }
    else{
        QTextStream inputFileStream(&newFile);//allow us to read from a file
        ui->textEdit->setText(inputFileStream.readAll());
        newFile.close();
        setFileName(fileName);
        setWindowModified(false);
        QFileInfo fileInfo(newFile);
        QString fileNameWithoutPath (fileInfo.fileName());//returns only the file name not the path
        ui->statusBar->showMessage(QString(tr("File %1 successfully opened")).arg(fileNameWithoutPath));
    }



}//end of function

void Notepad::setFileName(const QString &fileName)
{
    m_fileName = fileName;
 setWindowTitle(QString(tr("%1[*] - %2")).arg(m_fileName.isNull()?tr("untitled"): QFileInfo(m_fileName).fileName()).arg(QApplication::applicationName()));

}//end of function

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("open document"), QDir::currentPath(),tr("Text documents (*.txt);; pdf document (*)"));

    if(fileName.isNull())
        return;
    if(this->m_fileName.isNull() && !isWindowModified())
        loadFile(fileName);
    else{
        Notepad * editor = new Notepad();
        editor->show();
    }
}

void Notepad::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("Text Editor"), tr("This is better than Notepad++"));
}

bool Notepad::saveFile()
{

    if(m_fileName.isNull())
        return saveFileAs();

    else{
       QFile newFile(m_fileName);

              if(!newFile.open(QIODevice::WriteOnly | QIODevice::Text))//file is open for writeonly
            {
                QMessageBox::warning(this, tr("Load file error"), QString(tr("Unable to load file, %1")).arg(m_fileName));
                setFileName(QString());
                return false;
            }
            else
            {
                QTextStream outFileStream(&newFile);
                outFileStream<<ui->textEdit->toPlainText();
                newFile.close();
                setWindowModified(false);
                QFileInfo fileInfo(newFile);
                QString fileNameWithoutPath(fileInfo.fileName());
                ui->statusBar->showMessage(QString(tr("File %1 successfully saved")).arg(fileNameWithoutPath));
                return true;
            }



    }
}

bool Notepad::saveFileAs()
{
    QString fileName =QFileDialog::getSaveFileName(this,tr("Save document"), m_fileName.isNull()?QDir::currentPath():m_fileName, tr("Text document(*.txt)"));
    if(fileName.isNull())
        return false;
    else
    {
        setFileName(fileName);
        return saveFile();
    }
}//end of function saveFileAs

void Notepad::documentModified()
{
    this->setWindowModified(true);//say something has change within this window
}

void Notepad::closeEvent(QCloseEvent *e)
{
    if(isWindowModified())
    {
        QMessageBox::StandardButton response =QMessageBox::warning(this,tr("Document has been modified"),tr("The document has been modified. Do you want to Save it?"),
                                                                   QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel, QMessageBox::Cancel);
        switch(response)
        {
         case QMessageBox::Yes:
            if(saveFile())
                e->accept();
            else
                e->ignore();
            break;
        case QMessageBox::No:
            e->accept();
            break;
        case QMessageBox::Cancel:
            e->ignore();
        }
    }
    else
            {
                e->accept();
            }

}//end of function

void Notepad::on_actionSelect_Font_triggered()
{
    QFontDialog fontDialog;
    bool ok = false;
    QFont font = fontDialog.getFont(&ok, ui->textEdit->font(),this,tr("choose a Font"));

    if(ok)
        ui->textEdit->setFont(font);
}

void Notepad::on_actionPrint_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer,this);
    dlg->setWindowTitle(tr("Print Document"));

    if(dlg->exec() == QDialog::Accepted)
        ui->textEdit->print(&printer);
}

void Notepad::on_actiontoolBar_triggered()
{
        if(ui->actiontoolBar->isChecked())
            ui->mainToolBar->show();
        else
            ui->mainToolBar->hide();
}

void Notepad::on_actiontoUpperCase_triggered()
{

   QString text = ui->textEdit->toPlainText();
   for(int i=0; i < text.length(); i++){
       text[i]=text[i].toUpper();
   }
ui->textEdit->setText(text);

}

void Notepad::on_actiontoLowerCase_triggered()
{
    QString text = ui->textEdit->toPlainText();  
 ui->textEdit->setText(text.toLower());

}

int Notepad::wordCount()
{
    QString text = ui->textEdit->toPlainText();
    text=text.simplified();
    bool state=true;
    int totalword=0;
    if(!text.isEmpty())
    {
    QStringList list= text.split(QRegExp("\\W+"),QString::SkipEmptyParts);
    qDebug()<<list;
    totalword=list.size();//gets the totol numbers of words on the textedit
    }
    return totalword;//returns the total number of words
}

void Notepad::on_actionword_count_triggered()
{
    QString con= QString::number( wordCount());
    QMessageBox::information(this,tr("Number of words"), con  );//display number of words
}//end of function

void Notepad::on_actionFind_triggered()
{
    if(!findDialog)
    {
        findDialog =new finddialog();//initialize the pointer of type finddialog
    }
    findDialog->show();
    //allows you to bring back a dialog that is behind the current parent
    findDialog->raise();
    findDialog->activateWindow();

}//end of function

void Notepad::on_actionReverse_Text_triggered()
{

    QString text = ui->textEdit->toPlainText();//gets text from the textEdit
    QStringList words=text.split(QRegExp("\\b"));//
    QString reverse;

    while (!words.isEmpty())
    reverse.append(words.takeLast());

    ui->textEdit->setText(reverse);

}//end of function

void Notepad::on_actioncapitalize_triggered()
{
    QString text = ui->textEdit->toPlainText();//gets text from the textEdit

    if(!text.isEmpty())
    {
        text[0]=text[0].toUpper();
        for( int i =1; i < text.length(); i++)
            if(text[i]=='.' || text[i]=='!')
            {
               int x=i+1;
			   //continues moving untill it reached to a letter
                while ((x <text.length()) && (text[x] =="\t" || text[x]==' ' || text[x]=='\n') )x++;
                text[x]=text[x].toUpper();
                i=x;
            }
    }
ui->textEdit->setText(text);//set the capitalized

}//end of function

int Notepad::determineUniqueWords()
{
    QString text = (ui->textEdit->toPlainText()).toLower();//gets text from the textEdit
    int totalUniqueWords=0;

        if(!text.isEmpty())
        {
            QStringList listOfWords = text.split(QRegExp("\\W+"),QString::SkipEmptyParts);
            QStringList noDuplicate= text.split(QRegExp("\\W+"),QString::SkipEmptyParts);
            noDuplicate.removeDuplicates();

               for(int i =0; i < noDuplicate.size(); i++)//counts unique worlds
                if(listOfWords.count(noDuplicate[i])==1)//counts how man times a word appears in the listofwords
                    totalUniqueWords++;
        }
        
		return totalUniqueWords;

}//end of function

void Notepad::on_actionUnique_Word_count_triggered()
{
    QMessageBox::information(this,tr("Unique words"), QString::number(determineUniqueWords()));
}

bool Notepad::on_actionSave_as_PDF_triggered()
{

QString fileName=QFileDialog::getSaveFileName(this,tr("Save document as pdf"),QDir::currentPath(),tr("PDF document(*.pdf)"));
//fileName.isEmpty()?setFileName("output.pdf"):setFileName(fileName);
// return saveFile();


 /* dont change the windowtitle*/
fileName.isEmpty()?fileName="C:/Users/Steven Sub/Documents/UB course/Semester 4/GUI Programming/Steven_TextEditor/output.pdf":fileName;
QPdfWriter pdf(fileName);
pdf.setPageSize(QPagedPaintDevice::A4);
pdf.setPageMargins(QMargins(50,50,50,50));//set the margin on all side of the page

QPainter painter(&pdf);
QRect r = painter.viewport();
QString text = ui->textEdit->toPlainText();
painter.drawText(r,text);//writes on the page
painter.end();

}

void Notepad::on_actionDocument_Properties_triggered()
{    
    if(!docPropertiesDialog)
       docPropertiesDialog= new docpropertiesdialog();

    //send qlist along with total words and unique words as soon as the dialog is launched
    emit sendprocessedText(processString(),wordCount(),determineUniqueWords(),findNumOfCharwithspace(),determineLines());
    docPropertiesDialog->show();
    docPropertiesDialog->raise();
    docPropertiesDialog->activateWindow();

}//end of function

void Notepad::on_actionGenerate_Statistical_File_triggered()
{
    if(m_fileName.isEmpty())
    {
        QMessageBox::warning(this,"Statistical File error","There is no file currently open.");
    }
    else
    {
         QFileInfo fileInfo(m_fileName);
         QString statisticalFile = m_fileName;//set the name path of the Sta file
         statisticalFile = statisticalFile.remove("."+fileInfo.suffix());//the extension
         statisticalFile.insert(statisticalFile.length(),"_statistical.sta");//adds new name and extension
        QFile newFile(statisticalFile);

        if(!newFile.open(QIODevice::WriteOnly | QIODevice::Text))//file is open for writeonly
      {
          QMessageBox::warning(this, tr("Load file error"), QString(tr("Unable to load file, %1")).arg(m_fileName));
          //setFileName(QString());

      }else{
              writeStatisticFile(newFile,fileInfo);//write to the file if successfully opened
              QMessageBox::
              information(this,"Statistical File","Generation of Statistical file successfully completed");

           }
        newFile.flush();
        newFile.close();

    }//end of outter else

}

void Notepad::writeStatisticFile(QFile &statisticalFile, QFileInfo fInfo)
{

     QDate date=QDate::currentDate();

     QTextStream outFileStream(&statisticalFile);
     outFileStream<<"Statistical Data for: "+fInfo.fileName();
     outFileStream<<"\nGenerated Date: "+date.toString();
     outFileStream<<"\nTotal Word Count: "+QString::number(wordCount());
     outFileStream<<"\nUnique Word Count: "+QString::number(determineUniqueWords())<<"\nWord Frequency:\n";

   QList<Word> list = processString();//gets the words into the Qlist

   //writes the words to the Stattistic file
   for(int i=list.count()-1; i >=0; i--)
       outFileStream<<list[i];

     setWindowModified(false);

}//end of function

QList<Word> Notepad::processString()
{
    QString text=(ui->textEdit->toPlainText()).toLower();//gets the text from the textedit
    QStringList listOfWord;
    QList<Word> words;

     if(!text.isEmpty())//check if the test is not empty
     {
          listOfWord= text.split(QRegExp("\\W+"),QString::SkipEmptyParts);//chops every word

          while( !listOfWord.isEmpty())//checks if the QStringlist is not empty
           {
            bool state=false;
                  QString popWord = listOfWord.takeFirst();//pop out the first word from the QStringlist
                  Word w(popWord);//insert the popped word in the word object data member
                  w.setLength(popWord.length());// set the length of the word in the word object

                  if(words.isEmpty())//insert the first world if Qlist is empty
                      words<<w;
                    else//check if word already exist in the QList
                  {
                    for(int i=0; i < words.count(); i++)
                       if(words[i].getText()== w.getText())//if word is found in the Qlist frequency updated
                        {
                            state=true;
                            ++words[i];//increase the frequency by 1
                        }

                      if(!state)
                          words<<w;//insert the word if it is not found in the list

                  }//end of else

           }//end of while
          std::sort(words.begin(), words.end());
    return words;//return sorted list of words
     }//end of if

}//end of function

int Notepad::determineLines()
{
    QString text= ui->textEdit->toPlainText();
    int totalLines=0;
    if(!text.isEmpty())
    {
        totalLines++;//increment
        for(int i=0; i< text.size(); i++)
            if(text[i]=='\n')
                totalLines++;
    }
    return totalLines;//return number of lines

}//end of function
int Notepad::findNumOfCharwithspace()
{
    return ((ui->textEdit->toPlainText()).simplified()).size();//returns to character with space
}
