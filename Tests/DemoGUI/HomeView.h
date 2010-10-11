#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <qmainwindow.h>
#include <WidgetConstants.h>
//#include <smfglobal.h>
#include <smfcontact.h>
#include <smfpost.h>
#include <smfpicturealbum.h>
#include <smfactions.h>
#include <smfpicture.h>
#include <smfcontactfetcher.h>
#include <smfpostprovider.h>
#include <smfgallery.h>
#include <smfactivityfetcher.h>

//added by sangeeta 
#include <smfplaylist.h>
#include <smfmusic.h>
#include "ImageDownload.h"

class CustomListWidget;
class GridView;

class HomeView : public QObject
{
	Q_OBJECT
public:
    HomeView(QString aHeadName);
    ~HomeView();
    void SetupUI(QMainWindow *Mainwindow);
    void populateFriendsWidget();
    void populatePostsWidget();
    void populateActivitiesWidget();
    void populatePhotosGridView();
    void populatePlaylistWidget();
    void populateEventlistWidget();
    void populateRecommendationListWidget();
    
private:
    void getFriends();
    void getPosts();
    void downloadImages(const SmfItemIdentifier &identifier, const QMap<QString, QUrl> urlMap);
    void getAlbums();
    void getActivities();
    void getPhotos();
    void getEvents();
    void getPlayList();
    void getRecommendationList();
    
private slots:
    void friendsAvailable(SmfContactList* friendsList, SmfError , SmfResultPage );
    void postsAvailable(SmfPostList* postsList, SmfError , SmfResultPage );
    void albumsAvailable(SmfPictureAlbumList* albums, SmfError error, SmfResultPage resultPage);
    void activitiesAvailable(SmfActivityEntryList* postsList, SmfError , SmfResultPage );
    void picturesAvailable(SmfPictureList* albums, SmfError error, SmfResultPage resultPage);
    void playListAvailable(SmfPlaylistList* playlist, SmfError error, SmfResultPage resultPage);
    void EventListAvailable(SmfEventList* eventsList,SmfError error, SmfResultPage resultPage);
    void RecommendationListAvailable(SmfTrackInfoList* eventsList,SmfError error, SmfResultPage resultPage);
    void Navigated2OtherTab(int index);
    
private:
    QString iHeadName;
    QMainWindow *m_mainWindow;
    QTabWidget *TabWidget;
    CustomListWidget *iFrndsListWidget;
    CustomListWidget *iPostsListWidget;
    CustomListWidget *iActivityListWidget;
    CustomListWidget *iAlbumWidget;
    CustomListWidget *iPlaylistWidget;
    
    CustomListWidget *iEventslistWidget;
    CustomListWidget *iMusicSearchListWidget;
    GridView *AlbumView;
    ImageDownload m_downloader;
    
    SmfContactFetcher *m_contactFetcher;
    SmfPostProvider *m_postProvider;
    SmfGallery *m_gallery;
    SmfActivityFetcher *m_activityFetcher;
    SmfContactList *m_friendsList;
    SmfPostList *m_postsList;
    SmfPictureAlbumList *m_albumsList;
    SmfActivityEntryList *m_activitiesList;
    SmfPictureList *m_picList;
    SmfPlaylistService* m_playlistService;
  //  SmfMusicEvents* m_musicEvents;
    SmfTrackInfoList* m_TrackinfoList;
    
    //added by sangeeta
    
       
    SmfPlaylistList *m_playlist;
    SmfEventList *m_eventList;
    SmfMusicSearch *m_musicSearch;
    
};

#endif // HOMEVIEW_H
