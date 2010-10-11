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
* Siddhartha Chandra, Satish Kanteti Sasken Communication Technologies Ltd
* Description:
* class to maintian session & all credential keys.
*/

#ifndef FBSESSION_H
#define FBSESSION_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSettings>
#include <QDateTime>
#include "smfcredmgrclient.h"
#include "authAppConstants.h"

// FORWARD DECLARATIONS
class FBRequest;

const QString KFacebokkKeysFileName = "c://Data//FacebookKeys.txt";

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * An FBSession represents a single user's authenticated session for a Facebook application.
 *
 * To create a session, you must use the session key of your application (which can
 * be found on the Facebook developer website).  You may then use the login dialog to ask
 * the user to enter their email address and password.  If successful, you will get back a
 * session key which can be used to make requests to the Facebook API.
 *
 * Session keys are cached and stored on the disk of the device so that you do not need to ask
 * the user to login every time they launch the app.  To restore the last active session, call the
 * resume method after instantiating your session.
 */
class FBSession : public QObject
{
    Q_OBJECT

private:
    /**
      * the queue of requests
      */
    QList<FBRequest*>           iRequestQueue;

    /**
     * Your application's API key, as passed to the constructor.
     */
    QString iApiKey;

    /**
     * Your application's API secret, as passed to the constructor.
     */
    QString iApiSecret;

    /**
     * The URL to call to create a session key after login.
     *
     * This is an alternative to calling auth.getSession directly using the secret key.
     */
    QString iGetSessionProxy;

    /**
     * The current user's session key.
     */
    QString iSessionKey;

    /**
     * The current user's session secret.
     */
    QString iSessionSecret;

    /**
     * The expiration date of the session key.
     */
    QDateTime iExpirationDate;


    /**
      * the time at which the last request was performed, this is used to prevent
      * too many requests going to the server at once.
      */
    QDateTime   iLastRequestTime;
    int         iRequestBurstCount;


    /**
      * The settings object used to restore session from the disk.
      */
    QSettings   iSettings;
    /**
      * The Credentail Object used to store auth data
      */
    SmfCredMgrClient* m_Client; 
    /**
      * Variable to store 
      */ 
    QString iCMRegToken;

signals: /* the signals ommitted by FBSession */

    /**
      * Called when session logged in sucessfully
      * @param SessionKey is the fb assigned session key
      */
    void sessionDidLogin (QString SessionKey);

    /**
     * Called when a user closes the login dialog without logging in.
     */
    void sessionDidNotLogin ();

    /**
     * Called when a session is about to log out.
     * @param aUid is the fb assigned session id
     */
    void sessionWillLogout ();

    /**
     * Called when a session has logged out.
     */
    void sessionDidLogout ();

private slots:
    /**
      * handler function for the timer fired from startFlushTimer() function
      */
    void requestTimerReady();

public: /* class functions */

    /**
     * The globally shared session instance.
     */
    static FBSession* session();

    /**
     * Sets the globally shared session instance.
     *
     * This session is not retained, so you are still responsible for retaining it yourself.  The
     * first session that is created is automatically stored here.
     */
    static void setSession(FBSession* aSession);

    /**
     * Constructs a session and stores it as the globally shared session instance.
     *
     * @param aSessionProxy a url to that proxies auth.getSession
     */
    static FBSession* sessionForApplication ( const QString& aKey, const QString& aSecret, const QString& aSessionProxy);

public: /* instance functions */

    /**
     * Constructs a session for an application.
     *
     * @param secret the application secret (optional)
     * @param getSessionProxy a url to that proxies auth.getSession (optional)
     */
    FBSession( const QString& aKey, const QString& aSecret, const QString& aSessionProxy );

    /**
      * Destructor
      */
    ~FBSession();

    /**
     * Begins a session for a user with a given key and secret.
     */
    void beginSession ( const QString& aSessionKey, const QString& aSessionSecret, const QDateTime& aExpires );

    /**
     * Resumes a previous session whose uid, session key, and secret are cached on disk.
     */
     bool resume();

    /**
     * Cancels a login (no-op if the login is already complete).
     */
    void cancelLogin();

    /**
     * Ends the current session and deletes the uid, session key, and secret from disk.
     */
    void logout();

    /**
     * Sends a fully configured request to the server for execution.
     */
    void send (FBRequest* aRequest);

    /**
      * @return const QString& http:// URL to the facebook REST server
      */
    const QString& apiURL() const;

    /**
      * @return const QString& https:// URL to the secure facebook REST server
      */
    const QString& apiSecureURL() const;

    /**
     * Determines if the session is active and connected.
     * @return bool true if connected
     */
    bool isConnected() const;

    /**
      * @return const QString& the api secret
      */
    const QString& apiSecret() const { return iApiSecret; }

    /**
      * @return const QString& the GET? session proxy
      */
    const QString& getSessionProxy() const { return iGetSessionProxy; }

    /**
      * @return api key for this session
      */
    const QString& apiKey() const { return iApiKey; }

    /**
      * @return the session secret
      */
    const QString& sessionSecret() const { return iSessionSecret; }

    /**
      * @return the session key
      */
    const QString& sessionKey() const { return iSessionKey; }

private:

    /**
      * Saves the fb connect session information to disk
      */
    void save();
    /**
      * Forgets any fb connect session information saved to disk
      */
    void unsave();

    void startFlushTimer();

    /**
      * @param aRequest, the request to add to the session queue, owner ship is transferred to the session
      */
    void enqueueRequest(FBRequest* aRequest);

    /**
      * @param aRequest, the request to perform.
      * @param aEnqueue, if true add to queue if cant perform the request right now
      */
    bool performRequest(FBRequest* aRequest, bool aEnqueue);

    /**
      * flush the queue but performingRequest on the requests in the queue, if cant perform the request, waits a while using
      * startFlushTimer() and then tries to continue ..
      */
    void flushRequestQueue();
public:
    QString stroauth_nonce;
    QString stroauth_timestamp;
    QString stroauth_signature;
    QString stroauth_Token;
    QString stroauth_TokenSecret;
    //QString stroauth_Url;

};

#endif // FBSESSION_H
