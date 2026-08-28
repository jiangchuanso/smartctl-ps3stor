# Package name and version follow the upstream smartmontools release so the
# RPM is named smartmontools-7.4-<Release>.el8.<arch>.rpm. The Release number
# (currently 3) is the only part controlled by this project's packaging.
Name:           smartmontools
Version:        7.4
Release:        3%{?dist}
Summary:        smartctl extended with PS3STOR device support (based on smartmontools 7.4)

License:        GPLv2+
URL:            https://www.smartmontools.org/
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  rpm-build
# The smartd_warning.d/smart_curl_mail plugin (see the install section) sends
# alert emails directly via the SMTP protocol implemented by curl(1).
Requires:       curl

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

# smartd_warning.sh runs plugin scripts from smartd_warning.d (default
# --with-smartdplugindir = %{_sysconfdir}/smartd_warning.d). 'make install'
# does not create this directory, but the upstream 'bin-dist' layout ships
# it as an (empty) drop-in dir, so create it here for parity.
mkdir -p %{buildroot}%{_sysconfdir}/smartd_warning.d

# smartd_warning.d plugin: send smartd alert emails directly via SMTP with
# curl(1).  No local MTA is required.  Default: plain SMTP on port 25,
# no encryption, no authentication.  SMTPS/STARTTLS/AUTH examples are given
# in the script comments and in the smart_curl_mail.conf template.
cat > %{buildroot}%{_sysconfdir}/smartd_warning.d/smart_curl_mail <<'SMART_CURL_MAIL_EOF'
#!/bin/sh
#
# smart_curl_mail - smartd_warning.d plugin that sends smartd alert emails
# directly via SMTP using curl(1).  A local MTA is not required.
#
# SPDX-License-Identifier: GPL-2.0-or-later
#
# === Usage ===
# In /etc/smartd.conf add the plugin name (with a leading '@') to the
# comma-separated -m address list and use /etc/smartd_warning.sh as the exec
# script, e.g.:
#
#   -m @smart_curl_mail,admin@example.com -M exec /etc/smartd_warning.sh
#
# The plugin is started by /etc/smartd_warning.sh and reads the message from
# the environment variables SMARTD_ADDRESS, SMARTD_SUBJECT and
# SMARTD_FULLMESSAGE (see 'man smartd.conf' / 'man smartd_warning.sh').
#
# === Configuration ===
# Defaults below: plain SMTP to localhost:25, no encryption, no AUTH.
# Every variable may be overridden in
# /etc/smartd_warning.d/smart_curl_mail.conf (same variable names).
#
# Encryption examples (put the uncommented lines into the .conf file):
#   Implicit TLS (SMTPS, port 465):
#     SMARTD_SMTP_URL='smtps://mail.example.com:465'
#   STARTTLS on port 587:
#     SMARTD_SMTP_URL='smtp://mail.example.com:587'
#     SMARTD_CURL_OPTS='--ssl-reqd'
#   SMTP AUTH (LOGIN/PLAIN) if the server requires it:
#     SMARTD_SMTP_AUTH_USER='user'
#     SMARTD_SMTP_AUTH_PASS='password'
#   Any additional curl options (space separated):
#     SMARTD_CURL_OPTS='--connect-timeout 10 --max-time 60'
#
# Requires: curl with SMTP support (curl >= 7.20).

# --- Default settings (plain SMTP, port 25, no encryption) ---
SMARTD_SMTP_URL='smtp://localhost:25'
SMARTD_MAIL_FROM='smartd@localhost'
SMARTD_SMTP_AUTH_USER=
SMARTD_SMTP_AUTH_PASS=
SMARTD_CURL_OPTS=

# Override with user configuration if present
if [ -r /etc/smartd_warning.d/smart_curl_mail.conf ]; then
  . /etc/smartd_warning.d/smart_curl_mail.conf
fi

# Basic sanity checks
if [ -z "$SMARTD_SMTP_URL" ] || [ -z "$SMARTD_MAIL_FROM" ]; then
  echo "$0: SMARTD_SMTP_URL and SMARTD_MAIL_FROM must not be empty" >&2
  exit 1
fi
if [ -z "$SMARTD_ADDRESS" ]; then
  echo "$0: SMARTD_ADDRESS is empty - no recipients" >&2
  exit 1
fi

command -v curl >/dev/null 2>&1 || {
  echo "$0: curl not found - cannot send mail" >&2
  exit 1
}

# RFC 2047: encode non-ASCII text as UTF-8 base64 encoded words
rfc2047() {
  if printf '%%s' "$1" | LC_ALL=C grep -q '[^ -~]'; then
    printf '=?UTF-8?B?%%s?=' "$(printf '%%s' "$1" | base64 -w0)"
  else
    printf '%%s' "$1"
  fi
}

# Build RFC 5322 message with headers and SMARTD_FULLMESSAGE as body
tmp=$(mktemp) || exit 1
trap 'rm -f "$tmp"' EXIT HUP INT TERM

{
  printf 'From: <%%s>\n' "$SMARTD_MAIL_FROM"
  for a in $SMARTD_ADDRESS; do
    printf 'To: <%%s>\n' "$a"
  done
  printf 'Subject: %%s\n' "$(rfc2047 "${SMARTD_SUBJECT:-SMART error detected}")"
  printf 'Date: %%s\n' "$(date -R 2>/dev/null || date 2>/dev/null)"
  printf 'MIME-Version: 1.0\n'
  printf 'Content-Type: text/plain; charset=UTF-8\n'
  printf 'Content-Transfer-Encoding: 8bit\n'
  printf '\n'
  printf '%%s\n' "${SMARTD_FULLMESSAGE:-Smartd warning message}"
} > "$tmp"

