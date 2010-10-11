#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<smfglobal.h>
#include<QPushButton>
#include <QWidget>
#include <smfclient.h>
#include <smfactivityfetcher.h>
#include <smfpicture.h>
#include <smfpicturealbum.h>
#include <smfContact.h>
#include <smfcomment.h>
#include <smfgallery.h>

class MainWindow : public QWidget
{
	Q_OBJECT
public:
MainWindow(QWidget *parent =0);
~MainWindow();
QPushButton *pB; 

void TestFlickrAPIS();
void TestFaceBookAPIS();
//void TestActivities();
void TestUpload();

////////////////////////////////////////////
void TEST_CASE_FLCRUpload_func(SmfGallery *pSmfGallery);
void TEST_CASE_FLCRMultiUpload_func(SmfGallery *pSmfGallery);
void TEST_CASE_FLCRGetDescription_func(SmfGallery *pSmfGallery);
void TEST_CASE_FLCRPostComment_func(SmfGallery *pSmfGallery);
void TEST_CASE_FLCRGetPictures_func(SmfGallery *pSmfGallery);
void TEST_CASE_FLCRGetAlbums_func(SmfGallery *pSmfGallery);
void TEST_CASE_FBFriendsActivities_func(SmfActivityFetcher *actvityFetcher);


////////////////////////////////////////////////
private slots:
void StartTesting();
void activitiesAvailableSlot(SmfActivityEntryList* activitiesList, SmfError error, SmfResultPage resultPage);
void uploadFinishedSlot( QList<SmfError> error );
void descSlot(QString str,SmfError err );
void picturesAvailableSlot(SmfPictureList* picList, SmfError error, SmfResultPage resultPage);
void albumsAvailableSlot(SmfPictureAlbumList* albums, SmfError error, SmfResultPage resultPage);
private:
SmfPictureAlbumList *m_albumsList;
SmfPictureList *m_picList;
SmfGallery *pSmfGallery ;
SmfActivityFetcher *activityFetcher;
};
#endif
