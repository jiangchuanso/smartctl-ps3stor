# smartctl-ps3stor

> 中文 README。English version: [README.md](README.md).

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
- **RPM 版本固定为上游 `7.4`，与 Release 的 tag/版本无关**；Release 号（`*`）仅由 spec 的 `Release:` 字段控制（当前为 `1`，即 `1%{?dist}` → `.el8`）。
- 产物命名（包名 `smartmontools`，版本 `7.4`，Release `1`，架构 x86_64 / aarch64）：
  - `smartmontools-7.4-1.el8.x86_64.rpm`
  - `smartmontools-7.4-1.el8.aarch64.rpm`

> 提示：若 `centos:8` 多架构镜像日后被下架，可将工作流中 aarch64 对应的镜像改为 `arm64v8/centos:8`。

## 五、安装与使用（CentOS 8）

```bash
# 安装 RPM（aarch64 机器换成对应的 .aarch64.rpm 文件）
sudo rpm -ivh smartmontools-7.4-1.el8.x86_64.rpm

# 验证
which smartctl
smartctl --version

# 使用 ps3stor 设备类型读取 PS3 存储控制器信息
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1 -j   # JSON 输出
```

## 六、邮件通知用法说明

本仓库在 `packaging/email/` 下提供了一个开箱即用的 SMART 邮件预警工具集：

- `smartctl-email-alert` —— Python 3 脚本。运行 `smartctl` 采集每块磁盘的 SMART 属性与整体健康自检结果，按「阈值模板」逐项比对：任一指标低于（below）或高于（above）模板值即触发 **SMTP 邮件告警**。邮件直接调用 `curl(1)` 内置的 SMTP 协议发送（与 smartmontools.spec 的 `smart_curl_mail` 插件同一思路），**不依赖 Python smtplib，也无需本机 MTA**。依赖 `curl >= 7.20`（需编译 SMTP 支持）。
- `email.conf.example` —— 配置模板，复制为 `/etc/smartctl/email.conf` 后修改。
- `smartctl-email.service` / `smartctl-email.timer` —— systemd 单元，每 15 分钟检查一次（开机 2 分钟后首次运行）。

快速开始：

```bash
# 1. 安装脚本与配置
sudo cp packaging/email/smartctl-email-alert /usr/local/bin/
sudo chmod +x /usr/local/bin/smartctl-email-alert
sudo mkdir -p /etc/smartctl
sudo cp packaging/email/email.conf.example /etc/smartctl/email.conf
sudo chmod 600 /etc/smartctl/email.conf

# 2. 编辑 /etc/smartctl/email.conf：设置 [smtp] 的 url/from/to，以及 [thresholds]
#    （[smartctl] path = smartctl 使用 PATH 中的 smartctl，可用该键覆盖路径）

# 3. 试运行（仅打印将要发送的告警，不真正发邮件）
sudo smartctl-email-alert --dry-run

# 4. 启用 systemd 定时任务（每 15 分钟）
sudo cp packaging/email/smartctl-email.service /etc/systemd/system/
sudo cp packaging/email/smartctl-email.timer   /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable --now smartctl-email.timer
```

主要配置项（`/etc/smartctl/email.conf`，INI 格式）：

- `[smtp]`：`url`（推荐，如 `smtp://host:25`、`smtps://host:465`、`smtp://host:587` 并设 `curl_opts = --ssl-reqd`）、`from`、`to`（逗号分隔多人）、`subject_prefix`，可选 `user` / `password`。
- `[alert]`：`mode`（`alert` = 仅超阈值时发送；`daily` = 每次运行都发完整报告）、`health_check`、`temp_max`（温度上限 ℃）、`cooldown_minutes`（同一告警冷却，避免邮件轰炸）、`scan_all`（自动发现磁盘）/ `disks` 列表。
- `[thresholds]`：逐属性的阈值模板，例如 `Reallocated_Sector_Ct = value:below:90, raw:above:0`。

对于 PS3 存储控制器，磁盘会通过 `smartctl --scan`（可枚举 `ps3stor` 设备）自动发现；必要时可在 `[smartctl]` 设置 `extra_args = -d ps3stor,16`，或在 `[alert].disks` 中显式列出设备。

## 七、文件说明

| 文件 | 说明 |
| --- | --- |
| `smartctl-ps3stor.spec` | CentOS 8 RPM 打包 spec（包名 `smartmontools`，版本固定 `7.4`，Release 由打包设置控制，当前 `1`） |
| `.github/workflows/build-centos8-rpm.yml` | 发布时自动构建 x86_64 / aarch64 两个 el8 RPM 的工作流 |
| `packaging/email/` | SMART 邮件预警工具集（脚本 + 配置示例 + systemd 单元） |
| 其余文件 | smartmontools 7.4 完整源码（含 `ps3stor` 设备支持） |

## 八、许可证

遵循 smartmontools 上游许可 **GPLv2+**（见 `COPYING`）。
