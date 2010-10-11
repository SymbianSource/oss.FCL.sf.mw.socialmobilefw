#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H

#include <qlistwidget.h>
#include <smfcontact.h>
#include <smfpost.h>
#include <smfactions.h>
#include <smfplaylist.h>
#include "qmainwindow.h"

class CustomListWidget : public QListWidget
{
public:
    CustomListWidget();
    //QWidget* CreateListwidget(QStrng aIconPath,QString aFirstLine,QString aSecondLine);
    QWidget* CreateListwidget(SmfContactList* friendsList);
    QWidget* CreateListwidget(SmfPostList *postsList);
    QWidget* CreateListwidget(SmfActivityEntryList *activityList);
    QWidget* CreateListwidget(SmfPlaylistList *playList);
    void AddListItem(QString aIconPath,QString aFirstLine,QString aSecondLine);
    void AddListItem(QMainWindow *mainWindow,QStringList aIconPath, QStringList iconName);
    void AddListItem(QString aPlaylistname);
    ~CustomListWidget();

};


#endif // CUSTOMLISTWIDGET_H
