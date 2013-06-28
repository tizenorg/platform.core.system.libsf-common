Name:           libsf-common
Version:        0.3.20
Release:        1
License:        Apache-2.0
Summary:        Sensor framework common library
Group:          System/Sensor Framework
Source0:        libsf-common-%{version}.tar.gz
Source1001: 	libsf-common.manifest
BuildRequires:  cmake
BuildRequires:  libattr-devel
BuildRequires:  pkgconfig(dlog)
%description
Commonly used code and defintions for the sensor framework.

%package devel
Summary:        Sensor framework common - Development
Group:          Development/Libraries
Requires:       %{name} = %{version}

%description devel
Sensor framework common library - Development Files.

%prep
%setup -q
cp %{SOURCE1001} .

%build
%cmake .

make %{?_smp_mflags}

%install
%make_install

mkdir -p %{buildroot}%{_datadir}/license
cp LICENSE %{buildroot}%{_datadir}/license/%{name}

%files
%manifest %{name}.manifest
%{_libdir}/libsf_common.so
%{_datadir}/license/%{name}


%files devel
%manifest %{name}.manifest
%{_includedir}/sf_common/*.h
%{_libdir}/pkgconfig/sf_common.pc
