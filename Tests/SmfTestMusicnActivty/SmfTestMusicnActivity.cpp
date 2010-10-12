/****************************************************************************
**
** Trolltech hereby grants a license to use the Qt/Eclipse Integration
** plug-in (the software contained herein), in binary form, solely for the
** purpose of creating code to be used with Trolltech's Qt software.
**
** Qt Designer is licensed under the terms of the GNU General Public
** License versions 2.0 and 3.0 ("GPL License"). Trolltech offers users the
** right to use certain no GPL licensed software under the terms of its GPL
** Exception version 1.2 (http://trolltech.com/products/qt/gplexception).
**
** THIS SOFTWARE IS PROVIDED BY TROLLTECH AND ITS CONTRIBUTORS (IF ANY) "AS
** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
** PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
** OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** Since we now have the GPL exception I think that the "special exception
** is no longer needed. The license text proposed above (other than the
** special exception portion of it) is the BSD license and we have added
** the BSD license as a permissible license under the exception.
**
****************************************************************************/

#include <qdebug.h>
#include <smfclient.h>
#include <smfcontactfetcher.h>
#include <smfmusic.h>
#include <smfplaylist.h>

#include "SmfTestMusicnActivity.h"

SmfTestApp::SmfTestApp(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_contactFetcher = NULL;
	m_providerList = NULL;
	m_tracksofartist=NULL;
	m_tracksimilar=NULL;
	m_trackofAlbum=NULL;
	m_Playlist=NULL;
	m_Stores=NULL;
}

SmfTestApp::~SmfTestApp()
{
	if(m_contactFetcher)
		delete m_contactFetcher;
	if(m_providerList)
		delete m_providerList;
	if(m_tracksofartist)
		delete m_tracksofartist;
	if(m_tracksimilar)
		delete m_tracksimilar;
	if(m_trackofAlbum)
		delete m_trackofAlbum;
	if(m_Playlist)
		delete m_Playlist;
	if(m_Stores)
		delete m_Stores;
}


void SmfTestApp::getTracksOfArtists()
	{
	SmfClient client;
	qDebug()<<"Inside SmfTestApp::tracksofartist()";

			QString intfName("org.symbian.smf.client.music.search");
			
			SmfProviderList *providerList = client.GetServices(intfName);
			qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
			bool pluginFound = false;
		      foreach(SmfProvider provider, *providerList)
		         		{
		         		if("last.fm" == provider.serviceName())
		         			{
							pluginFound = true;
		         			qDebug()<<"Plugin for last.fm found";
		         			m_tracksofartist = new SmfMusicSearch(&provider);
		         			SmfTrackInfo TInfo;
		         			TInfo.setTitle("Aja Nachle");
		         			SmfArtists Artists; 
		 			        QStringList QList;
		         			QList.append("Adnan Sami");//A.R.Rehman");
		         			Artists.setNames(QList);
		         			SmfError err=m_tracksofartist->tracksOfArtist(Artists,1,10);//artist,1,1);
		         			qDebug()<<"Return Value of tracksOfArtist :"<<err;

		         			bool connected = connect(m_tracksofartist, SIGNAL(trackSearchAvailable(SmfTrackInfoList *, SmfError ,SmfResultPage)),
		         		     			this,SLOT(trackSearchAvailableSlot(SmfTrackInfoList*, SmfError, SmfResultPage)));//SmfTrackInfoList * ,SmfError)));
		         			//trackSearchAvailable
		        			qDebug()<<"Signal-slot connected ? = "<<connected;

		         			}
		         		}
		  	if(!pluginFound)
		  		{
		  		qDebug()<<"Plugin for Last.fm not found!!!";
		  		}
		  	
		  	qDebug()<<"Returning from SmfTestApp::getTracksOfArtists()";
			   	
	}


