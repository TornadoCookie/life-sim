bin_PROGRAMS = lifesim
noinst_LIBRARIES = liblife.a 

LDADD = $(noinst_LIBRARIES)
AM_CPPFLAGS = -I$(top_srcdir)/include -g -Wall -lcurl

lifesim_SOURCES = frontend/main.cpp
lifesim_LDADD = $(LDADD) -lcurl

liblife_a_SOURCES = \
    $(top_srcdir)/backend/events/urgent_life_event.cpp \
    $(top_srcdir)/backend/events/year_logger.cpp \
    $(top_srcdir)/backend/interface/interface.cpp \
    $(top_srcdir)/backend/life/life.cpp \
    $(top_srcdir)/backend/names/name_generator.cpp \
    $(top_srcdir)/backend/nations/nation_generator.cpp \
    $(top_srcdir)/backend/school/life_education.cpp \
    $(top_srcdir)/backend/network/network.cpp
