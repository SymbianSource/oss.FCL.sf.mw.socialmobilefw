/*! \file
    \brief File containing class description for DataStoreManager class.

    Copyright (c) 2010 Sasken Communication Technologies Ltd.
    All rights reserved.
    This component and the accompanying materials are made available
    under the terms of the "{License}"
    which accompanies  this distribution, and is available
    at the URL "{LicenseUrl}".

    \author Jaspinder Singh, Sasken Communication Technologies Ltd - Initial contribution

    \version 0.1

*/

#include <dsm.h>
#include <QDebug>

// Static data initialization
DataStoreManager* DataStoreManager::m_dsm_instance = NULL;
const QString DataStoreManager::db_name = "dsm.db";
DataStoreManagerState DataStoreManager::state = CLOSED;

//! \fn getDataStoreManager()
/*!
    \brief  To get a handle on the datastore instantiation.
    \brief  This function returns a pointer to the existing DataStoreManager instantiation(object).
            If no object exists, a new one is created.

    \return Pointer to the current instantiation of DataStoreManager.
*/
DataStoreManager* DataStoreManager::getDataStoreManager(){
    if(m_dsm_instance == NULL){
        m_dsm_instance = new DataStoreManager(DataStoreManager::db_name);
        if(!(m_dsm_instance->InitializeDataBase())){
            return NULL;
        }

    }

    return  m_dsm_instance;
}

//! \fn ~DataStoreManager()
/*!
    \brief  Destructor for DataStoreManager class.

    \note   Should this be public?
*/
DataStoreManager::~DataStoreManager(){
    db.close();
    state = CLOSED;
    delete m_dsm_instance;
}

//! \fn getState()
/*!
    \brief  Get the current state of the DataStoreManager object.
    \return The state of the DSM object.
*/
DataStoreManagerState DataStoreManager::getState() const{
    return state;
}

//! \fn getError()
/*!
    \brief  Get the last error message from the DataStoreManager object.
    \return The last error message string of the DSM object.
*/
QString DataStoreManager::getError() const{
    return m_last_msg;
}

/* Refactor this. Think Signals. */
QList <SMFSocialProfile> DataStoreManager::getAllRelated(const SMFUserProfile& user_profile) {
    QList <SMFSocialProfile> related_profiles;
    int usr_id = user_profile.userID();

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return related_profiles;
            /* Do something to signal an error. Just returning an empty list is NOT ok */
        }
    }

    QSqlQuery qry;
    qry.prepare("SELECT social_profiles.social_profile_id , social_profiles.user_id , social_profiles.sns_id , "
                " social_profiles.profile_url , social_profiles.screen_alias"
                " FROM social_profiles JOIN user_profiles WHERE user_profiles.user_id = social_profiles.user_id"
                " AND social_profiles.user_id = :user_id");
    qry.bindValue(":user_id", usr_id);

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        /* Do something to signal an error. Just returning an empty list is NOT ok */
        return related_profiles;
    }

    while(qry.next())
    {
        SMFSocialProfile _profile(qry.value(2).toInt()/* sns ID*/,
                                  qry.value(1).toInt() /* user ID */,
                                  qry.value(3).toString()/* url */,
                                  qry.value(4).toString()/* alias */);
        related_profiles << _profile;

    }
/*
    for( int i = 0; i < related_profiles.size(); i++){
        qDebug() << related_profiles.at(i).associatedSnsID() << ", " << related_profiles.at(i).associatedUserID()
                << related_profiles.at(i).profileURL() << related_profiles.at(i).screenAlias() ;
    }
*/
    qry.finish();
    db.close();

    return related_profiles;

}


SMFSocialProfile DataStoreManager::getRelatedByService(const SMFUserProfile& user_profile, const SMFSocialNetworkingSite& sns){

    SMFSocialProfile _profile;
    int usr_id = user_profile.userID();
    int sns_id = sns.snsID();

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return _profile;
            /* Do something to signal an error. Just returning an empty list is NOT ok */
        }
    }

    QSqlQuery qry;
    qry.prepare("SELECT social_profiles.social_profile_id , social_profiles.user_id , social_profiles.sns_id , "
                " social_profiles.profile_url , social_profiles.screen_alias"
                " FROM social_profiles JOIN user_profiles WHERE user_profiles.user_id = social_profiles.user_id"
                " AND social_profiles.user_id = :user_id AND social_profiles.sns_id = :sns_id");
    qry.bindValue(":user_id", usr_id);
    qry.bindValue(":sns_id", sns_id);

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        /* Do something to signal an error. Just returning an empty list is NOT ok */
        return _profile;
    }

    if(qry.next()) {
        _profile.setAssociatedSnsID(qry.value(2).toInt());
        _profile.setAssociatedUserID(qry.value(1).toInt());
        _profile.setProfileURL(qry.value(3).toString());
        _profile.setScreenAlias(qry.value(4).toString());
    }
/*
    qDebug() << _profile.associatedSnsID() << ", " << _profile.associatedUserID() << ", "
            << _profile.profileURL() << ", "<< _profile.screenAlias() ;
*/
    qry.finish();
    db.close();

    return _profile;
}

