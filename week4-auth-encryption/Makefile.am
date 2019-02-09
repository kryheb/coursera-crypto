AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = paddingoracle

paddingoracle_SOURCES= \
src/main.cpp \
src/paddingoracle.cpp \
src/utils/log/log.cpp \
src/utils/httpclient/simplehttpclient.cpp

paddingoracle_CXXFLAGS= \
-I src \
-DBOOST_LOG_DYN_LINK \
-std=c++17


paddingoracle_LDADD = \
$(PTHREAD_CFLAGS) \
-lcurl \
-lpthread \
-lboost_log \
-lboost_log_setup \
-lboost_system \
-lboost_thread \
-lboost_date_time \
-lstdc++fs
