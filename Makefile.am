bin_PROGRAMS = lifesim_terminal lifesim_gui
noinst_LIBRARIES = liblife.a 

LDADD = $(noinst_LIBRARIES)
AM_CPPFLAGS = -I$(srcdir)/include -g -Wall

lifesim_terminal_SOURCES = \
    $(srcdir)/frontend/terminal/main.cpp \
    $(srcdir)/include/interface.hpp

lifesim_terminal_LDADD = $(LDADD) -lcurl

lifesim_gui_SOURCES = \
    $(srcdir)/frontend/gui/main.cpp \
    $(srcdir)/include/interface.hpp \
    $(srcdir)/raygui/src/raygui.h
lifesim_gui_LDADD = $(LDADD) -lcurl $(srcdir)/raylib/raylib/libraylib.a
lifesim_gui_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/raylib/src -I$(srcdir)/raygui/src

$(srcdir)/raylib/raylib/libraylib.a: $(srcdir)/raylib/Makefile
	$(MAKE) -C $(srcdir)/raylib

liblife_a_SOURCES = \
    $(srcdir)/backend/events/urgent_life_event.cpp \
    $(srcdir)/backend/events/year_logger.cpp \
    $(srcdir)/backend/interface/interface.cpp \
    $(srcdir)/backend/life/life.cpp \
    $(srcdir)/backend/names/name_generator.cpp \
    $(srcdir)/backend/nations/nation_generator.cpp \
    $(srcdir)/backend/school/life_education.cpp \
    $(srcdir)/backend/network/network.cpp \
    $(srcdir)/backend/jobs/job.cpp \
    $(srcdir)/backend/jobs/company.cpp \
    $(srcdir)/backend/jobs/employer.cpp \
\
    $(srcdir)/include/events.hpp \
    $(srcdir)/include/jobs.hpp \
    $(srcdir)/include/life.hpp \
    $(srcdir)/include/names.hpp \
    $(srcdir)/include/nations.hpp \
    $(srcdir)/include/network.hpp \
    $(srcdir)/include/school.hpp
