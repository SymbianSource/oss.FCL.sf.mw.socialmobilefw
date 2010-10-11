#include "Mainwindow.h"
#include <QUrl>
#include <QDebug>
#include <QImage>
#include <QVariant>
#include <QtGui>
#include <QDataStream>
#include <smfprovider.h>
#include <qcontactguid.h>


/*#ifndef TEST_FLICKR
#define TEST_FLICKR
#define PROVIDER_NAME  "Flickr"
*/
//Test Cases
/*
#define TEST_CASE_FLCRAlbums
#define TEST_CASE_FLCRUpload*/
//#define TEST_CASE_FLCRPictures

//#define TEST_CASE_FLCRMultiUpload
//#define TEST_CASE_FLCRDescription
//#define TEST_CASE_FLCRPostComment
//#endif
/*
 */
#ifndef  TEST_FACEBOOK
#define TEST_FACEBOOK
#define PROVIDER_NAME  "Facebook"

#define TEST_CASE_FBFrndActvities
#endif
/*
#ifndef  TEST_LastFm
#define TEST_LastFm
#define PROVIDER_NAME  "last.fm"
#endif


#ifndef  TEST_LyricsFly
#define TEST_LyricsFly
#define PROVIDER_NAME  "Last.fm"
#endif
*/





MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
	pB = new QPushButton("Start Test Application",this);
	QObject::connect(pB,SIGNAL(clicked()),this,SLOT(StartTesting()));

}
MainWindow::~MainWindow()
{
}
void MainWindow::StartTesting()
{
/*#ifdef TEST_FLICKR
TestFlickrAPIS();
#endif
*/

#ifdef TEST_FACEBOOK
TestFaceBookAPIS();
#endif

}

void MainWindow::TestFlickrAPIS()
{
		SmfClient m_smf;
		QList<SmfProvider> *providerList = m_smf.GetServices("org.symbian.smf.client.gallery");
		qDebug() <<"SmfGallery::get services  "<<providerList->length() ;
		qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
		
		SmfError error =(SmfError) 100;
		QString str = m_smf.errorString(error);
		
		qDebug() << "SmfClient::error string returned " << str;	
		foreach(SmfProvider provider, *providerList)
					{
						if(PROVIDER_NAME == provider.serviceName())
						{
						qDebug()<<"Plugin for flickr found";
													
						pSmfGallery = new SmfGallery(&provider);
#ifdef TEST_CASE_FLCRUpload
						
						TEST_CASE_FLCRUpload_func(pSmfGallery);

#endif	
		
#ifdef TEST_CASE_FLCRMultiUpload	
						TEST_CASE_FLCRMultiUpload_func(pSmfGallery);
#endif
						
#ifdef TEST_CASE_FLCRDescription
						TEST_CASE_FLCRGetDescription_func(pSmfGallery);
						
#endif	
						
#ifdef TEST_CASE_FLCRPostComment
						TEST_CASE_FLCRPostComment_func(pSmfGallery);
						
#endif
#ifdef TEST_CASE_FLCRPictures
						TEST_CASE_FLCRGetPictures_func(pSmfGallery);
						
#endif
#ifdef TEST_CASE_FLCRAlbums
						TEST_CASE_FLCRGetAlbums_func(pSmfGallery);
						
#endif						
						}
					}
}
void MainWindow::TestFaceBookAPIS()
{
	SmfClient m_smf;
	QList <SmfProvider> *services = m_smf.GetServices("org.symbian.smf.plugin.activity.fetcher");
	qDebug() << "SmfClient::GetServices returned " << services->length() << " services.";
	
	foreach(SmfProvider provider, *services)
	    {
	    if(PROVIDER_NAME  == provider.serviceName())
	        {
	        qDebug()<<"Plugin for Facebook found";
	        activityFetcher = new SmfActivityFetcher(&provider);
	        
#ifdef TEST_CASE_FBFrndActvities
	        
	        TEST_CASE_FBFriendsActivities_func(activityFetcher);
	        
#endif
	        }
	    }
}
void MainWindow::TEST_CASE_FLCRUpload_func(SmfGallery *pSmfGallery)
	{
	SmfPictureAlbum smfPictureAlbum;
	SmfPicture smfImage;
	smfImage.setPicture(QImage("C:\\data\\4.jpg","JPG"));
	bool ret = connect(pSmfGallery,SIGNAL(uploadFinished(QList<SmfError> )),
								this,SLOT(uploadFinishedSlot(QList<SmfError> ))); 
	pSmfGallery->upload(&smfImage,&smfPictureAlbum);
	qDebug()<<"SmfGallery::upload picture ";
	}

