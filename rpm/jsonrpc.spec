Name:		jsonrpc
Version:	$jsonrpc
Release:	1%{?dist}
Summary:	MakerBot JSON-RPC library

License:	Proprietary
URL:		http://www.makerbot.com/makerware
Source:	        %{name}-%{version}.tar.gz

BuildRequires:	gcc, jsoncpp-devel >= 0.6.0
Requires:	jsoncpp >= 0.6.0


%description
A C++ implementation of the JSON-RPC protocol

%prep
%setup -q -n %{name}


%build
scons --no-devel-libs --install-prefix=%{buildroot}/%{_prefix}


%install
rm -rf %{build_root}
scons --no-devel-libs --install-prefix=%{buildroot}/%{_prefix} install

if [ ! -d %{buildroot}/%{_libdir} ]; then
    mv %{buildroot}/%{_prefix}/lib %{buildroot}/%{_libdir}
fi

%files
%{_libdir}/lib*


%package devel
Summary:	Developent files for MakerBot jsonrpc
Requires:	jsonrpc = $jsonrpc

%description devel
A C++ implementation fo the JSON-RPC protocol

%files devel
%{_includedir}/*


%changelog