void SmfTestApp::getTracksSimilar()
	{
	SmfClient client;
	qDebug()<<"Inside SmfTestApp::getTracksSimilar()";

			QString intfName("org.symbian.smf.client.music.search");
			
			SmfProviderList *providerList = client.GetServices(intfName);
			qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
			bool pluginFound = false;
		      foreach(SmfProvider provider, *providerList)
		         		{
		         		if("last.fm" == provider.serviceName())
		         			{
							pluginFound = true;
		         			qDebug()<<"Plugin for last.fm found";
		         			m_tracksimilar = new SmfMusicSearch(&provider);
		         			SmfTrackInfo TInfo;
		         				         			
							TInfo.setTitle("Tera Chehra");
							//TInfo.setTitle("Bambhole");
						
							SmfArtists Artists; 
				
							
							QStringList QList;
						
							QList.append("Adnan Sami");
							Artists.setNames(QList);
							TInfo.setArtists(Artists);
								
							//SmfError err=m_tracksimilar->tracksSimilar(TInfo,1,1);
							SmfMusicFingerPrint signature;
							signature.setId("+1-q7D7-iHR9cQ");
							SmfError err=m_tracksimilar->trackInfo(signature,1,1);
		         			bool connected = connect(m_tracksimilar, SIGNAL(trackSearchAvailable(SmfTrackInfoList *, SmfError ,SmfResultPage)),
		         		     			this,SLOT(trackSearchAvailableSlot(SmfTrackInfoList*, SmfError, SmfResultPage)));//SmfTrackInfoList * ,SmfError)));
		         			//trackSearchAvailable
		        			qDebug()<<"Signal-slot connected ? = "<<connected;

		         			}
		         		}
		  	if(!pluginFound)
		  		{
		  		qDebug()<<"Plugin for Last.fm not found!!!";
		  		}
		  	
		  	qDebug()<<"Returning from SmfTestApp::getTracksSimilar()";
			   	
	}
//tracksOfAlbum

