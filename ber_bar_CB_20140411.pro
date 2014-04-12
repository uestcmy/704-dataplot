
QT += opengl
OTHER_FILES += \
    Makefile \
    qpsk_ber \
    qpsk_ber.pro \
    qpsk_ber.pro.user

HEADERS += \
    ber.h \
    ber_bar.h \
    qpsk.h \
    throughput.h \
    ui_ber.h \
    ui_ber_bar.h \
    ui_qpsk.h

SOURCES += \
    ber.cpp \
    ber_bar.cpp \
    chol_inv_4.cpp \
    main.cpp \
    throughput.cpp

FORMS += \
    ber.ui \
    ber_bar.ui \
    qpsk.ui \
    throughput.ui