/* Cannot return something like this. Make this async */
SMFUserProfile DataStoreManager::getUserProfile(const QString& name, const QString& contact_id){    
    SMFUserProfile _profile;

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return _profile;
            /* Do something to signal an error. Just returning an empty list is NOT ok */
        }
    }

    QSqlQuery qry;
    qry.prepare("SELECT user_id , name , contact_id FROM user_profiles WHERE name=:name  AND contact_id=:contact_id");
    qry.bindValue(":name", name);
    qry.bindValue(":contact_id", contact_id);

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        /* Do something to signal an error. Just returning an empty list is NOT ok */
        return _profile;
    }

    if(qry.next()){
        _profile.setName(qry.value(1).toString());
        _profile.setContactID(qry.value(2).toString());
        _profile.setUserID(qry.value(0).toInt());
    }

    qry.finish();
    db.close();

    return _profile;
}

/*! Fetches the Social Networking site identified by \a name */
SMFSocialNetworkingSite DataStoreManager::getSNSEntry(const QString& name){
    SMFSocialNetworkingSite sns;

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return sns;
            /* Do something to signal an error. Just returning an empty list is NOT ok */
        }
    }

    QSqlQuery qry;
    qry.prepare("SELECT sns_id, sns_name, sns_url FROM sns_base WHERE sns_name=:name");
    qry.bindValue(":name", name);

    if(!qry.exec()){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return sns;
    }

    if(qry.next()){
        sns.setSnsID(qry.value(0).toInt());
        sns.setSnsName(qry.value(1).toString());
        sns.setSnsURL(qry.value(2).toString());
    }

    qry.finish();
    db.close();

    return sns;
}

/* Actually all these 'save' functions can be written as a template */
void DataStoreManager::saveUserProfile(const SMFUserProfile& user_profile){

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
        }
    }

    QSqlQuery qry;
    qry.prepare("UPDATE user_profiles"
                " SET name=:name , contact_id=:contact_id"
                " WHERE user_id=:user_id");
    qry.bindValue(":name", user_profile.name());
    qry.bindValue(":contact_id", user_profile.contactID());
    qry.bindValue(":user_id", user_profile.userID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
    }

    qry.finish();
    db.close();
}

void DataStoreManager::saveSocialProfile(const SMFSocialProfile& social_profile){

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
        }
    }

    QSqlQuery qry;
    qry.prepare("UPDATE social_profiles"
                " SET user_id=:user_id, sns_id=:sns_id, profile_url=:profile_url, screen_alias=:screen_alias"
                " WHERE social_profile_id=:social_profile_id");
    qry.bindValue(":user_id", social_profile.associatedUserID());
    qry.bindValue(":sns_id", social_profile.associatedSnsID());
    qry.bindValue(":profile_url", social_profile.profileURL());
    qry.bindValue(":screen_alias", social_profile.screenAlias());
    qry.bindValue(":social_profile_id", social_profile.profileID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
    }

    qry.finish();
    db.close();
}

void DataStoreManager::saveSNSEntry(const SMFSocialNetworkingSite& sns){

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
        }
    }

    QSqlQuery qry;
    qry.prepare("UPDATE sns_base"
                " SET sns_name=:sns_name, sns_url=:sns_url"
                " WHERE sns_id=:sns_id");
    qry.bindValue(":sns_name", sns.snsName());
    qry.bindValue(":sns_url", sns.snsURL());
    qry.bindValue(":sns_id", sns.snsID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
    }

    qry.finish();
    db.close();

}

void DataStoreManager::modifyRelation(SMFSocialProfile& sns, SMFUserProfile& new_user_profile){
    sns.setAssociatedUserID(new_user_profile.userID());
}


int DataStoreManager::addUserProfile(SMFUserProfile& user_profile){
    int user_id = user_profile.userID();

    /* Check Required to identify multiple Entries */

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return user_id;
        }
    }

    QSqlQuery qry;
    qry.prepare("INSERT INTO user_profiles (name, contact_id) VALUES (:name, :contact_id)");
    qry.bindValue(":name", user_profile.name());
    qry.bindValue(":contact_id", user_profile.contactID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return user_id;
    }

    qry.prepare("SELECT user_id FROM user_profiles WHERE name=:name AND contact_id=:contact_id");
    qry.bindValue(":name", user_profile.name());
    qry.bindValue(":contact_id", user_profile.contactID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return user_id;
    }

    if(qry.next()){
        user_id = qry.value(0).toInt();
        user_profile.setUserID(user_id);
    }

    qry.finish();
    db.close();

    return user_id;


}

int DataStoreManager::deleteUserProfile( SMFUserProfile& user_profile){

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return -1;
        }
    }

    QSqlQuery qry;
    qry.prepare("DELETE FROM user_profiles WHERE user_id=:user_id");
    qry.bindValue(":user_id", user_profile.userID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return -1;
    }

    qry.finish();
    db.close();

    return 0;
}

