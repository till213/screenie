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

#include "PlatformManager.h"
#include "PlatformManagerFactory.h"

class PlatformManagerFactoryPrivate
{
public:
    static PlatformManagerFactory *instance;
};

PlatformManagerFactory *PlatformManagerFactoryPrivate::instance = nullptr;

// public

PlatformManagerFactory &PlatformManagerFactory::getInstance()
{
    if (PlatformManagerFactoryPrivate::instance == nullptr) {
        PlatformManagerFactoryPrivate::instance = new PlatformManagerFactory();
    }
    return *PlatformManagerFactoryPrivate::instance;
}

void PlatformManagerFactory::destroyInstance()
{
    if (PlatformManagerFactoryPrivate::instance != nullptr) {
        delete PlatformManagerFactoryPrivate::instance;
        PlatformManagerFactoryPrivate::instance = nullptr;
    }
}

// protected

PlatformManagerFactory::~PlatformManagerFactory()
{
#ifdef DEBUG
    qDebug("PlatformManagerFactory::~PlatformManagerFactory: CALLED.");
#endif
}

// private

PlatformManagerFactory::PlatformManagerFactory()
{}
