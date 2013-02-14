Name:       libsf-common
Summary:    Commonly used code and defintions for the sensor framework
Version: 0.3.20
Release:    1
Group:      System/Sensors
License:    Apache-2.0
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
%cmake .


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%files
%manifest libsf-common.manifest
%{_libdir}/libsf_common.so
/usr/share/license/%{name}


%files devel
%{_includedir}/sf_common/*.h
%{_libdir}/pkgconfig/sf_common.pc
