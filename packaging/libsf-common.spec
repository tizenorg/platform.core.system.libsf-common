Name:       libsf-common
Summary:    Commonly used code and defintions for the sensor framework
Version:    0.3.6
Release:    1
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    libsf-common-%{version}.tar.bz2
BuildRequires:  cmake
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
%{_libdir}/libsf_common.so


%files devel
%{_includedir}/sf_common/*.h
%{_libdir}/pkgconfig/sf_common.pc