void SmfTestApp::getTracksOfAlbom()
	{
	// Tujha Chehra
	SmfClient client;
	qDebug()<<"Inside SmfTestApp::getTracksOfAlbom()";

			QString intfName("org.symbian.smf.client.music.search");
			
			SmfProviderList *providerList = client.GetServices(intfName);
			qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
			bool pluginFound = false;
		      foreach(SmfProvider provider, *providerList)
		         		{
		         		if("last.fm" == provider.serviceName())
		         			{
							pluginFound = true;
		         			qDebug()<<"Plugin for last.fm found";
		            			m_trackofAlbum = new SmfMusicSearch(&provider);
		            			SmfTrackInfo TInfo;
		            			TInfo.setTitle("Summer of 69");
		            			SmfAlbum album;
		            			SmfArtists art;
		            			QStringList strList;
		            			strList.append("Adnan Sami");
		            			art.setNames(strList);
		            			album.setName("Tujha Chehra");
		            			album.setArtists(art);
		        				SmfError err = m_trackofAlbum->tracksOfAlbum(album,1,2);
		            			qDebug()<<"Return Value of tracksOfAlbum :"<<err;
		            			bool connected = connect(m_trackofAlbum, SIGNAL(trackSearchAvailable(SmfTrackInfoList *, SmfError ,SmfResultPage)),
		         		     			this,SLOT(trackSearchAvailableSlot(SmfTrackInfoList*, SmfError, SmfResultPage)));//SmfTrackInfoList * ,SmfError)));
		         			//trackSearchAvailable
		        			qDebug()<<"Signal-slot connected ? = "<<connected;

		         			}
		         		}
		  	if(!pluginFound)
		  		{
		  		qDebug()<<"Plugin for Last.fm not found!!!";
		  		}
		  	
		  	qDebug()<<"Returning from SmfTestApp::getTracksOfAlbom()";
			   	
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////addToPlaylist//////////////////////////////////////////////////////////////////////////
/*
playlistID (Required) : The ID of the playlist - this is available in user.getPlaylists.
track (Required) : The track name to add to the playlist.
artist (Required) : The artist name that corresponds to the track to be added.
api_key (Required) : A Last.fm API key.
api_sig (Required) : A Last.fm method signature. See authentication for more information.
sk (Required) : A session key generated by authenticating a user via the authentication protocol. 
 */////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SmfTestApp::addToPlaylist()
	{
	

	SmfClient client;
	qDebug()<<"Inside SmfTestApp::addToPlaylist()";

			QString intfName("org.symbian.smf.plugin.music.playlist");

			SmfProviderList *providerList = client.GetServices(intfName);
			qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
			bool pluginFound = false;
		      foreach(SmfProvider provider, *providerList)
		         		{
			if("last.fm" == provider.serviceName())
				{
				pluginFound = true;
    			qDebug()<<"Plugin for last.fm found";
	    			m_Playlist = new SmfPlaylistService(&provider);
	    			SmfPlaylist pl_list;
	    			pl_list.setPlayListTitle("playlist_2");
	    			pl_list.setId("4b2es_playlist_2");
	    			SmfAlbum album;
	    			QList <SmfTrackInfo> list;
	    			SmfTrackInfo track;
					SmfArtists art;
					QStringList strList;
					strList.append("Adnan Sami");
					art.setNames(strList);
					album.setName("Teri Kasam");
					//album.setName("Tujha Chehra");
					//album.setId();
					album.setArtists(art);
					track.setArtists(art);
					track.setAlbum(album);
					track.setTitle("Kasam ");
					//track.setTitle("Hi Hawa Mand Wahe");
					list.append(track);
	    			SmfError err=m_Playlist->addToPlaylist(pl_list,&list);
	    			qDebug()<<"Return Value of PlayLists is :"<<err;
																//playlistsListAvailable(SmfPlaylistList * _t1, SmfError _t2, SmfResultPage _t3)
        			bool connected = connect(m_Playlist, SIGNAL(playlistsListAvailable(SmfPlaylistList *, SmfError ,SmfResultPage)),
     		     			this,SLOT(playlistsListAvailableSlot(SmfPlaylistList *, SmfError, SmfResultPage)));//SmfTrackInfoList * ,SmfError)));
   			
        			qDebug()<<"Signal-slot connected ? = "<<connected;

         			}
         		}
  	if(!pluginFound)
  		{
  		qDebug()<<"Plugin for Last.fm not found!!!";
  		}
  	
  	qDebug()<<"Returning from SmfTestApp::getPlaylist()";

	}


void SmfTestApp::getPlaylist()
	{
	SmfClient client;
	qDebug()<<"Inside SmfTestApp::getPlaylist()";

			QString intfName("org.symbian.smf.plugin.music.playlist");

			SmfProviderList *providerList = client.GetServices(intfName);
			qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
			bool pluginFound = false;
		      foreach(SmfProvider provider, *providerList)
		         		{
			if("last.fm" == provider.serviceName())
				{
				pluginFound = true;
    			qDebug()<<"Plugin for last.fm found";
	    			m_Playlist = new SmfPlaylistService(&provider);
	    		/*	SmfPlaylist pl_list;
	    			pl_list.setPlayListTitle("playlist_2");
	    			pl_list.setId("4b2es_playlist_2");
	    			SmfAlbum album;
	    			QList <SmfTrackInfo> *list;
					SmfTrackInfo track;
					SmfArtists art;
					QStringList strList;
					strList.append("Adnan Sami");
					art.setNames(strList);
					album.setName("Tujha Chehra");
					album.setArtists(art);
					track.setArtists(art);
					track.setAlbum(album);
					track.setTitle("Hi Hawa Mand Wahe");
					list->append(track);
				*/	//SmfError err=m_Stores->stores(track ,1,1);
	    			SmfError err=m_Playlist->playlists(1,5);
	    		//	SmfError err=m_Playlist->addToPlaylist(pl_list,list);
	    			//SmfError err=m_Playlist->postCurrentPlayingPlaylist(pl_list);
	    			qDebug()<<"Return Value of PlayLists is :"<<err;
																//playlistsListAvailable(SmfPlaylistList * _t1, SmfError _t2, SmfResultPage _t3)
        			bool connected = connect(m_Playlist, SIGNAL(playlistsListAvailable(SmfPlaylistList *, SmfError ,SmfResultPage)),
     		     			this,SLOT(playlistsListAvailableSlot(SmfPlaylistList *, SmfError, SmfResultPage)));//SmfTrackInfoList * ,SmfError)));
	    			
        			qDebug()<<"Signal-slot connected ? = "<<connected;

         			}
         		}
  	if(!pluginFound)
  		{
  		qDebug()<<"Plugin for Last.fm not found!!!";
  		}
  	
  	qDebug()<<"Returning from SmfTestApp::getPlaylist()";

	}

void SmfTestApp::postCurrentPlayingPlaylist()
	{
	SmfClient client;
	qDebug()<<"Inside SmfTestApp::postCurrentPlayingPlaylist()";

			QString intfName("org.symbian.smf.plugin.music.playlist");

			SmfProviderList *providerList = client.GetServices(intfName);
			qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
			bool pluginFound = false;
		      foreach(SmfProvider provider, *providerList)
		         		{
			if("last.fm" == provider.serviceName())
				{
				pluginFound = true;
    			qDebug()<<"Plugin for last.fm found";
	    			m_Playlist = new SmfPlaylistService(&provider);
	    			SmfPlaylist pl_list;
	    			pl_list.setPlayListTitle("playlist_2");
	    			pl_list.setId("4b2es_playlist_2");
	    		//	SmfAlbum album;
	    		//	QList <SmfTrackInfo> *list;
				//	SmfTrackInfo track;
				//	SmfArtists art;
				//	QStringList strList;
				//	strList.append("Adnan Sami");
				//	art.setNames(strList);
				//	album.setName("Tujha Chehra");
				//	album.setArtists(art);
				//	track.setArtists(art);
				//	track.setAlbum(album);
				//	track.setTitle("Hi Hawa Mand Wahe");
				//	list->append(track);
	    			//SmfError err=m_Playlist->addToPlaylist(pl_list,list);
	    			SmfError err=m_Playlist->postCurrentPlayingPlaylist(pl_list);
	    			qDebug()<<"Return Value of postCurrentPlayingPlaylist is :"<<err;
																//playlistsListAvailable(SmfPlaylistList * _t1, SmfError _t2, SmfResultPage _t3)
        			bool connected = connect(m_Playlist, SIGNAL(playlistsListAvailable(SmfPlaylistList *, SmfError ,SmfResultPage)),
     		     			this,SLOT(playlistsListAvailableSlot(SmfPlaylistList *, SmfError, SmfResultPage)));//SmfTrackInfoList * ,SmfError)));
	    			
        			qDebug()<<"Signal-slot connected ? = "<<connected;

         			}
         		}
  	if(!pluginFound)
  		{
  		qDebug()<<"Plugin for Last.fm not found!!!";
  		}
  	
  	qDebug()<<"Returning from SmfTestApp::postCurrentPlayingPlaylist()";

	}



void SmfTestApp::getStoresOfTrac()
	{
	// Tujha Chehra
	SmfClient client;
	qDebug()<<"Inside SmfTestApp::getStoresOfTrac()";

			QString intfName("org.symbian.smf.client.music.search");
			
			SmfProviderList *providerList = client.GetServices(intfName);
			qDebug()<<"client.GetServices returned a list with count = "<<providerList->count();
			bool pluginFound = false;
		      foreach(SmfProvider provider, *providerList)
		         		{
		         		if("last.fm" == provider.serviceName())
		         			{
							pluginFound = true;
		         			qDebug()<<"Plugin for last.fm found";
		            	
		         			/*	m_trackofAlbum = new SmfMusicSearch(&provider);
		            			SmfTrackInfo TInfo;
		            			TInfo.setTitle("Summer of 69");
		            			SmfAlbum album;
		            			SmfArtists art;
		            			QStringList strList;
		            			strList.append("Adnan Sami");
		            			art.setNames(strList);
		            			album.setName("Tujha Chehra");
		            			album.setArtists(art);
		        				SmfError err = m_trackofAlbum->tracksOfAlbum(album,1,2);
		            			qDebug()<<"Return Value of tracksOfAlbum :"<<err;
		            			
		            			*/
		         			
                  			m_Stores = new SmfMusicSearch(&provider);
                   			SmfTrackInfo TInfo;
                   			TInfo.setTitle("Kabhi Aisa Lagay");
                   			SmfComment com;
                   			com.setText("Excellent");
	            			SmfAlbum album;
                   			SmfTrackInfo track;
                   			SmfArtists art;
							QStringList strList;
							strList.append("Adnan Sami");
							art.setNames(strList);

							album.setName("Tujha Chehra");
							album.setArtists(art);
							track.setArtists(art);
							track.setAlbum(album);
							SmfError err=m_Stores->stores(track ,1,1);
                  			qDebug()<<"Return Value of Stores :"<<err;
																					//SmfProviderList * _t1, SmfError _t2, SmfResultPage _t3
                   			bool connected = connect(m_Stores, SIGNAL(storeSearchAvailable(SmfProviderList*,SmfError,SmfResultPage)),
                   			                     								this,SLOT(storelistsListAvailable(SmfProviderList* ,SmfError ,SmfResultPage)));
     	
		         			
		        			qDebug()<<"Signal-slot connected ? = "<<connected;

		         			}
		         		}
		  	if(!pluginFound)
		  		{
		  		qDebug()<<"Plugin for Last.fm not found!!!";
		  		}
		  	
		  	qDebug()<<"Returning from SmfTestApp::getStoresOfTrac()";
			   	
	}


void SmfTestApp::storelistsListAvailable(SmfProviderList* list ,SmfError error,SmfResultPage resultPage)
	{
	
	Q_UNUSED(resultPage)
		qDebug()<<"Inside SmfTestApp::storelistsListAvailable()";
				
		if(error)
			{
			SmfClient client;
			QString errStr = client.errorString(error);
			qDebug()<<"Error found, code = "<<error;
			qDebug()<<"Error string is = "<<errStr;

			return;
			}
			
		//display friends information
		
		qDebug()<<"Number of friends retrieved = "<<list->count();
		if(0 == list->count())
			{
			qDebug()<<"No of tracks available!!!";
			return;
			}
			
		foreach(SmfProvider tracs, *list)
			{
			qDebug()<<"Authentication AppName = "<<tracs.authenticationAppName();//playListTitle();
			qDebug()<<"App URL = "<<tracs.applicationUrl();//playListTitle();
			qDebug()<<"Authentication AppName = "<<tracs.serviceName();//playListTitle();
			
			}
		
		delete list;
	
	
	}

void SmfTestApp::playlistsListAvailableSlot(SmfPlaylistList* playlist ,SmfError error,SmfResultPage page)
{
	Q_UNUSED(page)

for(int i = 0;i<playlist->count();i++)

{


//qDebug()<<"Data Author"<<playlist.at(i).author();
qDebug()<<"Data Author"<<playlist->at(i).author();


qDebug()<<"Data Creation Date"<<playlist->at(i).creationDate();

//qDebug()<<"Data Comments"<<playlist.at(i).comments.at(0);

qDebug()<<"Data Id"<<playlist->at(i).id();

qDebug()<<"Data playListTitle"<<playlist->at(i).playListTitle();

qDebug()<<"Data location"<<playlist->at(i).location();


}
		
}





void SmfTestApp::trackSearchAvailableSlot(SmfTrackInfoList* list, SmfError error, SmfResultPage resultPage)//SmfTrackInfoList *s1 ,SmfError err)
	{
	qDebug()<<"tracks of artist";	
	
	Q_UNUSED(resultPage)
		qDebug()<<"Inside SmfTestApp::trackSearchAvailableSlot()";
				
		if(error)
			{
			SmfClient client;
			QString errStr = client.errorString(error);
			qDebug()<<"Error found, code = "<<error;
			qDebug()<<"Error string is = "<<errStr;

			return;
			}
			
		//display friends information
		
		qDebug()<<"Number of friends retrieved = "<<list->count();
		if(0 == list->count())
			{
			qDebug()<<"No of tracks available!!!";
			return;
			}
			
		foreach(SmfTrackInfo tracs, *list)
			{
			qDebug()<<"Track Title = "<<tracs.title();//playListTitle();
			}
		
		delete list;
	}

		         		
void SmfTestApp::getFacebookFriends()
	{
	qDebug()<<"Inside SmfTestApp::getFacebookFriends()";
	
	// Get the list of providers
	SmfClient client;
	QString intfName("org.symbian.smf.plugin.contact.fetcher");
	
	m_providerList = client.GetServices(intfName);
	qDebug()<<"client.GetServices returned a list with count = "<<m_providerList->count();
	
	// flag to check if required plugin is there
	bool pluginFound = false;
	foreach(SmfProvider provider, *m_providerList)
		{
		if("Facebook" == provider.serviceName())
			{
			qDebug()<<"Plugin for Facebook found";
			pluginFound = true;
			m_contactFetcher = new SmfContactFetcher(&provider);
			SmfError err = m_contactFetcher->friends(1, 10);
			qDebug()<<"Ret value of friends() = "<<err;
			
			bool connected = connect(m_contactFetcher, SIGNAL(friendsListAvailable(SmfContactList*, SmfError , SmfResultPage)),
					this, SLOT(friendsListAvailable(SmfContactList*, SmfError , SmfResultPage)));
			qDebug()<<"Signal-slot connected ? = "<<connected;
			}
		}
	
	if(!pluginFound)
		{
		qDebug()<<"Plugin for Facebook not found!!!";
		}
	
	qDebug()<<"Returning from SmfTestApp::getFacebookFriends()";
	}


void SmfTestApp::friendsListAvailable ( SmfContactList* list, SmfError error, SmfResultPage resultPage )
	{
	Q_UNUSED(resultPage)
	qDebug()<<"Inside SmfTestApp::friendsListAvailable()";
			
	if(error)
		{
		SmfClient client;
		QString errStr = client.errorString(error);
		qDebug()<<"Error found, code = "<<error;
		qDebug()<<"Error string is = "<<errStr;

		return;
		}
		
	//display friends information
	
	qDebug()<<"Number of friends retrieved = "<<list->count();
	if(0 == list->count())
		{
		qDebug()<<"No friends available!!!";
		return;
		}
		
	foreach(SmfContact contact, *list)
		{
		qDebug()<<"Friends name = "<<contact.value("Name").value<QContactName>().firstName();
		qDebug()<<"Friends status msg desc = "<<contact.value("Presence").value<QContactPresence>().customMessage();
		qDebug()<<"Friends profile image URL = "<<contact.value("Avatar").value<QContactAvatar>().imageUrl();
		qDebug()<<"";
		}
	
	delete list;
	}
