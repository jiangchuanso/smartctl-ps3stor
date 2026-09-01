# smartctl-ps3stor

<p align="center">
  <a href="https://github.com/jiangchuanso/smartctl-ps3stor/blob/main/README-ZH.md"><strong>中文</strong></a>
  &nbsp;·&nbsp;
  <a href="https://github.com/jiangchuanso/smartctl-ps3stor/blob/main/README.md">English</a>
</p>

# README-ZH（中文）

基于 [smartmontools 7.4](https://www.smartmontools.org/) 的 `smartctl`，增加了自定义的 **`ps3stor`** 设备类型，用于读取 PS3 存储控制器的 SMART 信息。

本仓库提供：

- 完整可编译的 smartmontools 7.4 源码（已集成 `ps3stor` 设备支持）
- `smartctl-ps3stor.spec`：用于打包 **CentOS 8（el8）** 的 RPM（包名 `smartmontools`，版本固定为上游 `7.4`）
- `.github/workflows/build-centos8-rpm.yml`：通过 GitHub Actions 在每次发布 Release 时，自动构建并附加 **x86_64 / aarch64** 两个架构的 CentOS 8 RPM

## 上游来源

本仓库的 `ps3stor` 设备支持（含 `ps3lib/` 厂商库、`ps3stor` 设备后端、`os_linux.cpp` / `scsinvme.cpp` 等集成）来自上游分支
[smart_7.4_ps3stor](https://gitee.com/babyxong/smart_7.4_ps3stor)。该分支基于 **smartmontools 7.4**，集成了 PS3 存储控制器的 SMART 读取能力；
本仓库在此基础上完成了 CentOS 8（el8）x86_64 / aarch64 双架构的原生 RPM 打包（RPM 版本固定为上游 `7.4`）。

## 一、兼容性说明（重要）

本仓库 **不附带** 任何预编译二进制；提供给 CentOS 8（el8）的 RPM 由 CI 在 **CentOS 8 容器** 内从源码 `rpmbuild`，因此二进制链接的是 CentOS 8 平台的 glibc / libstdc++，**保证 x86_64 与 aarch64 两个架构的原生兼容**。

- 本地 `./configure && make` 在你自己的构建机上编译出的 `smartctl` / `smartd` 依赖该构建机的工具链；
- 而 RPM 包中的二进制是在 CentOS 8 的 glibc 环境下编译的，可在 CentOS 8 上直接运行，无需适配。

## 二、支持的命令

（命令与参数含义整理自上游 `smartctl_ps3stor_README`，现已并入本文件）：

```bash
# 1. 扫描设备
smartctl --scan

# 2. 查看指定 PS3 存储控制器的详细信息
#    smartctl -x -d ps3stor,<设备ID> /dev/ctrl/<控制器ID>
smartctl -x -d ps3stor,16 /dev/ctrl/1

# 3. 以 JSON 格式输出（便于程序解析）
smartctl -x -d ps3stor,16 /dev/ctrl/1 -j
```

参数含义：

```
smartctl -x -d ps3stor,16 /dev/ctrl/1
                     ^      ^           ^
                     |      |           |------ Controller ID（控制器编号）
                     |      |------ Device ID（设备编号）
                     |------ ps3stor 设备类型
```

## 三、本地从源码构建

参考 [smartmontools 官方下载与编译说明](https://www.smartmontools.org/wiki/Download)。

> 以下源码安装步骤整理自上游 `smartctl_ps3stor_README`（已并入本文件）。

**方式一：使用本仓库（推荐）**

```bash
# 1. 克隆仓库（本仓库即是解压后的源码目录）
git clone https://github.com/jiangchuanso/smartctl-ps3stor.git
cd smartctl-ps3stor

# 2. 配置并编译
./configure
make

# 3. 生成可执行文件 ./smartctl
```

**方式二：使用上游 tarball（历史方式）**

```bash
# 1. 解压压缩包
tar zxvf smartctl_ps3_1_0_0.tar.gz

# 2. 进入目录并构建
cd smartctl_ps3_1_0_0
./configure
make

# 3. 生成可执行文件 smartctl
```

## 四、通过 GitHub Actions 构建 CentOS 8 RPM

工作流文件：`.github/workflows/build-centos8-rpm.yml`

- **触发方式一（推荐，直接拿到 RPM 附件）**：在 GitHub 上 **Draft a new release** 并打 tag（如 `v1.0.0`）后 **Publish**。Actions 会自动矩阵构建 x86_64 / aarch64（aarch64 通过 QEMU + `centos:8` 容器模拟），完成后两个 RPM 自动附加到该 Release。
- **触发方式二（仅测试）**：在 **Actions → Build CentOS 8 RPMs → Run workflow** 手动触发，产物以 Artifact 形式提供下载。

构建特征：

- 在 **CentOS 8 容器** 内 `dnf install gcc-c++ make rpm-build` 后 `rpmbuild`，保证链接平台 glibc。
- 已修复 CentOS 8 EOL 后的软件源（重定向到 `vault.centos.org`）。
- **RPM 版本固定为上游 `7.4`，与 Release 的 tag/版本无关**；Release 号（`*`）仅由 spec 的 `Release:` 字段控制（当前为 `4`，即 `4%{?dist}` → `.el8`）。
- 产物命名（包名 `smartmontools`，版本 `7.4`，Release `4`，架构 x86_64 / aarch64）：
  - `smartmontools-7.4-4.el8.x86_64.rpm`
  - `smartmontools-7.4-4.el8.aarch64.rpm`

> 提示：若 `centos:8` 多架构镜像日后被下架，可将工作流中 aarch64 对应的镜像改为 `arm64v8/centos:8`。

## 五、安装与使用（CentOS 8）

```bash
# 安装 RPM（aarch64 机器换成对应的 .aarch64.rpm 文件）
sudo rpm -ivh smartmontools-7.4-4.el8.x86_64.rpm

# 验证
which smartctl
smartctl --version

# 使用 ps3stor 设备类型读取 PS3 存储控制器信息
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1 -j   # JSON 输出
```

## 六、邮件通知用法说明

RPM 内置 `smartd_warning.d` 插件 `smart_curl_mail`，用 `curl(1)` 内置的 SMTP 协议直发告警邮件（与 smartmontools.spec 的 `smart_curl_mail` 插件同一实现），**不依赖 Python、不依赖本地 MTA**。依赖 `curl >= 7.20`（需编译 SMTP 支持），已写入 spec 的 `Requires`。

插件与配置分离：

- `/etc/smartd_warning.d/smart_curl_mail` —— 插件脚本（安装后 755），被 `/etc/smartd_warning.sh` 调用；
- `/etc/smartd_warning.d/smart_curl_mail.conf` —— 独立配置模板（安装后 600，因可能含 SMTP 凭据），全部项注释掉即用默认值（`smtp://localhost:25` 明文、无认证）。

使用官方 smartd 调用方式，在 `/etc/smartd.conf` 的 `-m` 收件人列表中加上 `@smart_curl_mail`，并用 `-M exec /etc/smartd_warning.sh` 触发：

```
# /etc/smartd.conf
/dev/sda -m @smart_curl_mail,admin@example.com -M exec /etc/smartd_warning.sh
```

配置（编辑 `/etc/smartd_warning.d/smart_curl_mail.conf`，变量与脚本内一致）：

- `SMARTD_SMTP_URL`：SMTP 服务器，如 `smtp://host:25`、`smtps://host:465`、`smtp://host:587`（配合 `SMARTD_CURL_OPTS='--ssl-reqd'` 走 STARTTLS）。
- `SMARTD_MAIL_FROM`：发件地址，默认 `smartd@localhost`。
- `SMARTD_SMTP_AUTH_USER` / `SMARTD_SMTP_AUTH_PASS`：SMTP AUTH 凭据（可选，写入后建议 `chmod 600`）。
- `SMARTD_CURL_OPTS`：附加 curl 选项，如 `--connect-timeout 10 --max-time 60`。

对于 PS3 存储控制器，可在 `/etc/smartd.conf` 中直接用 `-d ps3stor,16` 等设备行加入监控（`smartctl --scan` 可枚举 `ps3stor` 设备）。

### 发送测试邮件（官方 smartd 方式）

smartd 自带测试机制：在 `/etc/smartd.conf` 的设备行加上 `-M test`，smartd 启动时会按配置发送一封测试邮件，无需手动调用插件。

```
# /etc/smartd.conf
/dev/sda -m @smart_curl_mail,admin@example.com -M exec /etc/smartd_warning.sh -M test
```

```bash
# 重新加载配置并触发测试邮件
sudo systemctl restart smartd
```

- `-M test` 复用同一行已有的 `-m @smart_curl_mail` 收件人与 `-M exec /etc/smartd_warning.sh`，并读取 `smart_curl_mail.conf` 的 SMTP 配置；
- 发送失败会在 smartd 日志（`journalctl -u smartd`）中报错，据此调整 `smart_curl_mail.conf`；
- 需确保目标 SMTP 服务器可达（如 `smtp://localhost:25` 需本机有监听的 SMTP 服务，或改为真实邮件服务器地址）；
- 测试完成后移除 `-M test`，避免每次启动都发测试邮件。

## 七、文件说明

| 文件 | 说明 |
| --- | --- |
| `smartctl-ps3stor.spec` | CentOS 8 RPM 打包 spec（包名 `smartmontools`，版本固定 `7.4`，Release 由打包设置控制，当前 `3`） |
| `.github/workflows/build-centos8-rpm.yml` | 发布时自动构建 x86_64 / aarch64 两个 el8 RPM 的工作流 |
| `smartd_warning.d` | RPM 内置的 `smart_curl_mail` curl SMTP 告警插件（脚本 + 独立 conf，见「邮件通知用法说明」） |
| 其余文件 | smartmontools 7.4 完整源码（含 `ps3stor` 设备支持） |

## 八、许可证

遵循 smartmontools 上游许可 **GPLv2+**（见 `COPYING`）。
