/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2011 Oliver Knoll <till.oliver.knoll@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QString>
#include <QRegExp>

#include "Version.h"

class VersionPrivate
{
public:
    VersionPrivate()
        : majorNo(MajorNo), minorNo(MinorNo), subMinorNo(SubMinorNo)
    {}

    VersionPrivate(int theMajor, int theMinor, int theSubMinor)
        : majorNo(theMajor), minorNo(theMinor), subMinorNo(theSubMinor)
    {}

    int majorNo;
    int minorNo;
    int subMinorNo;

    static const int MajorNo;
    static const int MinorNo;
    static const int SubMinorNo;
    static const QString CodeName;
    static const QString UserVersion;
    static const QString ApplicationTitle;
};

// Application version
const int VersionPrivate::MajorNo = 1;
const int VersionPrivate::MinorNo = 0;
const int VersionPrivate::SubMinorNo = 0;
const QString VersionPrivate::CodeName = QString("Anarchic Amoeba");
const QString VersionPrivate::UserVersion = QString("15.08");
const QString VersionPrivate::ApplicationTitle = QString("Screenie"); // note: no translation here (i18n)

// public

Version::Version()
    : d(new VersionPrivate())
{

}

Version::Version(int majorNo, int minorNo, int subMinorNo)
    : d(new VersionPrivate(majorNo, minorNo, subMinorNo))
{

}

Version::Version(const QString &version)
    : d(new VersionPrivate())
{
    QRegExp versionRegExp("^(\\d+)\\.(\\d+)\\.(\\d+)$");
    if (versionRegExp.indexIn(version) != -1) {
        d->majorNo = versionRegExp.cap(1).toInt();
        d->minorNo = versionRegExp.cap(2).toInt();
        d->subMinorNo = versionRegExp.cap(3).toInt();
    }
}

Version::~Version()
{
    delete d;
}

int Version::getMajor()
{
    return d->majorNo;
}

int Version::getMinor()
{
    return d->minorNo;
}

int Version::getSubminor()
{
    return d->subMinorNo;
}

QString Version::toString()
{
    return QString("%1.%2.%3").arg(d->majorNo).arg(d->minorNo).arg(d->subMinorNo);
}

bool Version::operator==(const Version &other)
{
    bool result;
    result = d->majorNo == other.d->majorNo && d->minorNo == other.d->minorNo && d->subMinorNo == other.d->subMinorNo;
    return result;
}

bool Version::operator>=(const Version &other)
{
    bool result;
    if (d->majorNo > other.d->majorNo) {
        result = true;
    } else if (d->majorNo < other.d->majorNo) {
        result = false;
    } else {
        if (d->minorNo > other.d->minorNo) {
            result = true;
        } else if (d->minorNo < other.d->minorNo) {
            result = false;
        } else {
            if (d->subMinorNo >= other.d->subMinorNo) {
                result = true;
            } else {
                result = false;
            }
        }
    }
    return result;
}

bool Version::operator<(const Version &other)
{
    return !(*this >= other);
}

QString Version::getCodeName()
{
    return VersionPrivate::CodeName;
}

QString Version::getUserVersion()
{
    return VersionPrivate::UserVersion;
}

QString Version::getApplicationVersion()
{
    Version version;
    return version.toString();
}

QString Version::getApplicationName()
{
    return VersionPrivate::ApplicationTitle;
}
