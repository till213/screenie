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

#ifndef SECURITYTOKEN_H
#define SECURITYTOKEN_H

#include <QByteArray>

#include "UtilsLib.h"

class SecurityTokenPrivate;

/*!
 * \brief Provides file security tokens for permanent access across application restarts.
 *
 * The security token is reference-counted with #create, #retain and #release. Each create/retain call must be
 * balanced with the corresponding release call.
 *
 * For an introduction to Security-Scoped Bookmarks ("security token data") also refer to:
 * https://developer.apple.com/library/mac/documentation/security/conceptual/AppSandboxDesignGuide/AppSandboxInDepth/AppSandboxInDepth.html#//apple_ref/doc/uid/TP40011183-CH3-SW16
 */
class SecurityToken
{
public:

    /*!
     * Increases the reference counter by 1. Call this method if you want to keep
     * a reference to \em this instance.
     */
    UTILS_API void retain();

    /*!
     * Decreases the reference counter by 1. If the reference counter reaches 0 then
     * \em this instance is \c deleted. This instance is then invalid and no
     * further instance methods must be called after a release.
     */
    UTILS_API void release();

    UTILS_API bool isValid() const;

    /*!
     * Creates an instance of the SecurityToken. The reference count will be 1. In
     * order to \c delete the object call #release.
     *
     * \param securityTokenData
     *        the security token data which has been created with createSecurityTokenData()
     * \sa #release()
     * \sa #createSecurityTokenData(const QString &)
     */
    UTILS_API static SecurityToken *create(const QByteArray &securityTokenData);

    /*!
     * Creates a security access token data for the given \p filePath in order to access
     * files after the restart of a sandboxed application. The token
     * generation is platform-specific:
     * - On OS X Security-Scoped Bookmarks are generated
     * - On all other platforms an empty QByteArray is currently returned (= no
     *   security token)
     */
    UTILS_API static QByteArray createSecurityTokenData(const QString &filePath);

#ifdef DEBUG
    UTILS_API static void debugTokenToFilePath(const QByteArray &securityTokenData);
#endif

protected:
    explicit SecurityToken(const QByteArray &securityTokenData);
    virtual ~SecurityToken();

private:
    Q_DISABLE_COPY(SecurityToken)

    SecurityTokenPrivate *d;
};

#endif // SECURITYTOKEN_H
