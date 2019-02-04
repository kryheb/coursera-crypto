AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = msgintegrity

msgintegrity_SOURCES= \
src/main.cpp \
src/curldownloader.cpp \
src/msgintegrity.cpp \
src/utils/log/log.cpp

msgintegrity_CXXFLAGS= \
-I src \
-DBOOST_LOG_DYN_LINK \
-std=c++17


msgintegrity_LDADD = \
$(PTHREAD_CFLAGS) \
-lcurl \
-lpthread \
-lboost_log \
-lboost_log_setup \
-lboost_system \
-lboost_thread \
-lboost_date_time \
-lstdc++fs \
-lcrypto++
