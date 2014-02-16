/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2014 Oliver Knoll <till.oliver.knoll@gmail.com>

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

#include "SecurityToken.h"

class SecurityTokenPrivate
{
public:
    SecurityTokenPrivate()
        : refCount(1)
    {}

    int refCount;
};

// public

void SecurityToken::retain()
{
    ++d->refCount;
}

void SecurityToken::release()
{
    --d->refCount;
    if (d->refCount == 0) {
        delete this;
    }
}

bool SecurityToken::isValid() const
{
    return true;
}

SecurityToken *SecurityToken::create(const QByteArray &securityTokenData)
{
    SecurityToken *result = new SecurityToken(securityTokenData);
    return result;
}

QByteArray SecurityToken::createSecurityTokenData(const QString &filePath)
{
    Q_UNUSED(filePath);
    QByteArray result;
    return result;
}

#ifdef DEBUG
void SecurityToken::debugTokenToFilePath(const QByteArray &securityTokenData)
{
    Q_UNUSED(securityTokenData)
}
#endif

// protected

SecurityToken::SecurityToken(const QByteArray &securityTokenData)
{
    Q_UNUSED(securityTokenData)

    d = new SecurityTokenPrivate();
}

SecurityToken::~SecurityToken()
{
    delete d;
}
