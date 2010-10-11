/*
 * keys.h
 *
 *  Created on: Jul 16, 2010
 *      Author: satishkk
 */

#ifndef KEYS_H_
#define KEYS_H_

#define EMULATORTESTING
#include <apgtask.h>
#include <apgcli.h>

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif
static const QString kApiKey = "0TkpBXcncsq8rpDV3UGBokMmqYe0EaaBkazqDIbROUZuONZGQmjKrYk6F1cW6lqF";//"UvEPGC6mY4CzPLOv1aMAoTNzti8RyNbSGBoulTt8Lg3NdBWmaIfKizg5tEOVJEkQ";
static const QString kConsumerKey = "0TkpBXcncsq8rpDV3UGBokMmqYe0EaaBkazqDIbROUZuONZGQmjKrYk6F1cW6lqF";//"UvEPGC6mY4CzPLOv1aMAoTNzti8RyNbSGBoulTt8Lg3NdBWmaIfKizg5tEOVJEkQ";
static const QString kConsumerSecret = "7edeiblp4DKjew78jd8STv86v6sdL9QH0dYJBphAHjZDwtxDy3RL08mC6fd7baYU";//"DVzvei0jABEvrbUYiGBhpfynSxNQe1IcJ_YYrwG_gZptBTwNdVhSJJlMuqJbNDdr";//"1234567890";
static const QString kApiUrl = "https://api.linkedin.com";
static const QString kRequestTokenUrl = "https://api.linkedin.com/uas/oauth/requestToken";
static const QString kAccessTokenUrl = "https://api.linkedin.com/uas/oauth/accessToken";
static const QString kAuthorizeUrl = "https://api.linkedin.com/uas/oauth/authorize";
//static const QString kCallBackUrl = "oauth_callback=\"oob\"";
//static const QString kAccessToken = "173744194-hlcnfI3PNlFkj1yBzQTdJSbMzsmTVkduy5us41Zb";
//static const QString kAccessTokenSecret = "uGCTr6F5R069IKnzyNmw4UucRvFcsLuv2e01dnNelU";
//_LIT(kKey, "MCD8BKwGdgPHvAuvgvz4EQpqDAtx89grbuNMRd7Eh98&");
//_LIT(kBaseUri,"http://www.facebook.com/connect/login_success.html");
static const QString kBase = "https://www.linkedin.com/uas/oauth/authorize/submit";//"https://sites.google.com/site/smfsasken";//http://www.facebook.com/connect/login_success.html";
#endif /* KEYS_H_ */
