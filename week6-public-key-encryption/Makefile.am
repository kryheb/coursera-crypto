AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = modulusfactor

modulusfactor_SOURCES= \
src/main.cpp \
src/utils/log/log.cpp

modulusfactor_CXXFLAGS= \
-I src \
-DBOOST_LOG_DYN_LINK \
-std=c++17


modulusfactor_LDADD = \
$(PTHREAD_CFLAGS) \
-lgmpxx \
-lgmp \
-lpthread \
-lboost_log \
-lboost_log_setup \
-lboost_system \
-lboost_thread \
-lboost_date_time \
-lstdc++fs