void MainWindow::TEST_CASE_FLCRMultiUpload_func(SmfGallery *pSmfGallery)
	{

	SmfPictureAlbum smfPictureAlbum;
	smfPictureAlbum.setId("72157624246208730");
	SmfPicture smfPicture1,smfPicture2;
	smfPicture1.setPicture(QImage("C:\\data\\Cyber_towers.jpg","jpg"));
	smfPicture2.setPicture(QImage("C:\\data\\test.jpg","jpg"));          
	QList<SmfPicture> smfPicList;
	smfPicList.append(smfPicture1);
	smfPicList.append(smfPicture2);
	bool ret = connect(pSmfGallery,SIGNAL(uploadFinished(QList<SmfError> )),
										        this,SLOT(uploadFinishedSlot(QList<SmfError> ))); 
	pSmfGallery->upload(&smfPicList,&smfPictureAlbum);
	qDebug()<<"SmfGallery::upload picture List ";
	}
void MainWindow::TEST_CASE_FLCRGetDescription_func(SmfGallery *pSmfGallery)
	{
	SmfPicture smfImage;
	smfImage.setId("5058988107");
	SmfError err = pSmfGallery->description(smfImage);
	bool ret = QObject::connect(pSmfGallery,SIGNAL(descriptionAvailable(QString,SmfError)),
								this,SLOT(descSlot(QString,SmfError ))); 					    	
	qDebug()<<"SmfGallery::get description ";
	}
void MainWindow::TEST_CASE_FLCRPostComment_func(SmfGallery *pSmfGallery)
	{
	SmfPicture smfImage;
	smfImage.setId("5058988107");
	SmfComment smfComment;
	smfComment.setText("Hello Symbian");
	SmfError err = pSmfGallery->postComment(smfImage,smfComment);
	
	bool ret = QObject::connect(pSmfGallery,SIGNAL(uploadFinished(QList<SmfError> )),
			this,SLOT(uploadFinishedSlot(QList<SmfError> ))); 					    	
	qDebug()<<"SmfGallery::Post a Comment ";
	}
void MainWindow::TEST_CASE_FLCRGetPictures_func(SmfGallery *pSmfGallery)
	{
	QList<SmfPictureAlbum> smfAlbumList;
	SmfPictureAlbum smfPictureAlbum;
	smfPictureAlbum.setId("72157624966587095");
	smfAlbumList.append(smfPictureAlbum);
	int pageNum = 0, perPage = 0;
	SmfError err = pSmfGallery->pictures(smfAlbumList,pageNum , perPage);
	bool ret = connect(pSmfGallery,SIGNAL(picturesAvailable(SmfPictureList*, SmfError , SmfResultPage)),
			this, SLOT(picturesAvailableSlot(SmfPictureList*, SmfError , SmfResultPage))); 
	qDebug()<<"SmfGallery::Get picture ";
	}
void MainWindow::TEST_CASE_FLCRGetAlbums_func(SmfGallery *pSmfGallery)
	{
	SmfPictureAlbum smfPictureAlbum;
	SmfPicture smfImage;
	smfImage.setPicture(QImage("C:\\data\\4.jpg","JPG"));
	bool ret = connect(pSmfGallery,SIGNAL(albumsAvailable(SmfPictureAlbumList*, SmfError , SmfResultPage)),
			this, SLOT(albumsAvailableSlot(SmfPictureAlbumList*, SmfError , SmfResultPage)));
	QStringList names;
	SmfContact user; 
	pSmfGallery->albums(names, &user,1,2);
	qDebug()<<"SmfGallery::upload picture ";
	}
