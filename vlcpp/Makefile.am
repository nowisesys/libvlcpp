bin_PROGRAMS   = vlcpp

vlcpp_SOURCES  = \
  application.hpp application.cpp \
  options.hpp options.cpp \
  main.cpp

vlcpp_CXXFLAGS = -I../libvlcpp
vlcpp_LDADD    = ../libvlcpp/libvlcpp.la -ldl
