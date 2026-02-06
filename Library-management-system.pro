QT       += core gui
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbooks.cpp \
    addstudents.cpp \
    adminmenu.cpp \
    database.cpp \
    editstudentinformation.cpp \
    issuebooks.cpp \
    issuehistory.cpp \
    loginwindow.cpp \
    logoutdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    returnbooks.cpp \
    signupwindow.cpp \
    studentmenu.cpp \
    viewbooks.cpp \
    viewbooksstudent.cpp \
    viewstudentinformation.cpp

HEADERS += \
    addbooks.h \
    addstudents.h \
    adminmenu.h \
    database.h \
    editstudentinformation.h \
    issuebooks.h \
    issuehistory.h \
    loginwindow.h \
    logoutdialog.h \
    mainwindow.h \
    returnbooks.h \
    signupwindow.h \
    studentmenu.h \
    viewbooks.h \
    viewbooksstudent.h \
    viewstudentinformation.h

FORMS += \
    addbooks.ui \
    addstudents.ui \
    adminmenu.ui \
    editstudentinformation.ui \
    issuebooks.ui \
    issuehistory.ui \
    loginwindow.ui \
    mainwindow.ui \
    returnbooks.ui \
    signupwindow.ui \
    studentmenu.ui \
    viewbooks.ui \
    viewbooksstudent.ui \
    viewstudentinformation.ui

TRANSLATIONS += \
    Library-management-system_uk_UA.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    build/Desktop_Qt_6_9_3_MinGW_64_bit-Debug/debug/library.db \
    icons/left-arrow.png \
    icons/user.png \
    library.db

RESOURCES += \
    resources.qrc
