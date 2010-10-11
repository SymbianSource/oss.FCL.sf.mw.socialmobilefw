/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Siddhartha Chandra, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
* Siddhartha Chandra, Satish Kanteti Sasken Communication Technologies Ltd
* Description:
* UI Class of Authentication Application
*/

#include "AuthApp.h"
#include <QDebug>
#include <QMessageBox>

#include "keys.h"

AuthApp::AuthApp(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::AuthAppClass)
{
	qDebug()<<"Inside AuthApp::AuthApp()";
	ui->setupUi(this);
	iFBSession = FBSession::sessionForApplication(kConsumerKey,kConsumerSecret, QString());
	connect (iFBSession,SIGNAL(sessionDidLogin(QString)), this, SLOT(sessionDidLogin(QString)));
    connect (iFBSession, SIGNAL(sessionDidLogout()), this, SLOT(sessionDidLogout()));
    
    //CheckforLogin();
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
	qDebug()<<"Inside AuthApp::CheckforLogin()";
	iLoginDialog = NULL;
	if ( !( iFBSession->resume() ) ){
		ui->buttonForget->setDisabled(true);
		ui->pushButton->setEnabled(true);
	}
}
void AuthApp::on_pushButton_clicked()
{
	qDebug()<<"Inside AuthApp::on_pushButton_clicked()";
	iLoginDialog = new FBLoginDialog();
	//iLoginDialog->connectToGetToken();
	iLoginDialog->show();
}
void AuthApp::sessionDidLogin(QString sessionkey)
{
	Q_UNUSED(sessionkey)
	qDebug()<<"Inside AuthApp::sessionDidLogin()";
    if (iLoginDialog )
    {
		QMessageBox msgbox;
		QString msg ("Authorization completes!!");
		msgbox.setText(msg);
		msgbox.exec();
		
        iLoginDialog->deleteLater();;
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
	qDebug()<<"Inside AuthApp::sessionDidLogout()";
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
	qDebug()<<"Inside AuthApp::on_buttonForget_clicked()";
    iFBSession->logout();
}
