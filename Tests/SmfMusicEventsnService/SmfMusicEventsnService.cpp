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

#include "SmfMusicEventsnService.h"

SmfTestApp::SmfTestApp(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_contactFetcher = NULL;
	m_providerList = NULL;
}

SmfTestApp::~SmfTestApp()
{
	if(m_contactFetcher)
		delete m_contactFetcher;
	if(m_providerList)
		delete m_providerList;
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
//chinmaya
void SmfTestApp::lastFm()
	{
	qDebug()<<"Inside SmfTestApp::lastFm()";
		
	// Get the list of providers
	SmfClient client;
	QString intfName("org.symbian.smf.plugin.music.service");
	
	m_providerList = client.GetServices(intfName);
	qDebug()<<"client.GetServices returned a list with count = "<<m_providerList->count();
	
	// flag to check if required plugin is there
	bool pluginFound = false;
	foreach(SmfProvider provider, *m_providerList)
		{
		if("last.fm" == provider.serviceName())
			{
			qDebug()<<"Plugin for last.fm found";
			pluginFound = true;
			SmfMusicService *m_musicService = new SmfMusicService(&provider);
			SmfError err = m_musicService->userMusicInfo();			
			bool connected = QObject::connect(m_musicService,SIGNAL(userMusicInfoAvailable(SmfMusicProfile*,SmfError)),
					this,SLOT(userMusicInfoAvlbl(SmfMusicProfile*,SmfError)));
			qDebug() <<"Smfmusicservice::userMusicInfo" ;
			qDebug()<<"Signal-slot connected ? = "<<connected;
			}
		}
	
	if(!pluginFound)
		{
		qDebug()<<"Plugin for last.fm not found!!!";
		}
	
	qDebug()<<"Returning from SmfTestApp::last.fm()";
	}
void SmfTestApp::userMusicInfoAvlbl(SmfMusicProfile*,SmfError)
	{
	qDebug()<<"User music info";
	}





void SmfTestApp::FacebookFiltered()
	{
	qDebug()<<"Inside SmfTestApp::FacebookFiltered()";
		
	// Get the list of providers
	SmfClient client;
	QString intfName("org.symbian.smf.client.activity.fetcher");
	
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
			SmfActivityFetcher *p_smfActivityFetcher = new SmfActivityFetcher(&provider);
			QList<SmfActivityObjectType> Article;
			
			SmfActivityObjectType SmfActivityObjTypeComment;			
			Article.append(SmfActivityObjTypeComment);
			
			// specify some dummy value for Article
		  	SmfError err = p_smfActivityFetcher->filtered(Article);
			bool connected = QObject::connect(p_smfActivityFetcher,SIGNAL(resultsAvailable(SmfActivityEntryList*, SmfError, SmfResultPage)),
					this,SLOT(resultsAvailableSlot(SmfActivityEntryList*, SmfError, SmfResultPage)));
			qDebug() <<"SmfActivityFetcher::filter" ;
			qDebug()<<"Signal-slot connected ? = "<<connected;
			}
		}
	
	if(!pluginFound)
		{
		qDebug()<<"Plugin for Facebook not found!!!";
		}
	
	qDebug()<<"Returning from SmfTestApp::getFacebookFriends()";
	}

void SmfTestApp::FacebookActivities()
	{
	qDebug()<<"Inside SmfTestApp::FacebookFiltered()";
		
	// Get the list of providers
	SmfClient client;
	QString intfName("org.symbian.smf.client.activity.fetcher");
	
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
			SmfActivityFetcher *p_smfActivityFetcher = new SmfActivityFetcher(&provider);
			QContactName Name;
			//Name.setFirstName("Siddartha");
			//Name.setLastName("Chandra");
			
			SmfContact aFriend;
			// add some dummy values to aFriend methods
			QString str;
			str.append("Name");
			QVariant var = QVariant::fromValue<QContactName>(Name);

			aFriend.setValue(str,var);
			QContactGuid guid;

			// contact->value("Guid").value<QContactGuid>() ;

			QString userId = "558588290";

			guid.setGuid(userId); 

			QVariant contactId = QVariant::fromValue<QContactGuid>(guid);

			aFriend.setValue("Guid",contactId);
			int pageNum = 1,perPage=2;			  
			SmfError err = p_smfActivityFetcher->friendsActivities(aFriend,pageNum,perPage);
			bool connected = QObject::connect(p_smfActivityFetcher,SIGNAL(resultsAvailable(SmfActivityEntryList*, SmfError, SmfResultPage)),
					this,SLOT(resultsAvailableSlot(SmfActivityEntryList*, SmfError, SmfResultPage)));
			qDebug() <<"SmfActivityFetcher::filter" ;
			qDebug()<<"Signal-slot connected ? = "<<connected;
			}
		}
	
	if(!pluginFound)
		{
		qDebug()<<"Plugin for Facebook not found!!!";
		}
	
	qDebug()<<"Returning from SmfTestApp::getFacebookFriends()";
	}


void SmfTestApp::resultsAvailableSlot(SmfActivityEntryList *ptr, SmfError _t2, SmfResultPage _t3)
	{
	qDebug()<<"inside resultsAvailableSlot";	
	//qDebug()<<"data"<<SmfActivityEntryList->_t3;
	qDebug()<<ptr->at(0).id();
	foreach(SmfActivityEntry contact, (*ptr))
		{
		qDebug()<<"SmfActivityEntry id = "<<contact.id();
		/*qDebug()<<"Friends status msg desc = "<<contact.;
		qDebug()<<"Friends profile image URL = "<<contact.value("Avatar").value<QContactAvatar>().imageUrl();
		qDebug()<<"";*/
		}
	
	delete ptr;
	}
