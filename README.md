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
- **RPM version is pinned to upstream `7.4`** and is independent of the release tag; the Release number (`*`) is controlled solely by the spec's `Release:` field (currently `1`, i.e. `1%{?dist}` → `.el8`).
- Artifacts:
  - `smartmontools-7.4-1.el8.x86_64.rpm`
  - `smartmontools-7.4-1.el8.aarch64.rpm`

## Install & usage (CentOS 8)

```bash
# Install the RPM (on aarch64 use the .aarch64.rpm)
sudo rpm -ivh smartmontools-7.4-1.el8.x86_64.rpm

# Verify
which smartctl
smartctl --version

# Read PS3 storage controller info via the ps3stor device type
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1 -j   # JSON output
```

## Email alert (SMART monitoring)

A ready-to-use SMART email-alert toolkit lives in `packaging/email/`:

- `smartctl-email-alert` — Python 3 script. It runs `smartctl` on every disk,
  compares SMART attributes / overall-health against threshold templates, and
  sends an **SMTP email alert** when any metric violates a threshold. Email is
  sent via `curl(1)`'s built-in SMTP (same idea as smartmontools.spec's
  `smart_curl_mail` plugin) — **no Python smtplib, no local MTA required**.
  Requires `curl >= 7.20` (built with SMTP support).
- `email.conf.example` — config template; copy to `/etc/smartctl/email.conf`.
- `smartctl-email.service` / `smartctl-email.timer` — systemd units that run the
  check every 15 minutes (first run 2 min after boot).

Quick start:

```bash
# 1. Install the script and config
sudo cp packaging/email/smartctl-email-alert /usr/local/bin/
sudo chmod +x /usr/local/bin/smartctl-email-alert
sudo mkdir -p /etc/smartctl
sudo cp packaging/email/email.conf.example /etc/smartctl/email.conf
sudo chmod 600 /etc/smartctl/email.conf

# 2. Edit /etc/smartctl/email.conf: set [smtp] url/from/to, and [thresholds]
#    (path = smartctl uses the smartctl in PATH; override with [smartctl] path)

# 3. Dry run (prints what would be sent, sends nothing)
sudo smartctl-email-alert --dry-run

# 4. Enable the systemd timer (every 15 min)
sudo cp packaging/email/smartctl-email.service /etc/systemd/system/
sudo cp packaging/email/smartctl-email.timer   /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable --now smartctl-email.timer
```

Key configuration (`/etc/smartctl/email.conf`, INI):

- `[smtp]`: `url` (recommended, e.g. `smtp://host:25`, `smtps://host:465`,
  `smtp://host:587` with `curl_opts = --ssl-reqd`), `from`, `to`
  (comma-separated), `subject_prefix`, optional `user`/`password`.
- `[alert]`: `mode` (`alert` = only on threshold breach; `daily` = full report
  every run), `health_check`, `temp_max` (°C), `cooldown_minutes` (de-dupe
  window), `scan_all` (auto-discover disks) / `disks` list.
- `[thresholds]`: per-attribute templates, e.g.
  `Reallocated_Sector_Ct = value:below:90, raw:above:0`.

For PS3 storage controllers, the disks are auto-discovered via `smartctl --scan`
(which enumerates `ps3stor` devices); if needed, set `[smartctl] extra_args =
-d ps3stor,16` or list the device in `[alert].disks`.

## File layout

| File | Purpose |
| --- | --- |
| `smartctl-ps3stor.spec` | CentOS 8 RPM spec (name `smartmontools`, version `7.4`, Release `1`) |
| `.github/workflows/build-centos8-rpm.yml` | builds x86_64 / aarch64 el8 RPMs on release |
| `packaging/email/` | SMART email-alert toolkit |
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
