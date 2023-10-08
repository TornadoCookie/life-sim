bin_PROGRAMS = lifesim
noinst_LIBRARIES = liblife.a 

LDADD = $(noinst_LIBRARIES)
AM_CPPFLAGS = -I$(srcdir)/include -g -Wall

lifesim_SOURCES = \
    $(srcdir)/frontend/main.cpp \
    $(srcdir)/include/interface.hpp

lifesim_LDADD = $(LDADD) -lcurl

liblife_a_SOURCES = \
    $(srcdir)/backend/events/urgent_life_event.cpp \
    $(srcdir)/backend/events/year_logger.cpp \
    $(srcdir)/backend/interface/interface.cpp \
    $(srcdir)/backend/life/life.cpp \
    $(srcdir)/backend/names/name_generator.cpp \
    $(srcdir)/backend/nations/nation_generator.cpp \
    $(srcdir)/backend/school/life_education.cpp \
    $(srcdir)/backend/network/network.cpp \
    $(srcdir)/backend/jobs/jobs.cpp \
    $(srcdir)/include/events.hpp \
    $(srcdir)/include/jobs.hpp \
    $(srcdir)/include/life.hpp \
    $(srcdir)/include/names.hpp \
    $(srcdir)/include/nations.hpp \
    $(srcdir)/include/network.hpp \
    $(srcdir)/include/school.hpp
