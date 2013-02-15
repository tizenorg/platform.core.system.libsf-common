#sbs-git:slp/pkgs/l/libsf-common libsf-common 0.3.6 07588b34636f76e6457efb6d65e9318513c5957c
Name:       libsf-common
Summary:    Commonly used code and defintions for the sensor framework
Version: 0.3.18 
Release:    1
Group:      TO_BE/FILLED_IN
License:    Apache 2.0
Source0:    libsf-common-%{version}.tar.gz
BuildRequires:  cmake, libattr-devel
BuildRequires:  pkgconfig(dlog)


%description
Sensor framework common library



%package devel
Summary:    Sensor framework common (devel)
Group:      System/Sensor Framework
Requires:   %{name} = %{version}-%{release}
Requires:   libsf-common

%description devel
Sensor framework common library


%prep
%setup -q 


%build
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%manifest libsf-common.manifest
%{_libdir}/libsf_common.so


%files devel
%{_includedir}/sf_common/*.h
%{_libdir}/pkgconfig/sf_common.pc
