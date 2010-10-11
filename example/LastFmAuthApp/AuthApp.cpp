/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Narasimhulu Kavadapu, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
* Siddhartha Chandra, Sasken Communication Technologies Ltd
* Description:
* UI Class of Authentication Application
*/

#include "AuthApp.h"
#include <QDebug>
#include <QMessageBox>
#include <keys.h>

AuthApp::AuthApp(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::AuthAppClass)
{
	ui->setupUi(this);
	iFBSession = FBSession::sessionForApplication(kApiKey, kApiSecret, QString());//LastFm Web based
    connect (iFBSession, SIGNAL(sessionDidLogout()), this, SLOT(sessionDidLogout()));
    connect (iFBSession,SIGNAL(sessionDidLogin(QString)), this, SLOT(sessionDidLogin(QString)));
}

AuthApp::~AuthApp()
{
	delete iFBSession;
	delete ui;
}

void AuthApp::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void AuthApp::CheckforLogin()
{
	iLoginDialog = NULL;
	if ( !( iFBSession->resume() ) ){
		ui->buttonForget->setDisabled(true);
		ui->pushButton->setEnabled(true);
	}
}
void AuthApp::on_pushButton_clicked()
{
	qDebug()<<"Inside AuthApp::on_pushButton_clicked";
        iLoginDialog = new FBLoginDialog();
        iLoginDialog->connecttoGetFrob();
    
}
void AuthApp::sessionDidLogin(QString Token)
{
	Q_UNUSED(Token)
    if (iLoginDialog )
    {
		QMessageBox msgbox;
		QString msg ("Authorization completes!!");
		msgbox.setText(msg);
		msgbox.exec();
		
        iLoginDialog->deleteLater();
        iLoginDialog = NULL;
    }else{
		QMessageBox msgbox;
		QString msg ("your already authorized,Please logout and Login again for new authorization!!");
		msgbox.setText(msg);
		msgbox.exec();
    }
    
    ui->pushButton->setDisabled(true);
    ui->buttonForget->setEnabled(true);
}

void AuthApp::sessionDidLogout()
{
    QMessageBox msgbox;
    msgbox.setText("logged out successfully!!");
    msgbox.exec();
    
    ui->pushButton->setEnabled(true);
    ui->buttonForget->setDisabled(true);
    
}
void AuthApp::requestFailedWithFacebookError ( const FBError& aError )
{
    qDebug() << "facebook error is " << aError.code() << " - " << aError.description();
}

void AuthApp::on_buttonForget_clicked()
{
    iFBSession->logout();
}