void MainWindow::TEST_CASE_FBFriendsActivities_func(SmfActivityFetcher *actvityFetcher)
	{
	
	 SmfContact m_contact;
	 QContactGuid guid;
	 guid.setGuid("100001376714970");
	 QVariant contactId = QVariant::fromValue<QContactGuid>(guid);
	 m_contact.setValue("Guid",contactId);
	 actvityFetcher->friendsActivities(m_contact,1,1);
	 bool ret = connect(actvityFetcher, SIGNAL(resultsAvailable(SmfActivityEntryList*, SmfError , SmfResultPage)),
	 					this, SLOT(activitiesAvailableSlot(SmfActivityEntryList*, SmfError , SmfResultPage)));
	 qDebug()<<"connected"<<ret;
         			
         			}
void MainWindow::albumsAvailableSlot(SmfPictureAlbumList* albums, SmfError error, SmfResultPage resultPage)
	{
	Q_UNUSED(resultPage);
			
	qDebug()<<"Inside HomeView::albumsAvailable()";
	
	m_albumsList = albums;
	if(error)
		{
		SmfClient client;
		QString errStr = client.errorString(error);
		qDebug()<<"Error found, code = "<<error;
		qDebug()<<"Error string is = "<<errStr;
		return;
		}
	
	//display album details
	qDebug()<<"Number of albums retrieved = "<<albums->count();
	if(albums->count() == 0)
		{
		QString smferrString("No Albums");
		return;
		}
	
	QStringList albumPicList;
	QStringList albumNameList;
	foreach(SmfPictureAlbum album, *m_albumsList)
		{
		
		qDebug()<<"Album name = "<<album.title();
		qDebug()<<"Album description = "<<album.description();
		qDebug()<<"Album pictureCount = "<<album.pictureCount();
		qDebug()<<"Album id = "<<album.id();
		}
	

	}

void MainWindow::picturesAvailableSlot(SmfPictureList* picList, SmfError error, SmfResultPage resultPage)
	{
	Q_UNUSED(resultPage)
	qDebug()<<"Inside HomeView::picturesAvailable()";
	
	m_picList = picList;
	if(error)
		{
		SmfClient client;
		QString errStr = client.errorString(error);
		qDebug()<<"Error found, code = "<<error;
		qDebug()<<"Error string is = "<<errStr;
		return;
		}
	
	//display pic description
	qDebug()<<"Number of pic retrieved = "<<picList->count();
	
	QMap<QString, QUrl> urlMap;
	foreach(SmfPicture pic, *picList)
		{
		qDebug()<<"Photo title = "<<pic.title();
		qDebug()<<"Photo posted date = "<<pic.postedDate();
		qDebug()<<"Photo url = "<<pic.url();
		qDebug()<<"Photo id = "<<pic.id();
		}

	
	}

void MainWindow::uploadFinishedSlot( QList<SmfError> error )
{
	SmfClient m_smf;
	QString str = m_smf.errorString(error.at(0));
	qDebug()<<"upload finished";
	qDebug()<<" SMF Error Code"<<str;
 }

void MainWindow::descSlot(QString str,SmfError err )
{
	SmfClient m_smf;
	QString errstr = m_smf.errorString(err);
	qDebug()<<"description string"<<str;
	qDebug()<<" SMF Error Code"<<errstr;
 }
void MainWindow::activitiesAvailableSlot(SmfActivityEntryList* activitiesList, SmfError error, SmfResultPage resultPage)
		{
		Q_UNUSED(resultPage)
		qDebug()<<"Inside HomeView::activitiesAvailable()";
		
		
		qDebug()<<"Number of activities retrieved = "<<activitiesList->count();
		
			SmfClient client;
			QString errStr = client.errorString(error);
			qDebug()<<"Error found, code = "<<error;
			qDebug()<<"Error string is = "<<errStr;
			
			
			
		//display activity description
		qDebug()<<"Number of activities retrieved = "<<activitiesList->count();
		
		
		
		foreach(SmfActivityEntry activity, *activitiesList)
			{
			qDebug()<<"Activity author name = "<<activity.author().value("Name").value<QContactName>().firstName();
			qDebug()<<"Activity title = "<<activity.title().title();
			qDebug()<<"Activity details = "<<activity.details().description();
			}

		
}
