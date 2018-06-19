QT += gui widgets

CONFIG += c++11 #console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    ashurawind.cpp \
    email.cpp \
    html.cpp

#INCLUDEPATH += f:/Workspace/workspaceQt/milestone32 # Own lib
#LIBS += -Lf:/Workspace/workspaceQt/milestone32/build-milestone32-Desktop-Release/release -lmilestone32 # Own lib

INCLUDEPATH += f:/Workspace/workspaceQt/mimetic0.9.8/include
LIBS += -L"f:/Workspace/workspaceQt/mimetic0.9.8/lib" -lmimetic -lmimeticcodec -lmimeticos -lmimeticrfc822

INCLUDEPATH += "f:/Workspace/workspaceQt/myhtml-4.0.2/include"
LIBS += -L"f:/Workspace/workspaceQt/myhtml-4.0.2/lib" -lmyhtml

FORMS += \
    ashurawind.ui

HEADERS += \
    ashurawind.h \
    email.h \
    html.h

#QMAKE_LFLAGS +=  -static -static-libgcc -static-libstdc++
#LIBS += -lboost_chrono -lboost_regex -lboost_system -lboost_filesystem -lboost_random -lboost_thread -lssl -lcrypto -lgdi32 -lpsapi -lz -lws2_32

