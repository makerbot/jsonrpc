Name:		jsonrpc
Version:	2.0.0
Release:	1%{?dist}
Summary:	MakerBot JSON-RPC library

License:	GPL
URL:		http://github.com/makerbot/json-cpp
Source:	        %{name}-%{version}.tar.gz

BuildRequires:	gcc, jsoncpp-devel >= 0.6.0
Requires:	jsoncpp >= 0.6.0


%description
A C++ implementation of the JSON-RPC protocol

%prep
%setup -q -n %{name}


%build
scons install_prefix=%{buildroot}/%{_prefix} config_prefix=%{buildroot}/%{_sysconfdir}


%install
rm -rf %{build_root}
scons install_prefix=%{buildroot}/%{_prefix} config_prefix=%{buildroot}/%{_sysconfdir} install


%files
/usr/lib/lib*.so


%package devel
Summary:	Developent files for MakerBot jsonrpc
Requires:	jsonrpc

%description devel
A C++ implementation fo the JSON-RPC protocol

%files devel
%{_includedir}/%{name}/*.h


%changelog

