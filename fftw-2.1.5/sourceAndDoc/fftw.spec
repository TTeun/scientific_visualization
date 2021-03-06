# RPM spec file for FFTW.
# This file is used to build Redhat Package Manager packages for the
# FFTW library.  Such packages make it easy to install and uninstall
# the library and related files from binaries or source.
#
# This spec file is for version 2.1.5 of FFTW; the appropriate
# version numbers are automatically substituted in to fftw.spec.in
# by the configure script.  However, fftw.spec.in may need to be
# modified for future releases, if the list of installed files
# or build commands change.
#
# The icon associated with this package can be downloaded from:
#     http://www.fftw.org/fftw-logo-thumb.gif
# and will need to be placed in /usr/src/redhat/SOURCES, along with
# the fftw-2.1.5.tar.gz file, to build the RPM.  To build, use
# the command: rpm --clean -ba fftw.spec
#
# Alternatively, you can just use 'make rpm' (as root).
#
Name: fftw
Summary: fast fourier transform library
Version: 2.1.5
Release: 1
Copyright: GPL
Icon: fftw-logo-thumb.gif
Group: Libraries
Prefix: /usr/local
BuildRoot: /tmp/%{name}-%{version}-buildroot
Source: ftp://ftp.fftw.org/pub/fftw/fftw-2.1.5.tar.gz
URL: http://www.fftw.org
%description
FFTW is a collection of fast C routines for computing the Discrete Fourier
Transform in one or more dimensions.  It includes complex, real, and
parallel transforms, and can handle arbitrary array sizes efficiently.
This RPM package includes both the double- and single-precision FFTW
uniprocessor and threads libraries.  (The single-precision files have
an "s" prefix.)
%package devel
Summary: headers, libraries, & docs for FFTW fast fourier transform library
Group: Development/Libraries
Prefix: %{prefix}
Requires: fftw = %{version}-%{release}
%description devel
This package contains the additional header files, documentation, and
libraries you need to develop programs using the FFTW fast fourier
transform library.
%prep

# We will be compiling two copies of FFTW, one for double precision and
# one for single precision.  During the build process, these copies
# will be stored in fftw-%{version}/double and fftw-%{version}/single

# Unpack the tar archive, first (-c) creating a fftw-%{version}
# directory and then unpacking in there.
%setup -c

# Now, rename the unpacked FFTW directory to "double":
mv ${RPM_PACKAGE_NAME}-${RPM_PACKAGE_VERSION} double

# Second, make a copy of this directory in "single":
cp -r double single

%build

# Configure and build the double and single precision versions.
# Notes:
#  (1) We install into ${RPM_BUILD_ROOT}, which is set either
#      by the BuildRoot option above or by --buildroot at build-time.
#      This allows you to build the RPM without blowing away your existing
#      FFTW installation, and even without being root.
#  (2) The double-precision version is installed with the normal library
#      names, while the single-precision version is installed with an "s"
#      prefix.

cd double
%ifarch i386
./configure --prefix="${RPM_BUILD_ROOT}%{prefix}" --enable-shared --enable-i386-hacks --enable-threads
%else
./configure --prefix="${RPM_BUILD_ROOT}%{prefix}" --enable-shared --enable-threads
%endif
make

cd ../single
./configure --prefix="${RPM_BUILD_ROOT}%{prefix}" --enable-shared --enable-type-prefix --enable-threads --enable-float
make

%install

cd double
make install

cd ../single
make install

%clean

rm -rf ${RPM_BUILD_ROOT}

%post
# Force ${RPM_INSTALL_PREFIX}/lib to be in /etc/ld.so.conf?
# No...that would make it impossible to completely uninstall reliably.
# Instead, warn the user fix a broken /etc/ld.so.conf manually:
if test -z `grep ${RPM_INSTALL_PREFIX}/lib /etc/ld.so.conf`; then
     echo "warning: ${RPM_INSTALL_PREFIX}/lib is not in /etc/ld.so.conf" 1>&2
     echo "         You should add it and then run ldconfig." 1>&2
fi

# run ldconfig to update the runtime linker database with the new libraries
# (make sure /sbin is in the $PATH)
PATH="/sbin:$PATH" ldconfig

%postun
# after uninstall, run ldconfig to remove the libs from the linker database
PATH="/sbin:$PATH" ldconfig

%files
/usr/local/lib/libfftw.so.2.0.7
/usr/local/lib/libfftw_threads.so.2.0.7
/usr/local/lib/librfftw.so.2.0.7
/usr/local/lib/librfftw_threads.so.2.0.7
/usr/local/lib/libsfftw.so.2.0.7
/usr/local/lib/libsfftw_threads.so.2.0.7
/usr/local/lib/libsrfftw.so.2.0.7
/usr/local/lib/libsrfftw_threads.so.2.0.7
/usr/local/lib/libfftw.so.2
/usr/local/lib/libfftw_threads.so.2
/usr/local/lib/librfftw.so.2
/usr/local/lib/librfftw_threads.so.2
/usr/local/lib/libsfftw.so.2
/usr/local/lib/libsfftw_threads.so.2
/usr/local/lib/libsrfftw.so.2
/usr/local/lib/libsrfftw_threads.so.2
%files devel
/usr/local/include/fftw.h
/usr/local/include/fftw_threads.h
/usr/local/include/rfftw.h
/usr/local/include/rfftw_threads.h
/usr/local/include/sfftw.h
/usr/local/include/sfftw_threads.h
/usr/local/include/srfftw.h
/usr/local/include/srfftw_threads.h
/usr/local/info/fftw.info
/usr/local/info/fftw.info-1
/usr/local/info/fftw.info-2
/usr/local/info/fftw.info-3
/usr/local/info/fftw.info-4
/usr/local/info/fftw.info-5
/usr/local/lib/libfftw.a
/usr/local/lib/libfftw.la
/usr/local/lib/libfftw.so
/usr/local/lib/libfftw_threads.a
/usr/local/lib/libfftw_threads.la
/usr/local/lib/libfftw_threads.so
/usr/local/lib/librfftw.a
/usr/local/lib/librfftw.la
/usr/local/lib/librfftw.so
/usr/local/lib/librfftw_threads.a
/usr/local/lib/librfftw_threads.la
/usr/local/lib/librfftw_threads.so
/usr/local/lib/libsfftw.a
/usr/local/lib/libsfftw.la
/usr/local/lib/libsfftw.so
/usr/local/lib/libsfftw_threads.a
/usr/local/lib/libsfftw_threads.la
/usr/local/lib/libsfftw_threads.so
/usr/local/lib/libsrfftw.a
/usr/local/lib/libsrfftw.la
/usr/local/lib/libsrfftw.so
/usr/local/lib/libsrfftw_threads.a
/usr/local/lib/libsrfftw_threads.la
/usr/local/lib/libsrfftw_threads.so
