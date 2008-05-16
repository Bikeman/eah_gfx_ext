/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   This file is part of Einstein@Home.                                   *
 *                                                                         *
 *   Einstein@Home is free software: you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published     *
 *   by the Free Software Foundation, version 2 of the License.            *
 *                                                                         *
 *   Einstein@Home is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifndef keinsteinsaver_H__
#define keinsteinsaver_H__

#include "kscreensaver.h"
#include "keinsteinsaverui.h"

class keinsteinsaver : public KScreenSaver
{
    Q_OBJECT
public:
    keinsteinsaver( WId drawable );
    virtual ~keinsteinsaver();
private:
    void readSettings();
    void blank();
};

class keinsteinsaverSetup : public keinsteinsaverUI
{
    Q_OBJECT
public:
    keinsteinsaverSetup( QWidget *parent = NULL, const char *name = NULL );

private slots:
    void slotOkPressed();
    void slotCancelPressed();

private:
    void readSettings();
    keinsteinsaver *saver;
};

#endif
