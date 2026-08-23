# smartctl-ps3stor

基于 [smartmontools 7.4](https://www.smartmontools.org/) 的 `smartctl`，增加了自定义的 **`ps3stor`** 设备类型，用于读取 PS3 存储控制器的 SMART 信息。

本仓库提供：

- 完整可编译的 smartmontools 7.4 源码（已集成 `ps3stor` 设备支持）
- `smartctl-ps3stor.spec`：用于打包 **CentOS 8（el8）** 的 RPM
- `.github/workflows/build-centos8-rpm.yml`：通过 GitHub Actions 在每次发布 Release 时，自动构建并附加 **x86_64 / aarch64** 两个架构的 CentOS 8 RPM

---

## 一、兼容性说明（重要）

对仓库中已编译好的二进制（`smartctl`，约 6.8 MB）做过 ELF 分析：

| 检查项 | 结果 | 与 CentOS 8 的关系 |
| --- | --- | --- |
| 架构 | **x86_64** | ⚠️ 仅有 x86_64，**不含 aarch64** |
| 最高 GLIBC 需求 | **GLIBC_2.17** | CentOS 8 自带 glibc **2.28** → 2.28 > 2.17，**兼容** |
| 最高 libstdc++ 需求 | GLIBCXX_3.4.19 | CentOS 8 的 libstdc++ 提供 3.4.25 → **兼容** |
| 动态链接 | libc / libm / libpthread / libstdc++ / libgcc_s | CentOS 8 均具备 |

**结论：**

- 现有 **x86_64** 二进制在动态链接层面**兼容 CentOS 8**（大概构建于 CentOS 7 / RHEL 7 环境，比 CentOS 8 更保守，可运行）。
- 但它**仅 x86_64**；CentOS 8 上的 **aarch64** 必须源码重编——这正是本仓库 CI 要解决的。
- 本仓库发布的 RPM 是在 **CentOS 8 容器内** 从源码 `rpmbuild` 的，因此二进制链接的是 CentOS 8 的 glibc，**保证两个架构的原生兼容**。

> 注意：仓库中已通过 `.gitignore` 排除该 6.8 MB 预编译二进制及所有构建产物，GitHub 上的源码从零构建，更干净。

---

## 二、支持的命令

来自 `smartctl_ps3stor_README`：

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

---

## 三、本地从源码构建

参考 [smartmontools 官方下载与编译说明](https://www.smartmontools.org/wiki/Download)。

```bash
# 1. 解压源码（本仓库即是解压后的目录）
#    git clone https://github.com/jiangchuanso/smartctl-ps3stor.git
#    cd smartctl-ps3stor

# 2. 配置并编译
./configure
make

# 3. 生成可执行文件 ./smartctl
```

---

## 四、通过 GitHub Actions 构建 CentOS 8 RPM

工作流文件：`.github/workflows/build-centos8-rpm.yml`

- **触发方式一（推荐，直接拿到 RPM 附件）**：在 GitHub 上 **Draft a new release** 并打 tag（如 `v1.0.0`）后 **Publish**。Actions 会自动矩阵构建 x86_64 / aarch64（aarch64 通过 QEMU + `centos:8` 容器模拟），完成后两个 RPM 自动附加到该 Release。
- **触发方式二（仅测试）**：在 **Actions → Build CentOS 8 RPMs → Run workflow** 手动触发，产物以 Artifact 形式提供下载。

构建特征：

- 在 **CentOS 8 容器** 内 `dnf install gcc-c++ make rpm-build` 后 `rpmbuild`，保证链接平台 glibc。
- 已修复 CentOS 8 EOL 后的软件源（重定向到 `vault.centos.org`）。
- 产物命名示例（包名 `smartctl-ps3stor`，版本 `1.0.0`，Release 带 `.el8`）：
  - `smartctl-ps3stor-1.0.0-1.el8.x86_64.rpm`
  - `smartctl-ps3stor-1.0.0-1.el8.aarch64.rpm`

> 提示：若 `centos:8` 多架构镜像日后被下架，可将工作流中 aarch64 对应的镜像改为 `arm64v8/centos:8`。

---

## 五、安装与使用（CentOS 8）

```bash
# 安装 RPM
sudo rpm -ivh smartctl-ps3stor-1.0.0-1.el8.x86_64.rpm

# 验证
which smartctl
smartctl --version

# 使用 ps3stor 设备类型读取 PS3 存储控制器信息
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1
sudo smartctl -x -d ps3stor,16 /dev/ctrl/1 -j   # JSON 输出
```

---

## 六、文件说明

| 文件 | 说明 |
| --- | --- |
| `smartctl-ps3stor.spec` | CentOS 8 RPM 打包 spec（包名 `smartctl-ps3stor`，版本 `1.0.0`） |
| `.github/workflows/build-centos8-rpm.yml` | 发布时自动构建 x86_64 / aarch64 两个 el8 RPM 的工作流 |
| `smartctl_ps3stor_README` | 原始英文编译与使用说明 |
| 其余文件 | smartmontools 7.4 完整源码（含 `ps3stor` 设备支持） |

---

## 七、许可证

遵循 smartmontools 上游许可 **GPLv2+**（见 `COPYING`）。
