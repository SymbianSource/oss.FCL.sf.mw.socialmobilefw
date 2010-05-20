/**
 * Copyright (c) 2010 Sasken Communication Technologies Ltd.
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of the "Eclipse Public License v1.0"
 * which accompanies  this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html"
 *
 * Initial Contributors:
 * Chandradeep Gandhi, Sasken Communication Technologies Ltd - Initial contribution
 *
 * Description:
 * Represents activities in terms similar to standard activity stream http://activitystrea.ms/
 */

#ifndef SMFACTIONS_H_
#define SMFACTIONS_H_

#include <QList>
#include <QImage>
#include <QDateTime>
#include <QStringList>
#include <QUrl>
#include <qdatastream.h>
#include <QSharedData>
#include <QMetaType>

#include "smfclientglobal.h"


/**
 * The actions in the activity based on Atom Activity Base Schema
 * see http://activitystrea.ms/schema/1.0/activity-schema-01.html
 */
enum SmfActivityVerb
	{
	SmfActivityMarkAsFavorite,
	SmfActivityStartFollowing,
	SmfActivityMarkLiked,
	SmfActivityMakeFriend,
	SmfActivityJoin,
	SmfActivityPlay,
	SmfActivityPost,
	SmfActivitySave,
	SmfActivityShare,
	SmfActivityTag,
	SmfActivityUpdate,
	};

/**
 * The type of the object on which the activity is performed - as per Atom Activity Base Schema
 * see http://activitystrea.ms/schema/1.0/activity-schema-01.html
 */
enum SmfActivityObjectType
	{
	SmfActivityObjTypeArticle,
	SmfActivityObjTypeAudio,
	SmfActivityObjTypeBookmark,
	SmfActivityObjTypeComment,
	SmfActivityObjTypeFile,
	SmfActivityObjTypeFolder,
	SmfActivityObjTypeGroup,
	SmfActivityObjTypeList,
	SmfActivityObjTypeNote,
	SmfActivityObjTypePerson,
	SmfActivityObjTypePhoto,
	SmfActivityObjTypePhotoAlbum,
	SmfActivityObjTypePlace,
	SmfActivityObjTypePlaylist,
	SmfActivityObjTypeProduct,
	SmfActivityObjTypeReview,
	SmfActivityObjTypeService,
	SmfActivityObjTypeStatus,
	SmfActivityObjTypeVideo,
	SmfActivityObjTypeMusic,
	SmfActivityObjTypeEvent,
	SmfActivityObjTypeAdvanced
	};

enum SmfActivityFor
	{
		SmfActivitySelf,
		SmfActivityFriend,
		SmfActivityOthers
	};

class SmfActivityObjectPrivate;
/**
 * @ingroup smf_common_group
 * The object class represents an object in an activity (e.g.a picture or a music track on which somebody commented)
 */
class SMFCLIENT_EXPORT SmfActivityObject 
	{
public:
	/**
	 * Constructor with default argument
	 */
	SmfActivityObject();

	/**
	 * Copy Constructor
	 */
	SmfActivityObject(const SmfActivityObject& aOther);
	
	/**
	 * Destructor
	 */
	~SmfActivityObject();

	/**
	 * retruns service specific Id of this object in the entry 
	 */
	QString id() const;
	
	/**
	 * returns thumbnail if availabel for this object in the entry
	 */
	QImage thumbnail() const;
	
	/**
	 * Captions for this object
	 */
	QString caption() const;
	
	/**
	 * retruns the type of this object
	 */
	SmfActivityObjectType type() const;
	
	/**
	 * returns data of the object - see type() for casting
	 */
	QVariant objData() const;
	
	/**
	 * url of this object, e.g. url of the image on which somebody commented
	 */
	QString link() const;
	
	/**
	 * Time of creation of this object
	 */
	QDateTime time() const;
	
	/**
	 * Description of the content of this object
	 */
	QString content() const;

	/**
	 * sets service specific id for this object
	 */
	void setId(const QString& aId);
	
	/**
	 * sets thumbnail for this object , not mandatory
	 */
	void setThumbnail(const QImage& aIcon);
	
	/**
	 * sets caption for this object
	 */
	void setCaption(const QString& aCap);
	
	/**
	 * sets type of this object - ObjData should be set next accordingly
	 */
	void setType(const SmfActivityObjectType& aObjType);
	
	/**
	 * sets object data e,g, SmfImage, SmfComment, SmfMusic, SmfEvent etc
	 * setType() with suitable type should have been called before  
	 */
	void setObjData( const QVariant& aData);
	/**
	 * sets the url for the object
	 */
	void setLink( const QString& aLink);
	/**
	 * sets the time of creation/last modification of the object
	 */
	void setTime( const QDateTime& aTime);
	
	/**
	 * sets the description of the content of this object
	 */
	void setContent( const QString& aText) ;
	
private:
	QSharedDataPointer<SmfActivityObjectPrivate> d;
	
	friend QDataStream& operator >> ( QDataStream &aOut, const SmfActivityObject& aData);
	friend QDataStream& operator << ( QDataStream &aIn, const SmfActivityObject& aData);	
	};
