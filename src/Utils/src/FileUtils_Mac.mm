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
#import <Foundation/NSString.h>

#import <QByteArray>
#import <QString>
#import <QtMac>

#import "FileUtils.h"

QByteArray FileUtils::createFileAccessBookmark(const QString &filePath)
{
    QByteArray result;
    NSString *nsFilePath;
    NSURL *url;
    NSData *bookmark;
    NSError *error;

    // workaround: there should be a QtMac::toNSString function, but apparently there is no such function
    nsFilePath = filePath.toNSString();
    url = [NSURL fileURLWithPath:nsFilePath isDirectory:NO];
    error = nil;
    bookmark = [url bookmarkDataWithOptions:NSURLBookmarkCreationWithSecurityScope
             includingResourceValuesForKeys:nil
                              relativeToURL:nil // app-scoped bookmark
                                      error:&error];
    if (!error) {
        result = QtMac::fromNSData(bookmark);
    }

    return result;
}
