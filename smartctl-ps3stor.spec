Name:           smartctl-ps3stor
Version:        1.0.0
Release:        1%{?dist}
Summary:        smartctl extended with PS3STOR device support (based on smartmontools 7.4)

License:        GPLv2+
URL:            https://www.smartmontools.org/
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  rpm-build

%description
smartctl (from smartmontools 7.4) extended with the custom "ps3stor" device
type for PS3 storage controllers, e.g.:

    smartctl --scan
    smartctl -x -d ps3stor,16 /dev/ctrl/1
    smartctl -x -d ps3stor,16 /dev/ctrl/1 -j

This package provides only the smartctl CLI and its manual page, built
natively for the target distribution so it links against the platform glibc.

%prep
%setup -q -n %{name}-%{version}

%build
# Build against the platform toolchain/glibc so the resulting binary is
# guaranteed compatible with the target OS.
%configure
%make_build

%install
%make_install

%files
%license COPYING
%doc AUTHORS
%doc smartctl_ps3stor_README
%{_sbindir}/smartctl
%{_mandir}/man8/smartctl.8*

%changelog
* Sat Aug 23 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 1.0.0-1
- Initial package: smartctl 7.4 with ps3stor device support.
