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
  QList<SmfGallery> galleries = Smf::GetServices("org.symbian.smf.gallery");

  // We will use the first one now
  SmfGallery myGallery = galleries[0];

  // Adjust our view to show where these pictures came from
  m_view.setIcon(myGallery.serviceIcon());
  m_view.setProvder(myGallery.serviceName());
  m_view.setDescription(myGallery.description());

  QList<SmfPicture> pics = myGallery.pictures();
  foreach(SmfPicture pic, pics) {
    m_view.add(pic); // do something with the picture in this gallery
  }
}

/** 2. Upload a picture captured by the user to some selection of galeries.*/
void MyApplication::uploadPicture(QImage picture, QList<SmfGallery> galleries)
{
  // The list could be from a selection of galleries chosen by the user,
  // think multiple TweetDeck accounts?
  foreach(SmfGallery gallery, galleries) {
    gallery.upload(picture);
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
    QList<SmfContactFetcher> contactFetcherList = Smf::GetServices("org.symbian.smf.contact.fetcher");

    //let us show list of friends from first one
    showlist(contactFetcherList [ 0 ]);

    //now from the second provider
    showlist(contactFetcherList [ 1 ]);

    //user now matches one contact from list 1 to another contact in list 2 -
	SmfRelationMgr mgr = Smf::GetRelationMgr();
	SmfRelationId id = mgr.create(contactFetcherList [ 0 ], selectedContact1);
	mgr.associate(id,contactFetcherList [ 1 ], selectedContact2);

	//now show user all the relations he has made so far
	QList<SmfRelationId> relations = mgr.getAll();
	foreach(SmfRelationId id, relations) {
		QList<SmfRelationItem> items =  mgr.get(id);
		foreach(SmfRelationItem item,items) {
			SmfProvider provider* = item.getProvider();
        	m_view.setIcon(provider.serviceIcon());
        	m_view.setTitle(item.value("Name"));
		}
	}
}

void MyApplication::showlist(SmfContactFetcher fetcher)
{
    //get users friend list
    QList<SmfContact> friendsList  =  fetcher.friends();

    // Adjust our view to show where these pictures came from
    //display service name description and the logo
    m_view.setIcon( myFetcher.serviceIcon() );
    m_view.setProvider( myFetcher.serviceName() );
    m_view.setDescription( myFetcher.description() );

    //now display the images
    foreach(SmfContact contact, friendsList) {
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
    QList<SmfPostProvider> postServices = Smf::GetServices("org.symbian.smf.contact.posts");

    //let us use the first one
    SmfPostProvider myPostServer = postServices[ 0 ];

    //Adjust our view to show where these posts came from (e.g. tweets from twitter)
    //display service name description and the logo
    m_view.setIcon( myPostServer.serviceIcon() );
    m_view.setProvider( myPostServer.serviceName() );
    m_view.setDescription( myPostServer.description() );

    SmfPost reply = new SmfPost(sampleString,samplmage, sampleUrl);
    //post my udpate to be visible to all
    myPostServer.updatePost(reply);

    //get all posts to me in my profle (e.g. twits from all friends)
    QList <SmfPost> posts = postServices.getPosts();

    //read the first post
    SmfPost firstPost =  posts.at(0);
    SmfContact currentContact = firstPost.contact();

    //reply only to the sender - can check availability this service before sending
    myPostServer.postDirected(currentContact,reply);
    //presentation layout to be decided

    //now display the latest post
    qSort(posts.begin(),posts.end(),caseCompareTimeMoreThan);
    m_view.setPostData(posts.at(0));

}

/**
  * 5. This is an example of getting song recommendations from a social netowrking sites
  */
void MyApplication::getMusic()
{
    // Some common interface for finding implementations.
    QList<SmfMusicSearch> musicServices = Smf::GetServices("org.symbian.smf.music");

    //let us use the first one
    SmfMusicSearch mServer = musicServices.at(0);

    //search songs similar to currently playing
    QList<SmfTrackInfo> songs = mServer.recommendations(currTrack);

    //display to the user
    m_view.setIcon( mServer.serviceIcon() );
    m_view.setProvider( mServer.serviceName() );
    m_view.setDescription( mServer.description() );
    foreach(SmfTrackInfo track, songs)    {
		m_view.add(track);
		}

    //allow user to select a track and get purchase links
    QList<SmfProvider> stores = mServer.stores(selectedTrack);

}

void MyApplication::updateCurrentPlaying(QList<SmfMusicSearch> musicServices, SmfTrackInfo currTrack)
{
    //after purchasing and downloading is over, user plays the track
    //now post the current platying track to all service providers
    foreach(SmfMusicSearch provider, musicServices) {
		provider.postCurrentPlaying(currTrack);
	}
    //postCurrentPlaying is also a slot funtion, may be application can use connect
}

void MyApplication::displayLyrics(SmfTrackInfo currTrack)
{
	// Some common interface for finding implementations.
	SmfLyricsService lyricsService = Smf::GetServices("org.symbian.smf.music.lyrics","lyricsfly.com");

	QList<SmfLyrics> list = lyricsService.lyrics(currTrack);
    //now display the latest edited lyrics
    qSort(list.begin(),list.end(),caseCompareTimeMoreThan);
    m_view.setLyricsData(list.at(0));
}
