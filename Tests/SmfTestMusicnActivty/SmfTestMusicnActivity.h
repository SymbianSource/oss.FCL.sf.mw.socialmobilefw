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

#ifndef SMFTESTMUSICNACTIVITY_H
#define SMFTESTMUSICNACTIVITY_H

#include <QtGui/QWidget>
#include <smfprovider.h>
#include <smfcontact.h>
#include <smfglobal.h>
#include <smfmusic.h>
#include <smfplaylist.h>
#include <smfmusicfingerprint.h>

#include "ui_SmfTestMusicnActivty.h"

class SmfContactFetcher;
class SmfMusicService;
//class SmfPlaylistServicePrivate;
class SmfTestApp : public QWidget
{
    Q_OBJECT

public:
	SmfTestApp(QWidget *parent = 0);
    ~SmfTestApp();
    void getFacebookFriends();
    void getTracksOfArtists();
    void getTracksSimilar();
    void getTracksOfAlbom();
    void getPlaylist();
    void getStoresOfTrac();
    void addToPlaylist();
    void postCurrentPlayingPlaylist();
            
private slots:
		
	void friendsListAvailable ( SmfContactList* list, SmfError error, SmfResultPage resultPage );
	void trackSearchAvailableSlot(SmfTrackInfoList* list, SmfError error, SmfResultPage resultPage);//SmfTrackInfoList *s1 ,SmfError err);
    void playlistsListAvailableSlot(SmfPlaylistList* playlist ,SmfError error,SmfResultPage page);
    void storelistsListAvailable(SmfProviderList* ,SmfError ,SmfResultPage);
private:
    Ui::SmfTestApp ui;
    SmfContactFetcher *m_contactFetcher;
    SmfProviderList *m_providerList;
    SmfMusicSearch *m_tracksofartist;
    SmfMusicSearch	*m_tracksimilar;
    SmfMusicSearch	*m_trackofAlbum;
    SmfPlaylistService *m_Playlist;
    SmfMusicSearch	*m_Stores;
};

#endif // SMFTESTMUSICNACTIVITY_H

