# Package name and version follow the upstream smartmontools release so the
# RPM is named smartmontools-7.4-<Release>.el8.<arch>.rpm. The Release number
# (currently 1) is the only part controlled by this project's packaging.
Name:           smartmontools
Version:        7.4
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

This package provides the smartctl CLI and the smartd daemon (based on
smartmontools 7.4), built natively for the target distribution so they link
against the platform glibc. It also ships drivedb.h and update-smart-drivedb.

%prep
%setup -q -n %{name}-%{version}

%build
# Build against the platform toolchain/glibc so the resulting binary is
# guaranteed compatible with the target OS.
%configure
%make_build

%install
%make_install

# Ship the full useful smartmontools set: the smartctl CLI, the smartd
# daemon, update-smart-drivedb, drivedb.h, plus their man pages and config.
# Only the upstream documentation tree is dropped; the docs we publish are
# declared explicitly below via %doc/%license.
rm -rf %{buildroot}%{_docdir}/smartmontools

# Install a systemd unit for smartd. The upstream smartd.service.in uses the
# /usr/local prefix, which does not match this package's FHS layout, so we
# ship our own unit pointing at /usr/sbin/smartd and /etc.
mkdir -p %{buildroot}/usr/lib/systemd/system
cat > %{buildroot}/usr/lib/systemd/system/smartd.service <<'EOF'
[Unit]
Description=Self Monitoring and Reporting Technology (SMART) Daemon
Documentation=man:smartd(8) man:smartd.conf(5)
ConditionVirtualization=no

[Service]
Type=simple
EnvironmentFile=-/etc/sysconfig/smartmontools
ExecStart=/usr/sbin/smartd -n $smartd_opts
ExecReload=/bin/kill -HUP $MAINPID

[Install]
WantedBy=multi-user.target
EOF

%files
%license COPYING
%doc AUTHORS
%doc README README.md README-ZH.md
%{_sbindir}/smartctl
%{_sbindir}/smartd
%{_sbindir}/update-smart-drivedb
%config(noreplace) /etc/smartd.conf
%config(noreplace) /etc/smartd_warning.sh
%{_datadir}/smartmontools/drivedb.h
%{_mandir}/man8/smartctl.8*
%{_mandir}/man8/smartd.8*
%{_mandir}/man8/update-smart-drivedb.8*
%{_mandir}/man5/smartd.conf.5*
/usr/lib/systemd/system/smartd.service

%post
/usr/bin/systemctl daemon-reload >/dev/null 2>&1 || :
/usr/bin/systemctl enable smartd.service >/dev/null 2>&1 || :
/usr/bin/systemctl start smartd.service >/dev/null 2>&1 || :

%preun
if [ "$1" -eq 0 ] ; then
    /usr/bin/systemctl stop smartd.service >/dev/null 2>&1 || :
    /usr/bin/systemctl disable smartd.service >/dev/null 2>&1 || :
fi

%postun
/usr/bin/systemctl daemon-reload >/dev/null 2>&1 || :

%changelog
* Thu Aug 27 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 7.4-1
- Rename package to smartmontools and pin the version to the upstream 7.4
  release; the Release number (1) now tracks this project's packaging only.

* Mon Aug 24 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 1.0.0-1
- Add smartd daemon, drivedb.h, update-smart-drivedb and a systemd unit that
  is enabled and started on package install.

* Sun Aug 23 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 1.0.0-1
- Initial package: smartctl 7.4 with ps3stor device support.
