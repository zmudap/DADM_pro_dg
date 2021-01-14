QT -= gui
QT       += core gui printsupport
QT += charts

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Dfa.cpp \
        Waves.cpp \
        hrv1.cpp \
        hrv2.cpp \
        qcustomplot.cpp \
        Visualisation\visualisation.cpp \
        ecg_baseline.cpp \
        main.cpp \
        mainwindow.cpp \
        r_peaks.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Dfa.h \
    Waves.h \
    Waves/Waves.h \
    hrv1.h \
    hrv2.h \
    qcustomplot.h \
    Visualisation\visualisation.h \
    ecg_baseline.h \
    mainwindow.h \
    r_peaks.h

DISTFILES += \
    ECG_original_signal_10s.txt \
    img/ekg1.png \
    img/ekg2.png \
    signal_201.txt \
    signal_203.txt \
    signal_205.txt \
    signal_test_11.txt \
    vector.txt

FORMS += \
    Visualisation/visualisation.ui \
    mainwindow.ui

INCLUDEPATH += C:/gsl3/include/
INCLUDEPATH += C:/gsl3/lib
LIBS += -LC:/gsl3/lib -llibgsl -llibgslcblas -lm

RESOURCES += \
    resources.qrc
