Content:
==============================================================================
This is pre-beta version of Social Mobile Framework code. Folder organization is as follows - 
                Doxyfile - Doxygen configuration file 

                \doc - contains zipped doxygen genrated documentation

                \example - contains sample plugins, auth application and client test application
                                    \example\AuthApp
                                    \example\fbactivityfetcherplugin
                                    \example\fbcontactfetcherplugin
                                    \example\fbpostproviderplugin
                                    \example\FlickrAuthApp
                                    \example\flickrcontactfetcherplugin
                                    \example\flickrgalleryplugin
                                    \example\smfclientapp


                \smf - Contains smf source code

                \smf\smfcredentialmgr - this is credential manager source code 

                \smf\smfservermodule - this is SMF server 
                                \smf\smfservermodule\smfclient
                                \smf\smfservermodule\smfserver
                                \smf\smfservermodule\smfserver\datastoremgr
                                \smf\smfservermodule\smfserver\pluginmgr
                                \smf\smfservermodule\smfserver\server
                                \smf\smfservermodule\smfserver\smfplugins
                                \smf\smfservermodule\smfserver\transportmgr

                \smf\smfservermodule\util - this contains utility classes for plugins e.g. JSON
                                \smf\smfservermodule\util\qjson

                \smf\smfsettingsui - this contains SMF settins UI [incomplete]




Dependencies:
==============================================================================
SMF is built on Nokia_Symbian3_SDK_v0.8 and Qt 4.6.3. 
You would need to install qt-mobility-symbian-opensource-1.0.0. 
To enable building of SMF in public SDK, the unified key store (RSA Signing) related code is 
blocked with compiler switch. Use Symbian^3 PDK if you want to enable this feature.

Main Interfaces that can be tested end-to-end using this set-up are ContactsFetcher and 
SmfPost. Though framework code is available, plugins are yet to be available for other service 
interfaces for end-to-end testing.

Limitations:
==============================================================================
1. Support for Non-Symbian (Desktop) version of Qt is not supported at this moment.
2. Integration of Authentication Application with Credential Manager is not complete at the moment. 
    So sample plugins require some hacks to access application information for Facebook and Flickr. 
    Please see pre-build step 2 on how to achieve this.
3. SMFSettingsUI is not complete at this moment. This impacts network proxy settings. See pre-build
    step 1 on how to overcome this.

Pre-build steps:
==============================================================================
1. SMF uses QNetwork classes to setup proxy-settings. For testing on emulator (WINSCW), find the 
    hardcoded value under compiler switch "EMULATORTESTING" in file smftransportmanagerutil.cpp. 
    If your proxy requires authentication, provide "<username> space <password>" in plaintext in a file 
    "DoNotShare.txt" and place this file in "C:/Data/" - which is at "\epoc32\winscw\c\data" under your 
    SDK installation folder. You are good to go now.
2. Since sample plugins are not integrated with Auth App and Credential Mgr yet, currently it expects
the necessary credentials in plain text. Sample smfclientapp uses facebook plugins, place a file 
named "FaceBookKeys.txt" under "C:/Data/" - which is at "\epoc32\winscw\c\data". This file should
contain keys in the following order:  API key, API secret, session key, session secret.. all separated by \n
If you want to know how to generate these, please see http://wiki.developers.facebook.com/index.php/Authorization_and_Authentication_for_Desktop_Applications
We are integrating plugins with CredentialMgr, so these painful steps won't be required once it is up.


Building Steps :  - 
==============================================================================
1. Use smfrepo.pro present in top level to build. This single pro file builds SmfServer, SmfCredMgr, SmfClient,
Sample Plugins and Sample TestApp
2. To test on emulator (WINSCW) you can run smfclientapp.exe.
3. QJson requires high capability, so testing on device would require appropriate cerificatres for signing. 
see https://www.symbiansigned.com/app/page

Running Sample Application
==============================================================================
1. Run project from \example\smfclientapp. This should fetch names of the friends from facebook [for the user who
provided the keys in step 2 in pre-build steps]
2. The results might not show properly on UI due to problems in S^3 SDK. However textual result can be viewed
at the generated log file - \epoc32\winscw\c\data\SmfClientLogs.txt
3. The Smf Server generates a separate log file under same directiry - \epoc32\winscw\c\data\SmfLog.txt
If the 