int DataStoreManager::addSocialProfile(SMFSocialProfile& social_profile){
    int social_prof_id = social_profile.profileID();

    /* Check Required to identify multiple entries */

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return social_prof_id;
        }
    }

    /*  There might be a request to add a social profile with either user and/or sns profile(s)
        not stored in the database. What to do in that case? Automatically store the other profiles too?
    */
    QSqlQuery qry;
    qry.prepare("INSERT INTO social_profiles (user_id, sns_id, profile_url, screen_alias) "
                "VALUES (:user_id, :sns_id, :profile_url, :screen_alias)");
    qry.bindValue(":user_id", social_profile.associatedUserID());
    qry.bindValue(":sns_id", social_profile.associatedSnsID());
    qry.bindValue(":profile_url", social_profile.profileURL());
    qry.bindValue(":screen_alias", social_profile.screenAlias());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return social_prof_id;
    }

    qry.prepare("SELECT social_profile_id FROM social_profiles  "
                "WHERE user_id=:user_id AND sns_id=:sns_id AND profile_url=:profile_url AND screen_alias=:screen_alias");
    qry.bindValue(":user_id", social_profile.associatedUserID());
    qry.bindValue(":sns_id", social_profile.associatedSnsID());
    qry.bindValue(":profile_url", social_profile.profileURL());
    qry.bindValue(":screen_alias", social_profile.screenAlias());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return social_prof_id;
    }

    if(qry.next()){
        social_prof_id = qry.value(0).toInt();
        social_profile.setProfileID(social_prof_id);
    }

    qry.finish();
    db.close();

    return social_prof_id;
}

int DataStoreManager::deleteSocialProfile(SMFSocialProfile& social_profile){

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return -1;
        }
    }

    QSqlQuery qry;
    qry.prepare("DELETE FROM social_profiles WHERE social_profile_id=:social_profile_id");
    qry.bindValue(":social_profile_id", social_profile.profileID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return -1;
    }

    qry.finish();
    db.close();

    return 0;
}

int DataStoreManager::addSNSEntry( SMFSocialNetworkingSite& sns){
    int sns_id = sns.snsID();

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return sns_id;
        }
    }

    QSqlQuery qry;
    qry.prepare("INSERT INTO sns_base (sns_name, sns_url) VALUES (:sns_name, :sns_url)");
    qry.bindValue(":sns_name", sns.snsName());
    qry.bindValue(":sns_url", sns.snsURL());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return sns_id;
    }

    qry.prepare("SELECT sns_id FROM sns_base WHERE sns_name=:sns_name AND sns_url=:sns_url");
    qry.bindValue(":sns_name", sns.snsName());
    qry.bindValue(":sns_url", sns.snsURL());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return sns_id;
    }

    if(qry.next()){
        sns_id = qry.value(0).toInt();
        sns.setSnsID(sns_id);
    }

    qry.finish();
    db.close();

    return sns_id;
}

int DataStoreManager::deleteSNSEntry(SMFSocialNetworkingSite& sns){

    if (!(db.isOpen())){
        if(!(db.open())){
            state = ERROR;
            m_last_msg = db.lastError().text();
            return -1;
        }
    }

    QSqlQuery qry;
    qry.prepare("DELETE FROM sns_base WHERE sns_id=:sns_id");
    qry.bindValue(":sns_id", sns.snsID());

    if(!(qry.exec())){
        m_last_msg = qry.lastError().text();
        qry.finish();
        return -1;
    }

    qry.finish();
    db.close();

    return 0;
}

int DataStoreManager::createRelation(const SMFUserProfile& user_profile, SMFSocialProfile& social_profile){
    social_profile.setAssociatedUserID(user_profile.userID());
    /* Save the socialProfile */
    //saveSocialProfile(social_profile);
    return 0;
}

int DataStoreManager::deleteRelation(const SMFUserProfile&,  SMFSocialProfile& social_profile){
    social_profile.setAssociatedUserID(-1);
    /* Save the profile */
    //saveSocialProfile(social_profile);
    return 0;
}


DataStoreManager::DataStoreManager(const QString& db_name, QObject* parent):QObject(parent){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_name);
}

bool DataStoreManager::InitializeDataBase(){

    if(!db.open()){
        state = ERROR;
        m_last_msg = db.lastError().text();
        return FALSE;
    }
    else
        state = READY;

    QSqlQuery create_tables[3];

    create_tables[0].prepare("CREATE TABLE IF NOT EXISTS user_profiles (user_id INTEGER PRIMARY KEY AUTOINCREMENT , name TEXT, contact_id TEXT)");
    create_tables[1].prepare("CREATE TABLE IF NOT EXISTS sns_base (sns_id INTEGER PRIMARY KEY AUTOINCREMENT , sns_name TEXT, sns_url TEXT)");
    create_tables[2].prepare("CREATE TABLE IF NOT EXISTS social_profiles (social_profile_id INTEGER PRIMARY KEY AUTOINCREMENT , user_id INTEGER, sns_id INTEGER, profile_url TEXT, screen_alias TEXT)");



    for(int i = 0; i < 3; i++){
        if(!create_tables[i].exec()){
            state = ERROR;
            m_last_msg = create_tables[i].lastError().text();
            return FALSE;
        }
        create_tables[i].finish();
    }

    db.close();
    state = CLOSED;
    return TRUE;
}
