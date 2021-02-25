/****
 *
 * AUTHOR: Steven J Sub
 * LECTURER: Mr. Farshad Rabbani
 * Date: March 11 2019
 * DESCRIPTION: This notepad allows user to write and read text. Allows user to see text properties
 * and as well creates chart base on the frquency of the words and also have other function
 *
 * */
#include "word.h"
#include <QString>
#include <iostream>
#include <QDebug>
using namespace std;

Word::Word(QString t)
{
    text=t;
    frequency=1;
    setLength(text.size());
}

Word::Word(QString t, int length)
{
    text=t;
    setLength(length);
    frequency=1;

}

void Word::setFrequency(int i)
{
    frequency =i;
}

int Word::getFrequency() const
{
    return frequency;
}

void Word::setLength(int i)
{
    length=i;
}

int Word::getLength() const
{
    return length;
}
void Word::setText(QString txt)
{
    text=txt;
}

QString Word::getText() const
{
    return text;
}

void Word::operator++()
{
    ++frequency;
}

bool Word::operator <(Word &w)
{
    if( this->getFrequency() == w.getFrequency())
    {
        if(this->getText() < w.getText())
            return true;
    }else if(this->getFrequency() < w.getFrequency())
        return true;


    return false;
}

bool Word::operator>(Word &w)
{
    if( this->getFrequency() > w.getFrequency())
            return true;
    else if(this->getFrequency() == w.getFrequency())
        if(this->getText()==w.getText())
            return true;

    return false;
}

bool Word::operator==(Word &w)
{
    if(this->getText()==w.getText())
        return true;

    return false;
}

QDebug operator<<(QDebug dbg, const Word &w)
{
    dbg.space()<<w.getText()<<w.getFrequency()<<w.getLength();
    return dbg;
}

ostream &operator<<(ostream &out, const Word w)
{
    out<<w.getText()<<" "<<w.getFrequency()<<endl;
    return out;
}
QTextStream &operator<<(QTextStream &out, const Word w){
    out<<w.getText()<<" "<<w.getFrequency()<<" "<<w.getLength()<<"\n";
    return out;
}
QTextStream &operator>>(QTextStream &in, Word &w)
{
    QString txt;
    int freq=0,len=0;
    in>>txt>>freq>>len;
    w.setText(txt);
    w.setLength(len);
    w.setFrequency(freq);
    return in;
}
