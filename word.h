#ifndef WORD_H
#define WORD_H
#include <QString>
#include <iostream>
#include <QTextStream>
#include <QObject>
using namespace std;

class Word
{

public:
    Word(QString t);//the constructor will build a Word object based on t and
    //initialize count to 1
    Word (QString t, int length);
    void setFrequency(int i);
    void setText(QString);
    int getFrequency() const;
    void setLength(int i);
    int getLength() const;
    QString getText() const;
    void operator++();
    bool operator <(Word &w);
    bool operator>(Word &w);
    bool operator==(Word & w);
    friend ostream& operator<<(ostream &out, const Word w);
    friend QDebug operator<<(QDebug dbg, const Word &w);
    friend QTextStream &operator<<(QTextStream &out, const Word w);
    friend QTextStream &operator>>(QTextStream &in, Word &w);
private:
    QString text;
    int frequency;
    int length;
};

#endif // WORD_H
