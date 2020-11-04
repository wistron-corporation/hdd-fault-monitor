AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS = -I$(top_srcdir)

bin_PROGRAMS = \
	hdd-monitor

hdd_monitor_SOURCES = \
	argument.cpp \
	main.cpp \
	hdd-manager.cpp \
	mowgli-hdd.cpp 

hdd_monitor_LDADD = \
	-li2c \
	$(top_builddir)/libhddmonitor.la \
	$(PHOSPHOR_LOGGING_LIBS) \
	${PHOSPHOR_DBUS_INTERFACES_LIBS} \
	$(SDBUSPLUS_LIBS) \
	$(SDEVENTPLUS_LIBS)

hdd_monitor_CXXFLAGS = \
	$(PHOSPHOR_LOGGING_CFLAGS) \
	${PHOSPHOR_DBUS_INTERFACES_CFLAGS} \
	$(SDBUSPLUS_CFLAGS) \
	$(SDEVENTPLUS_CFLAGS)

