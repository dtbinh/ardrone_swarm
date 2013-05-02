default: install

EXTRA_CMAKE_FLAGS = -DCMAKE_INSTALL_PREFIX=`rospack find bfl_boost`/install
include $(shell rospack find mk)/cmake.mk

install: all
	cd build && make install
