# smartctl-ps3stor

<p align="center">
  <a href="https://github.com/jiangchuanso/smartctl-ps3stor/blob/main/README-ZH.md">中文</a>
  &nbsp;·&nbsp;
  <a href="https://github.com/jiangchuanso/smartctl-ps3stor/blob/main/README.md"><strong>English</strong></a>
</p>

# README (English)

`smartctl-ps3stor` is `smartctl` built on
[smartmontools 7.4](https://www.smartmontools.org/) with a custom **`ps3stor`**
device type added, used to read SMART information from PS3 storage controllers.

This repository provides:

- Complete, compilable smartmontools 7.4 source tree (with `ps3stor` support integrated)
- `smartctl-ps3stor.spec`: RPM spec for **CentOS 8 (el8)** — package name `smartmontools`, version pinned to upstream `7.4`
- `.github/workflows/build-centos8-rpm.yml`: GitHub Actions workflow that builds and attaches **x86_64 / aarch64** CentOS 8 RPMs on every release

## Upstream source

The `ps3stor` device support (including the `ps3lib/` vendor library and the
`ps3stor` device backend in `os_linux.cpp` / `scsinvme.cpp`) originates from the
upstream branch
[smart_7.4_ps3stor](https://gitee.com/babyxong/smart_7.4_ps3stor), which is
based on **smartmontools 7.4**. This repository adds native CentOS 8 (el8)
x86_64 / aarch64 RPM packaging on top of it (RPM version pinned to upstream `7.4`).

## Compatibility

This repository ships **no prebuilt binaries**. The CentOS 8 (el8) RPMs are
built from source inside a **CentOS 8 container**, so the binaries link against
CentOS 8 glibc / libstdc++ and are natively compatible with both **x86_64** and
**aarch64**.

## Supported commands

```bash
# 1. Scan devices
smartctl --scan

# 2. Inspect a PS3 storage controller
#    smartctl -x -d ps3stor,<Device ID> /dev/ctrl/<Controller ID>
smartctl -x -d ps3stor,16 /dev/ctrl/1

# 3. JSON output (for programmatic parsing)
smartctl -x -d ps3stor,16 /dev/ctrl/1 -j
```

Argument meaning:

```
smartctl -x -d ps3stor,16 /dev/ctrl/1
                     ^      ^           ^
                     |      |           |------ Controller ID
                     |      |------ Device ID
                     |------ ps3stor device type
```

## Build from source

See the [smartmontools download & build instructions](https://www.smartmontools.org/wiki/Download).

```bash
# Option A: use this repository (recommended)
git clone https://github.com/jiangchuanso/smartctl-ps3stor.git
cd smartctl-ps3stor
./configure
make
# produces ./smartctl

# Option B: use the upstream tarball (historical)
tar zxvf smartctl_ps3_1_0_0.tar.gz
cd smartctl_ps3_1_0_0
./configure
make
```

## Build CentOS 8 RPM via GitHub Actions

Workflow: `.github/workflows/build-centos8-rpm.yml`

- **Trigger 1 (recommended):** draft & publish a GitHub Release (tag e.g. `v1.0.0`).
  The workflow matrix-builds x86_64 / aarch64 (aarch64 via QEMU + `centos:8`
  container) and attaches both RPMs to the release.
- **Trigger 2 (testing):** run manually via `Actions → Build CentOS 8 RPMs → Run workflow`.

Build notes:

- Built inside a **CentOS 8 container** (`dnf install gcc-c++ make rpm-build`) so it links the platform glibc.
- CentOS 8 EOL repos are redirected to `vault.centos.org`.
- **RPM version is pinned to upstream `7.4`** and is independent of the release tag; the Release number (`*`) is controlled solely by the spec's `Release:` field (currently `2`, i.e. `2%{?dist}` → `.el8`).
- Artifacts:
  - `smartmontools-7.4-2.el8.x86_64.rpm`
  - `smartmontools-7.4-2.el8.aarch64.rpm`

## Install & usage (CentOS 8)

```bash
# Install the RPM (on aarch64 use the .aarch64.rpm)
sudo rpm -ivh smartmontools-7.4-2.el8.x86_64.rpm

# Verify
which smartctl
smartctl --version

# Read PS3 storage controller info via the ps3stor device type
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1 -j   # JSON output
```

## Email alert (SMART monitoring)

The RPM ships a `smartd_warning.d` plugin, `smart_curl_mail`, which sends smartd
alert emails directly via the SMTP protocol built into `curl(1)` (same
implementation as smartmontools.spec's `smart_curl_mail` plugin) — **no Python,
no local MTA required**. Requires `curl >= 7.20` (with SMTP support), declared
in the spec's `Requires`.

Script and config are split:

- `/etc/smartd_warning.d/smart_curl_mail` — the plugin script (installed 755),
  invoked by `/etc/smartd_warning.sh`;
- `/etc/smartd_warning.d/smart_curl_mail.conf` — a separate config template
  (installed mode 600, since it may hold SMTP credentials); all values commented
  out mean defaults (`smtp://localhost:25`, plain, no auth).

Use the official smartd mechanism: add `@smart_curl_mail` to the `-m` recipient
list in `/etc/smartd.conf` and trigger with `-M exec /etc/smartd_warning.sh`:

```
# /etc/smartd.conf
/dev/sda -m @smart_curl_mail,admin@example.com -M exec /etc/smartd_warning.sh
```

Configuration (edit `/etc/smartd_warning.d/smart_curl_mail.conf`, same variable
names as in the script):

- `SMARTD_SMTP_URL`: SMTP server, e.g. `smtp://host:25`, `smtps://host:465`, or
  `smtp://host:587` with `SMARTD_CURL_OPTS='--ssl-reqd'` for STARTTLS.
- `SMARTD_MAIL_FROM`: envelope/From address, default `smartd@localhost`.
- `SMARTD_SMTP_AUTH_USER` / `SMARTD_SMTP_AUTH_PASS`: SMTP AUTH credentials
  (optional; if set, consider `chmod 600` on the .conf file).
- `SMARTD_CURL_OPTS`: extra curl options, e.g. `--connect-timeout 10 --max-time 60`.

For PS3 storage controllers, add device lines such as `-d ps3stor,16` to
`/etc/smartd.conf` (`smartctl --scan` enumerates `ps3stor` devices).

### Sending a test email (official smartd way)

smartd has a built-in test mechanism: add `-M test` to a device line in
`/etc/smartd.conf`, and smartd sends a test email on startup using the same
configuration — no need to invoke the plugin manually:

```
# /etc/smartd.conf
/dev/sda -m @smart_curl_mail,admin@example.com -M exec /etc/smartd_warning.sh -M test
```

```bash
# Reload the config and trigger the test email
sudo systemctl restart smartd
```

- `-M test` reuses the same line's `-m @smart_curl_mail` recipient and
  `-M exec /etc/smartd_warning.sh`, and reads SMTP settings from
  `smart_curl_mail.conf`.
- On failure smartd logs the error (`journalctl -u smartd`); adjust
  `smart_curl_mail.conf` accordingly.
- Make sure the target SMTP server is reachable (e.g. `smtp://localhost:25`
  requires a listening SMTP service, or point it at a real mail server).
- Remove `-M test` after testing so smartd does not send a test email on every
  startup.

## File layout

| File | Purpose |
| --- | --- |
| `smartctl-ps3stor.spec` | CentOS 8 RPM spec (name `smartmontools`, version `7.4`, Release `2`) |
| `.github/workflows/build-centos8-rpm.yml` | builds x86_64 / aarch64 el8 RPMs on release |
| `smartd_warning.d` | `smart_curl_mail` curl SMTP alert plugin shipped by the RPM (script + conf) |
| rest | full smartmontools 7.4 source (with `ps3stor` support) |

## License

smartmontools upstream license **GPLv2+** (see `COPYING`).

## About smartmontools (upstream)

> `$Id: README 4986 2019-12-01 22:04:31Z samm2 $`

**HOME**

The home for smartmontools is located at:

    http://www.smartmontools.org/

Mailing list for support and other questions:

    https://listi.jpberlin.de/mailman/listinfo/smartmontools-support

**OVERVIEW**

smartmontools contains utilities that control and monitor storage devices using
the Self-Monitoring, Analysis and Reporting Technology (SMART) system built into
ATA/SATA and SCSI/SAS hard drives and solid-state drives. This is used to check
the reliability of the drive and to predict drive failures.

**CONTENTS**

The suite contains two utilities:

- `smartctl` — command line utility designed to perform S.M.A.R.T. tasks such as disk self-checks, and to report the S.M.A.R.T. status of the disk.
- `smartd` — daemon that periodically monitors S.M.A.R.T. status and reports errors and changes in S.M.A.R.T. attributes to syslog.

**GETTING STARTED**

To examine SMART data from a disk, try:

    smartctl -a /dev/sda

See the manual page `man smartctl` for more information.

To start automatic monitoring of your disks with the smartd daemon:

    smartd -d     # foreground (debug) mode
    smartd         # background mode (logs to SYSLOG)

To receive email warnings, configure `smartd.conf` with the `-m` mail warning
directive. See `man smartd` for more information.

**OBTAINING / BUILDING / INSTALLING**

- Source tarballs: http://sourceforge.net/projects/smartmontools/files/
- SVN: `svn co http://svn.code.sf.net/p/smartmontools/code/trunk/smartmontools smartmontools`
- Detailed installation instructions: see the `INSTALL` file.