# Assemble curl command line (POSIX sh, no arrays needed)
set -- --silent --show-error
set -- "$@" --url "$SMARTD_SMTP_URL"
set -- "$@" --mail-from "$SMARTD_MAIL_FROM"
for a in $SMARTD_ADDRESS; do
  set -- "$@" --mail-rcpt "$a"
done
if [ -n "$SMARTD_SMTP_AUTH_USER" ]; then
  set -- "$@" --login "$SMARTD_SMTP_AUTH_USER" --password "$SMARTD_SMTP_AUTH_PASS"
fi
# shellcheck disable=SC2086
set -- "$@" $SMARTD_CURL_OPTS
set -- "$@" --upload-file "$tmp"

# Send the message (plain SMTP by default, see .conf for encryption)
curl "$@"
SMART_CURL_MAIL_EOF
chmod 755 %{buildroot}%{_sysconfdir}/smartd_warning.d/smart_curl_mail

# Optional user configuration template (all values commented out = use defaults)
cat > %{buildroot}%{_sysconfdir}/smartd_warning.d/smart_curl_mail.conf <<'SMART_CURL_MAIL_CONF_EOF'
# /etc/smartd_warning.d/smart_curl_mail.conf
# Optional configuration for the smart_curl_mail smartd warning plugin.
# All values are commented out - the built-in defaults (plain SMTP to
# localhost:25, no encryption, no authentication) are used instead.
# Uncomment and adjust only what you need.  This file is read by
# /etc/smartd_warning.d/smart_curl_mail.
#
# NOTE: the SMTP URL defaults to smtp://localhost:25 (plain, no auth). If
# your mail server requires TLS or AUTH, set the variables here:
#
#   # Implicit TLS (SMTPS, port 465):
#   #SMARTD_SMTP_URL='smtps://mail.example.com:465'
#   # STARTTLS on port 587:
#   #SMARTD_SMTP_URL='smtp://mail.example.com:587'
#   #SMARTD_CURL_OPTS='--ssl-reqd'
#   #SMARTD_SMTP_AUTH_USER='user'
#   #SMARTD_SMTP_AUTH_PASS='password'

# SMTP server URL: plain SMTP (default, port 25) ...
#SMARTD_SMTP_URL='smtp://localhost:25'
# ... implicit TLS (SMTPS, port 465) ...
#SMARTD_SMTP_URL='smtps://mail.example.com:465'
# ... or plain port with STARTTLS (port 587):
#SMARTD_SMTP_URL='smtp://mail.example.com:587'
#SMARTD_CURL_OPTS='--ssl-reqd'

# Envelope and RFC 5322 'From:' address
#SMARTD_MAIL_FROM='smartd@localhost'

# SMTP AUTH (LOGIN/PLAIN) credentials - empty disables AUTH.
# NOTE: this file may contain a plaintext password.  If you set the AUTH
# variables here, restrict access to root only:
#   chmod 600 /etc/smartd_warning.d/smart_curl_mail.conf
#SMARTD_SMTP_AUTH_USER=
#SMARTD_SMTP_AUTH_PASS=

# Any additional curl options (space separated)
#SMARTD_CURL_OPTS='--connect-timeout 10 --max-time 60'
SMART_CURL_MAIL_CONF_EOF
chmod 600 %{buildroot}%{_sysconfdir}/smartd_warning.d/smart_curl_mail.conf

%files
%license COPYING
%doc AUTHORS
%doc README README.md README-ZH.md
%{_sbindir}/smartctl
%{_sbindir}/smartd
%{_sbindir}/update-smart-drivedb
%config(noreplace) /etc/smartd.conf
%config(noreplace) /etc/smartd_warning.sh
# Drop-in directory for smartd_warning.sh plugin scripts (see bin-dist layout).
%dir %{_sysconfdir}/smartd_warning.d
%config(noreplace) %{_sysconfdir}/smartd_warning.d/smart_curl_mail
%config(noreplace) %{_sysconfdir}/smartd_warning.d/smart_curl_mail.conf
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
* Fri Aug 28 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 7.4-3
- Upgrade bundled vendor ps3lib from 2.2.0.14 to 2.9.0.12 (new static libs
  libps3.a / libps3_aarch64.a and updated headers under ps3lib/, including the
  new ps3lib_pcieswitch_host.h). No smartctl/smartd source changes.

* Fri Aug 28 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 7.4-2
- Replace the packaging/email Python alert toolkit with a smartd_warning.d
  plugin 'smart_curl_mail' that sends smartd alert emails directly via SMTP
  using curl(1) (no local MTA required). Configuration is split into a
  separate /etc/smartd_warning.d/smart_curl_mail.conf file (installed mode 600,
  since it may hold SMTP credentials); use it via the official smartd
  mechanism: -m @smart_curl_mail,... -M exec /etc/smartd_warning.sh.
  Requires: curl added.

* Thu Aug 27 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 7.4-1
- Rename package to smartmontools and pin the version to the upstream 7.4
  release; the Release number (1) now tracks this project's packaging only.

* Mon Aug 24 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 1.0.0-1
- Add smartd daemon, drivedb.h, update-smart-drivedb and a systemd unit that
  is enabled and started on package install.

* Sun Aug 23 2026 smartctl-ps3stor maintainer <maintainer@example.com> - 1.0.0-1
- Initial package: smartctl 7.4 with ps3stor device support.
