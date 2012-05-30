Name:       libsf-common
Summary:    Commonly used code and defintions for the sensor framework
Version:    0.3.6
Release:    1
Group:      TO_BE/FILLED_IN
License:    Apache-2.0
Source0:    libsf-common-%{version}.tar.gz
Source1001: packaging/libsf-common.manifest 
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
cp %{SOURCE1001} .
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%manifest libsf-common.manifest
%{_libdir}/libsf_common.so


%files devel
%manifest libsf-common.manifest
%{_includedir}/sf_common/*.h
%{_libdir}/pkgconfig/sf_common.pc
