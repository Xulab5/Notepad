#include <QtGui>
#include "finddialog.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

finddialog::finddialog(QWidget *parent) : QWidget(parent)
{
    label = new QLabel(tr("Find &what"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    backwardCheckBox = new QCheckBox(tr("Search Backwards"));
    caseCheckBox = new QCheckBox(tr("Match &case"));

    caseCheckBox = new QCheckBox(tr("Match &case"));
    backwardCheckBox = new QCheckBox(tr("Search &Backwards"));

    findButton = new QPushButton(tr("&Find"));
    findButton->setDefault(true);//enable to be trigger when user hits enter key on the keyboard
    findButton->setEnabled(false);//set the button to not be able to be click useless something triggers it
   //adds icon to the table
    QIcon icon("C:/Users/Steven Sub/Downloads/images/find.png");
    findButton->setIcon(icon);
    closeButton = new QPushButton(tr("close"));

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));

    connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout * topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);//adds the layout to the mainWindow
    setWindowTitle("Find");
    setFixedHeight(sizeHint().height());

}
void finddialog::findClicked()
{
    QString text = lineEdit->text();//get the text inputted from the inputBox
    //check if the casecheckbox is checked if it is then the text is caseSensitive
    Qt::CaseSensitivity cs = caseCheckBox->isChecked()? Qt::CaseSensitive: Qt::CaseInsensitive;

    if(backwardCheckBox->isChecked())
        emit findprev(text, cs);
    else
        emit findNext(text, cs);
}

void finddialog:: enableFindButton(const QString &text)
{
    findButton->setEnabled( !text.isEmpty() );//enable findbutton to be click if there's a text in the lineEdit
}