// Make the class SmfActivityObject known to QMetaType, so that as to register it.
Q_DECLARE_METATYPE(SmfActivityObject)





class SmfActivityEntryPrivate;
/**
 * @ingroup smf_common_group
 * The object class represents an entry in the list of activities 
 */
class SMFCLIENT_EXPORT SmfActivityEntry
	{

public:
	
	SmfActivityEntry();
	SmfActivityEntry(const SmfActivityEntry& aOther);
	~SmfActivityEntry();
	
	/**
	 * Returns service specific id of the of entry 
	 */
	QString id() const;
	/**
	 * returns the title of the entry - mostly be a string 
	 */
	SmfPost title() const;

	/**
	 * returns detail descriptiopn of this entry in the activity list. might be absent if title is sufficient
	 */
	SmfPost details() const;
	
	/**
	 * returns the author of the activity - the name and the uri field are most commonly used
	 * Other information fields might be empty
	 */
	SmfContact author() const;
	
	/**
	 * returns the verb of the activity ,e.g. Robin "marked" Joseph as a friend   
	 */
	SmfActivityVerb actionName() const;

	/**
	 * There can be multiple objects in a single activity entry, though this may be rare - only for few verbs.
	 * @return list of activity objects (mostly one object)
	 */
	QList<SmfActivityObject> activities() const;
	
	/**
	 * returns information about the target of the activity, for verbs that support a target.
	 * For example, a target is a photo album to which photos were added
	 */
	SmfActivityObject targetObj() const;

	/**
	 * sets service specific id of the of entry 
	 */
	bool setId( const QString& aId);
	
	/**
	 * sets the title of the entry - mostly be a string 
	 */
	bool setTitle(const SmfPost& aTitle);

	/**
	 * sets detail descriptiopn of this entry in the activity list. might be absent if title is sufficient
	 */
	bool setDetails(const SmfPost& aDetails);
	
	/**
	 * sets the author of the activity - the name and the uri field are most commonly used
	 * Other information fields might be empty
	 */
	bool setAuthor(const SmfContact& aContact);
	
	/**
	 * sets the verb of the activity ,e.g. Robin "marked" Joseph as a friend   
	 */
	void setActionName(SmfActivityVerb  aVerb);

	/**
	 * sets single or multiple objects in a single activity entry, though multiple may be rare - only for few verbs.
	 * @param list of activity objects (mostly one object)
	 */
	bool setActivities(QList<SmfActivityObject>& aList);
	
	/**
	 * returns information about the target of the activity, for verbs that support a target.
	 * For example, a target is a photo album to which photos were added
	 */
	bool setTargetObj(const SmfActivityObject& aTarget);
	
private:
	QSharedDataPointer<SmfActivityEntryPrivate> d;
	
	friend QDataStream& operator >> ( QDataStream &aOut, const SmfActivityEntry& aData);
	friend QDataStream& operator << ( QDataStream &aIn, const SmfActivityEntry& aData);	
	};
// Make the class SmfActivityEntry known to QMetaType, so that as to register it.
Q_DECLARE_METATYPE(SmfActivityEntry)

#endif /* SMFACTIONS_H_ */
