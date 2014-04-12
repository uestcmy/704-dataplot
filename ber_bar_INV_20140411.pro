QT += opengl

OTHER_FILES += \
    ber_bar_CB_20140411.pro.user \
    ber_bar_INV_20140411.pro.user \
    Makefile \
    qpsk_ber \
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
    moc_ber.cpp \
    moc_ber_bar.cpp \
    throughput.cpp

FORMS += \
    ber.ui \
    ber_bar.ui \
    qpsk.ui \
    throughput.ui
