# KiriKiri2 接入记录

> 引擎：KrKr2-Next（https://github.com/reAAAq/KrKr2-Next）
> 平台：iOS 16.0+ / arm64
> 状态：设备版编译、链接、归档通过；模拟器版构建中

## 1 为什么先做这个引擎

ONScripter 的引擎是阻塞主循环，接入时要和宿主抢线程；KiriKiri 2 的 `bridge/engine_api` 是 tick 驱动，
`engine_tick(handle, delta_ms)` + `engine_read_frame_rgba`，和 Aurora 的 `tick` / `acquire_frame` /
`present_frame` 是一一对应的，不需要为适配写平台补丁。

## 2 构建

- 依赖：vcpkg manifest（136 个包，含 ANGLE(Metal)、OpenCV、Boost、SDL2、libpng、bzip2、lzma、opusfile）
- triplet：上游自带 `vcpkg/triplets/arm64-ios.cmake`
- preset：`iOS Release Config`（`CMAKE_SYSTEM_NAME=iOS`，arm64，部署目标 15.0）
- 产物：`bridge/engine_api/libengine_api.a`，737 个目标全编过

### 2.1 必须打的三个补丁

| 现象 | 原因 | 处理 |
|---|---|---|
| `No SOURCES given to target: CubismFramework` | Live2D Cubism SDK 是专有的，仓库里被 gitignore，`file(GLOB_RECURSE)` 拿到空列表 | 把 `add_library(CubismFramework STATIC ...)` 改成 INTERFACE 目标，所有 `PUBLIC`/`PRIVATE` 降为 `INTERFACE` |
| `'CubismFramework.hpp' file not found` | 源列表仍挂着 `krkrlive2d.cpp` | 从所有 `CMakeLists.txt` 删掉该行，并删除源文件（有的目标用通配抓源码） |
| `require bison 3.8.2, but have 2.3` | macOS 自带 bison 停在 2006 年 | `brew install bison`，显式 `-DBISON_EXECUTABLE=$(brew --prefix bison)/bin/bison` |

另外要装 `autoconf automake autoconf-archive`（gperf 要求）和 `ninja`（preset 指定的生成器）。

### 2.2 链接期的三个坑

1. **符号缺失**：`TVPRegisterKrkrLive2DPluginAnchor` / `g_live2dRenderTarget` 仍被引用
   —— 宏 `KRKR2_LIVE2D` 在别处也被定义。解法是 App 侧提供惰性实现（`krkr_live2d_stub.cpp`），
   结构体布局按 `krkrgles.cpp` 的声明对齐（`GLuint` → `unsigned int`，`GLsizei` → `int`）。
2. **CoreMedia 符号**：OpenCV 拖进来的 ffmpeg 有 VideoToolbox 编解码器，需要
   `CoreMedia / VideoToolbox / CoreImage / CoreServices` 框架。
3. **库顺序**：30 多个静态库按字母序链会报未定义符号。用 `libtool -static` 合成一个
   `libkrkrell.a`（774 MB）后顺序问题消失；最终 App 只有 37.7 MB，说明链接器只拉被引用的目标文件。

## 3 适配层

`Modules/KiriKiri/src/`：

- `krkr_engine.h`：接缝（`create` / `openGame` / `tick` / `setSurfaceSize` / `destroy`）
- `krkr_engine.cpp`：真实实现，直接调用 `engine_api.h`
- `krkr_engine_stub.cpp`：未链接引擎时返回 UNSUPPORTED
- `krkr_module.cpp`：ABI 适配，tick 里推进引擎、按 stride 拷帧到宿主 surface 再 present
- `krkr_live2d_stub.cpp`：Live2D 符号的惰性实现

## 4 证据

| 内容 | 位置 |
|---|---|
| 引擎构建（737/737 目标） | `ci-status/krkr-run11.md` |
| App 链接成功 + ipa | `ci-status/ios-run182.md` |
| 可执行文件内的模块 id | `com.aurora.runtime.kirikiri`，与 `engine_create`、`engine_tick` 同处 |
| 冒烟素材 | `TestGames/minimal-krkr/startup.tjs` |

## 5 未完成

- 模拟器运行验证（引擎正在为 `arm64-iossimulator` 重建）
- 真机安装（需要签名，等本人）
- 游戏内容覆盖：目前只有 `startup.tjs` 的最小用例，`.xp3` 打包、`Config.tjs`、脚本归档都未验证
