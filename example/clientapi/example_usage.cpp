#include <smf/smfprovider.h>
#include <smf/smfgallery.h>
#include <smf/smfcontact.h>
#include <smf/smfpostprovider.h>
#include <smf/smffcontactfetcher.h>
#include <smf/smfmusic.h>

/** 1. Display a gallery on the screen for some remote service.
 * assume m_view is some gallery view object in the application.*/
void MyApplication::displayGallery()
	{
	// Some common interface for finding implementations.
	QList<SmfGallery> galleries = Smf::GetServices("org.symbian.smf.gallery\0.2");

	// We will use the first one now
	SmfGallery myGallery = galleries[0];

	// Adjust our view to show where these pictures came from
	m_view.setIcon(myGallery.serviceIcon());
	m_view.setProvder(myGallery.serviceName());
	m_view.setDescription(myGallery.description());

	/**
	 * Asynchrnous request to fetch the pictures.
	 * The picturesAvailable() signal is emitted 
	 * with SmfPictureList once the pictures have arrived.
	 */
	myGallery.pictures();
	QObject::connect(myGallery,SIGNAL(picturesAvailable(SmfPictureList*, QString, int)),this,SLOT(showPicsSlot(SmfPictureList*, QString)));
	}
}
void MyApplication::showPicsSlot(SmfPictureList* pics, QString err)
	{
	//check err string if there is any error

	//if no error
	foreach(SmfPicture* pic, pics) {
		m_view.add(pic); // do something with the picture in this gallery
	}
	}

/** 2. Upload a picture captured by the user to some selection of galeries.*/
void MyApplication::uploadPicture(QImage picture, QList<SmfGallery> galleries)
	{
	/**
	 * When uploading is finished we can check the success of the uploading
	 */	
	QObject::connect(myGallery,SIGNAL(uploadFinished(bool)),this,SLOT(uploaded(bool)));
	// The list could be from a selection of galleries chosen by the user,
	// think multiple TweetDeck accounts?
	foreach(SmfGallery gallery, galleries) {
		gallery.upload(picture);
	}
	}
void MyApplication::uploaded(bool success)
	{
	if(!success)
		{
	//error occured while uploading
		}
	}

/**
 * 3. This is an example of displaying the friends profile image in a view from one or more
 * service provider. Note that this service can be provided by any kind of service provider,
 * e.g. last.fm music service where users maintain profiles and friends.
 */
void MyApplication::displayFriends()
	{
	// Some common interface for finding implementations.
	QList<SmfContactFetcher> contactFetcherList = Smf::GetServices("org.symbian.smf.contact.fetcher\0.2");
	//Request friend list,
	//The friendsListAvailable() signal
	//is emitted with SmfContactList once data is arrived.
	QObject::Connect(contactFetcherList[0],SIGNAL(friendsListAvailable(SmfContactList*, QString, int)),
			this,SLOT(showlist(SmfContactList*));
	fetcher.friends();
	}

void MyApplication::showlist(SmfContactList* friendsList)
	{

	// Adjust our view to show where these pictures came from
	//display service name description and the logo
	m_view.setIcon( myFetcher.serviceIcon() );
	m_view.setProvider( myFetcher.serviceName() );
	m_view.setDescription( myFetcher.description() );

	//now display the images
	foreach(SmfContact* contact, friendsList) {
		QImage pic = contact.value("Avatar");
		m_view.setPicture(pic);
		m_view.setTitle(contact.value("Name"));
	}
	}
/**
 * 4. This is an example of posting and reading user updates to social netowrking sites
 */
void MyApplication::postUpdate()
	{
	// Some common interface for finding implementations.
	QList<SmfPostProvider> postServices = Smf::GetServices("org.symbian.smf.contact.posts\0.2");

	//let us use the first one
	SmfPostProvider myPostServer = postServices[ 0 ];

	//Adjust our view to show where these posts came from (e.g. tweets from twitter)
	//display service name description and the logo
	m_view.setIcon( myPostServer.serviceIcon() );
	m_view.setProvider( myPostServer.serviceName() );
	m_view.setDescription( myPostServer.description() );

	SmfPost reply = new SmfPost(sampleString,samplmage, sampleUrl);
	//post my udpate to be visible to all, connect to updatePostFinished()
	// signal of SmfPostProvider to track the success
	myPostServer.updatePost(reply);

	//Asynchronously get all posts to me in my profle (e.g. twits from all friends)
	//connect to postsAvailable to show the post
	postServices.getPosts();

	QObject::Connect(myPostServer,SIGNAL(postsAvailable(SmfPostList*, QString, int)),this,SLOT(showPosts(SmfPostList*, QString)));
	}
void MyApplication::showPosts(SmfPostList* posts, QString err)
	{
	//Show the first post  
	m_view.setPostData(posts->at(0));
	}
/**
 * 5. This is an example of getting song recommendations from a social netowrking sites
 */
void MyApplication::getMusic()
	{
	// Some common interface for finding implementations.
	QList<SmfMusicSearch> musicServices = Smf::GetServices("org.symbian.smf.music\0.2");

	//let us use the first one
	SmfMusicSearch mServer = musicServices.at(0);
	QObject::Connect(mServer,SIGNAL(trackSearchAvailable(SmfTrackInfoList*, QString,int)),this,SLOT(showTrackSearch(SmfTrackInfoList*)));
	QObject::Connect(mServer,SIGNAL(storeSearchAvailable(SmfProviderList*, QString,int)),this,SLOT(showStoreSearch(SmfProviderList*)));
	//search songs similar to currently playing,
	//connect to trackSearchAvailable signal to get the result
	mServer.recommendations(currTrack);
	//display to the user
	m_view.setIcon( mServer.serviceIcon() );
	m_view.setProvider( mServer.serviceName() );
	m_view.setDescription( mServer.description() );



	}
void MyApplication::showTrackSearch(SmfTrackInfoList* songs)
	{
	foreach(SmfTrackInfo* track, songs){
		m_view.add(track);
	}
	//allow user to select a track and get purchase links
	//connect to showStoreSearch signal to display the stores for that track
	mServer.stores(selectedTrack);
	}
void MyApplication::showStoreSearch(SmfProviderList* stores)
	{
	//show stores
	}
void MyApplication::updateCurrentPlaying(QList<SmfMusicSearch> musicServices, SmfTrackInfo currTrack)
	{
	//after purchasing and downloading is over, user plays the track
	//now post the current platying track to all service providers
	//postFinished() signal of SmfMusicSearch can be tracked to check the success of the posts
	foreach(SmfMusicSearch provider, musicServices) {
		provider.postCurrentPlaying(currTrack);
	}
	//postCurrentPlaying is also a slot funtion, may be application can use connect
	}

void MyApplication::displayLyrics(SmfTrackInfo currTrack)
	{
	// Some common interface for finding implementations.
	SmfLyricsService lyricsService = Smf::GetServices("org.symbian.smf.music.lyrics\0.2","lyricsfly.com");
	QObject::connect(lyricsService,SIGNAL(lyricsAvailable(SmfLyricsList*, QString, int)),this,SLOT(showLyrics(SmfLyricsList*));

	//Request to get the lyrics
	//lyricsAvailable() signal of SmfLyricsService is emitted when lyrics is available
	lyricsService.lyrics(currTrack);

	}
void MyApplication::showLyrics(SmfLyricsList* list)
	{
	//now display the latest edited lyrics
	qSort(list->begin(),list->end(),caseCompareTimeMoreThan);
	m_view.setLyricsData(list->at(0));
	}
