#include "builddata.h"
#include <QProcess>
#include <QRegExp>
#include <QFile>
#include <QDebug>

BuildData::BuildData()
{
}

void BuildData::splitVersion(void)
{
    QString::iterator it = _version.begin();
    while (it != _version.end())
    {
        int digitmode = it->isDigit();
        QString part="";

        if (digitmode)
            while (it != _version.end() && (*it).isDigit())
                part += *it++;
        else
            while (it != _version.end() && !(*it).isDigit())
                part += *it++;
        _partslist <<part;
        digitmode = !digitmode;
    }
}

void BuildData::setVersion(const char * v)
{
    _version =v;
    splitVersion();
}

bool BuildData::isEmpty(void)
{
    bool result = _version.isEmpty();
    return (result);
}


void BuildData::read(const char *filename)
{
    QFile f(filename);
    QString contents;
    QStringList lines;

    if (f.exists())
    {
        f.open(QIODevice::ReadOnly);
        contents = f.readAll();
        //Check file is valid and not a captive portal
        if (contents.left(10)!="Build-date")
            contents="";
        f.close();
    }
    else
        contents="";

    lines     = contents.split("\n");
    foreach (const QString& str, lines)
    {
        if (str.contains("Version"))
        {
            setVersion(str.split(":").at(1).trimmed().toUtf8());
            qDebug() << "Version: " <<str << "," <<_version;
        }
    }
}



bool operator<(const BuildData& l, const BuildData& r)
{
    bool result=false;
    QStringList lparts = l._partslist;
    QStringList rparts = r._partslist;
    //The version breakdown must not be empty
    if (l._partslist.isEmpty() || r._partslist.isEmpty())
        return result;

    //Remove any non-digit prefix like 'V'
    //So both versions start at the same point
    //QString first = l._partslist.first();

    if (!lparts.first().at(0).isDigit())
        lparts.removeFirst();
    if (!rparts.first().at(0).isDigit())
        rparts.removeFirst();

    int mode=1; //digit

    int llen=lparts.count();
    int rlen=rparts.count();

    //qDebug() <<"xlen:" << llen << "," <<rlen;
    //qDebug() << "Comparing " << l._version << "," <<r._version;
    //qDebug() << "Comparing " << lparts << "," <<rparts;

    for (int i=0; (i< rlen); i++)
    {
        if (i>=llen)
        {
            //R has more parts
            //qDebug() << "R has more parts";
            result=true;
            break;
        }

        if (mode)
        {   //comparing digits

            int lint=0,rint=0;
            rint=rparts.at(i).toInt();
            lint=lparts.at(i).toInt();

            //qDebug() << "Mode=" << mode << " lint=" << lint << " rint=" << rint;

            if (lint < rint)
            {
                //L < R
                //qDebug() << "L < R:" << lint <<","<<rint;
                result = true;
                break;
            }
            if (lint > rint)
            {
                //L > R
                //qDebug() << "L > R:" << lint <<","<<rint;
                result = false;
                break;
            }
        }
        else
        {   //Comparing delimiters
            QString rs="";
            QString ls="";

            rs=rparts.at(i);
            ls=lparts.at(i);

            //qDebug() << "Mode=" << mode << " ls=" <<ls<< " rs=" << rs;

            if (ls != rs)
            {
                if (ls==".")
                {   //A . means it is later than any other alpha/beta version
                    //qDebug() << "L > R:" << ls <<","<<rs;
                    result=false;
                    break;
                }
                if (ls < rs)
                {
                    //qDebug() << "L < R:" << ls <<","<<rs;
                    result = true;
                    break;
                }
                if (ls > rs)
                {
                    //qDebug() << "L > R:" << ls <<","<<rs;
                    result = false;
                    break;
                }
            }
        }
        mode=!mode;
    }
    return (result);
}
