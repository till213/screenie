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

#import <Foundation/NSURL.h>
#import <Foundation/NSError.h>
#import <Foundation/NSString.h>

#import <QByteArray>
#import <QString>
#import <QtMac>

#import "SecurityToken.h"

#include "SecurityToken.h"

namespace
{
    bool startAccess(const QByteArray &securityTokenData)
    {
        NSData *bookmarkData;
        NSError *error = nil;
        BOOL bookmarkDataIsStale;
        NSURL *url;
        bool result;

        if (!securityTokenData.isNull()) {
            bookmarkData = securityTokenData.toNSData();
            url = [NSURL URLByResolvingBookmarkData:bookmarkData
                                            options:NSURLBookmarkResolutionWithSecurityScope
                                      relativeToURL:nil
                                bookmarkDataIsStale:&bookmarkDataIsStale
                                              error:&error];
            if (error == nil && bookmarkDataIsStale == NO) {
                [url startAccessingSecurityScopedResource];
                result = true;
            } else {
                result = false;
            }
        } else {
            result = true;
        }

        return result;
    }

    void stopAccess(const QByteArray &securityTokenData)
    {
        NSData *bookmarkData;
        NSError *error = nil;
        BOOL bookmarkDataIsStale;
        NSURL *url;

        if (!securityTokenData.isNull()) {
            bookmarkData = securityTokenData.toNSData();
            url = [NSURL URLByResolvingBookmarkData:bookmarkData
                                            options:NSURLBookmarkResolutionWithSecurityScope
                                      relativeToURL:nil
                                bookmarkDataIsStale:&bookmarkDataIsStale
                                              error:&error];

            [url stopAccessingSecurityScopedResource];
        }
    }
}

class SecurityTokenPrivate
{
public:
    SecurityTokenPrivate(const QByteArray theSecurityTokenData)
        : securityTokenData(theSecurityTokenData),
          refCount(1),
          valid(false)
    {}

    // make a copy of the original security token data, as the original data
    // in RecentFile might get deleted while moving position in the recent file list
    const QByteArray securityTokenData;
    int refCount;
    bool valid;
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
    return d->valid;
}

SecurityToken *SecurityToken::create(const QByteArray &securityTokenData)
{
    SecurityToken *result = new SecurityToken(securityTokenData);
    return result;
}

QByteArray SecurityToken::createSecurityTokenData(const QString &filePath)
{
    QByteArray result;
    NSString *nsFilePath;
    NSURL *url;
    NSData *bookmarkData;
    NSError *error;

    nsFilePath = filePath.toNSString();
    url = [NSURL fileURLWithPath:nsFilePath isDirectory:NO];
    error = nil;
    bookmarkData = [url bookmarkDataWithOptions:NSURLBookmarkCreationWithSecurityScope
             includingResourceValuesForKeys:nil
                              relativeToURL:nil // app-scoped bookmark
                                      error:&error];
    if (bookmarkData != nil) {
        result.fromNSData(bookmarkData);
    }

    return result;
}

#ifdef DEBUG
void SecurityToken::debugTokenToFilePath(const QByteArray &securityTokenData)
{
    NSData *bookmarkData;
    NSError *error = nil;
    BOOL bookmarkDataIsStale;
    NSURL *url;

    if (!securityTokenData.isNull()) {
        bookmarkData = securityTokenData.toNSData();
        url = [NSURL URLByResolvingBookmarkData:bookmarkData
                                        options:NSURLBookmarkResolutionWithSecurityScope
                                  relativeToURL:nil
                            bookmarkDataIsStale:&bookmarkDataIsStale
                                          error:&error];

        NSString *filePath = [url absoluteString];
        if (url != nil) {
            NSLog(@"SecurityToken::tokenToFilePath: %@", filePath);
        } else {
            qDebug("SecurityToken::tokenToFilePath: Returned URL is NULL!");
        }
    } else {
        qDebug("SecurityToken::tokenToFilePath: token data is NULL!");
    }
}
#endif

// protected

SecurityToken::SecurityToken(const QByteArray &securityTokenData)
{
    d = new SecurityTokenPrivate(securityTokenData);
    d->valid = startAccess(securityTokenData);
#ifdef DEBUG
    qDebug("SecurityToken::SecurityToken(): called. Valid: %d", d->valid);
    debugTokenToFilePath(d->securityTokenData);
#endif
}

SecurityToken::~SecurityToken()
{
#ifdef DEBUG
    qDebug("SecurityToken::~SecurityToken(): called.");
    debugTokenToFilePath(d->securityTokenData);
#endif
    stopAccess(d->securityTokenData);
    delete d;
}

