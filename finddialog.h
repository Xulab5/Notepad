/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */
#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QWidget>
//forward declaration
class QPushButton;
class QCheckBox;
class QLabel;
class QLineEdit;

class finddialog : public QWidget
{
    Q_OBJECT
public:
    explicit finddialog(QWidget *parent = nullptr);

signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findprev(const QString &str, Qt::CaseSensitivity cs);

public slots:
    void findClicked();
    void enableFindButton(const QString &text);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *backwardCheckBox;
    QCheckBox *caseCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
