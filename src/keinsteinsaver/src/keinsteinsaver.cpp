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

#include <stdlib.h>
#include <qcheckbox.h>
#include <qcolor.h>
#include <kapplication.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kconfig.h>
#include <kglobal.h>
#include "keinsteinsaver.h"
#include "keinsteinsaverui.h"

//! libkscreensaver interface
extern "C"
{
    const char *kss_applicationName = "keinsteinsaver.kss";
    const char *kss_description = I18N_NOOP( "keinsteinsaver" );
    const char *kss_version = "2.2.0";

    keinsteinsaver *kss_create( WId id )
    {
        KGlobal::locale()->insertCatalogue("keinsteinsaver");
        return new keinsteinsaver( id );
    }

    QDialog *kss_setup()
    {
        KGlobal::locale()->insertCatalogue("keinsteinsaver");
        return new keinsteinsaverSetup();
    }
}

//-----------------------------------------------------------------------------
//! dialog to setup screen saver parameters
keinsteinsaverSetup::keinsteinsaverSetup( QWidget *parent, const char *name )
        : keinsteinsaverUI( parent, name, TRUE )
{
    /// @todo
    //Connect your signals and slots here to configure the screen saver.
    connect( OkayPushButton, SIGNAL( released() ),
             SLOT( slotOkPressed() ) );
    connect( CancelPushButton, SIGNAL( released() ),
             SLOT( slotCancelPressed() ) );
}


//! read settings from config file
void keinsteinsaverSetup::readSettings()
{
    KConfig *config = KGlobal::config();
    config->setGroup( "Settings" );
    /// @todo
    // Add your config options here...
    CheckBox1->setChecked(config->readBoolEntry( "somesetting", false ));
}


//! Ok pressed - save settings and exit
void keinsteinsaverSetup::slotOkPressed()
{
    KConfig *config = KGlobal::config();
    config->setGroup( "Settings" );
    /// @todo
    // Add your config options here.
    config->writeEntry( "somesetting", CheckBox1->isChecked() );
    config->sync();

    accept();
}

void keinsteinsaverSetup::slotCancelPressed()
{
    reject();
}
//-----------------------------------------------------------------------------


keinsteinsaver::keinsteinsaver( WId id ) : KScreenSaver( id )
{
    readSettings();
    blank();
}

keinsteinsaver::~keinsteinsaver()
{}


//! read configuration settings from config file
void keinsteinsaver::readSettings()
{
    KConfig *config = KGlobal::config();
    config->setGroup( "Settings" );
    /// @todo
    // Add your config options here...
    bool somesetting = config->readBoolEntry( "somesetting", false );
}


void keinsteinsaver::blank()
{
    /// @todo
    //Add your code to render the screen.
    setBackgroundColor( QColor(black)  );
    //
    erase();
}
