# KiriKiri2 接入记录

> 引擎：KrKr2-Next（https://github.com/reAAAq/KrKr2-Next）
> 平台：iOS 16.0+ / arm64
> 状态：设备版编译、链接、归档通过；模拟器里引擎初始化、开游戏、执行 TJS 全部实测通过

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


## 6 模拟器实测（2026-11-13）

证据：`docs/evidence/kirikiri-simulator-running.png`、`kirikiri-simulator-sim.log`。

```
[2026-11-13 00:28:21.881] [core] [debug] EngineBootstrap: starting initialization
[2026-11-13 00:28:29.722] [core] [info] EGL initialized: version 1.5
[2026-11-13 00:28:29.722] [core] [info] EGL vendor: Google Inc. (Apple)
[2026-11-13 00:28:29.722] [core] [info] EGL version string: 1.5 (ANGLE 2.1.7258 git hash: d9fc4a372074)
[2026-11-13 00:28:29.759] [core] [info] ANGLE EGL context created successfully: 1280x720
[2026-11-13 00:28:29.763] [core] [info] GL_RENDERER: ANGLE (Apple, ANGLE Metal Renderer: Apple iOS simulator GPU, Version 26.2 (Build 23C54))
[2026-11-13 00:28:29.767] [core] [info] EngineBootstrap: ANGLE EGL context ready
[2026-11-13 00:28:29.768] [core] [info] EngineBootstrap: initialization complete (1280x720)
[2026-11-13 00:28:29.769] [core] [info] engine_open_game: runtime initialized, starting application with path: /Users/runner/Library/Developer/CoreSimulator/Devices/98A0BFA3-CEC3-4FD6-9916-5A2149774DBF/data/Containers/Data/Application/2B48E5CF-CB42-4CF7-9E32-736B387D25D2/Library/Application Support/AuroraData/Games/minimal-krkr (normalized: /Users/runner/Library/Developer/CoreSimulator/Devices/98A0BFA3-CEC3-4FD6-9916-5A2149774DBF/data/Containers/Data/Application/2B48E5CF-CB42-4CF7-9E32-736B387D25D2/Library/Application Support/AuroraData/Games/minimal-krkr/)
[2026-11-13 00:28:29.788] [core] [debug] engine_open_game: calling Application->StartApplication...
[2026-11-13 00:28:29.788] [core] [debug] StartApplication: normalizing storage path...
[2026-11-13 00:28:29.788] [core] [debug] StartApplication: TVPInitScriptEngine...
[2026-11-13 00:28:29.793] [core] [info] [TVP Console]  (info) Data path : file://./users/runner/library/developer/coresimulator/devices/98a0bfa3-cec3-4fd6-9916-5a2149774dbf/data/containers/data/application/2b48e5cf-cb42-4cf7-9e32-736b387d25d2/library/application support/auroradata/games/minimal-krkr/savedata/
[2026-11-13 00:28:29.809] [core] [debug] StartApplication: TVPInitFontNames...
```

界面里 `minimal-krkr` 与 `KiriKiri2` 都显示 RUNNING，Stop 按钮可用。

### 6.1 模拟器这条路踩过的四个坑

| 现象 | 根因 | 处理 |
|---|---|---|
| `Float16ToFloat32.cpp.o built for 'iOS'` | 模拟器 triplet 由 sed 生成，但上游文件没有 `VCPKG_OSX_SYSROOT` 行，sed 静默无效，vcpkg 全程按 iphoneos 编 | 改成显式 `echo 'set(VCPKG_OSX_SYSROOT iphonesimulator)' >> triplet` |
| `glib: pipe2 undeclared` | Darwin 只在完整特性集下声明 pipe2，glib 的包含顺序把它挡住了 | 给仓库自带的 glib port 打补丁，`#ifdef HAVE_PIPE2` → `#if 0` |
| portfile 语法损坏（宿主 glib 也挂） | 补丁用正则插在 `vcpkg_extract_source_archive(` 之后，而该调用是跨行的，插进了参数列表 | 括号配平定位，插在调用结束后 |
| `onscripter.a(...) found architecture 'x86_64'` | ONScripter 的引擎对象用写死的 `-arch x86_64` 编译 | 改成 arm64，与三方库统一 |

### 6.2 测试脚本的时序坑

引擎把 `startup.tjs` 执行在 `system/initialize.tjs` **之前**，那个时刻 `Window`/`Layer` 不可用，
`new Layer(...)` 会抛异常并让 `engine_open_game` 返回失败。冒烟脚本因此只调用
`Debug.message()`——能打出这一行就说明 TJS 虚拟机在跑。

## 7 仍然缺的

- 画面呈现：引擎渲染到自己的 layer，宿主这边只做了 `read_frame_rgba` 取帧，尚未把它接到 SwiftUI 的显示视图（和 ONScripter 同款遗留项）
- 真实游戏：只验证了 `startup.tjs`，`.xp3` 封包、`Config.tjs`、存档路径都未实测
- 真机安装：需要签名，等本人


## 8 画面通路（2026-11-13 补）

截图：`docs/evidence/kirikiri-simulator-frames.png`。

界面上 `Runtime output` 面板位于库列表下方，显示一块 1280×720（16:9）的黑色画面，
底下一行 `frames presented: 24 audio 48 kHz`。

黑色是因为冒烟脚本只打日志、不画东西，面板本身证明的是通路：

```
引擎渲染帧 → engine_read_frame_rgba → 模块 present_frame → 宿主 surface_pixels
→ aurora_bridge_frame_data → Metal texture → MTKView 显示
```

顺手修掉的两个问题：

1. **帧序号恒为 0**：模块的 `query_status` 里写死 `frames_presented = 0`，而
   `MetalFrameView` 靠序号去重（`sequence != uploadedSequence` 才重新上传纹理），
   所以只有第一帧被上传、之后永远不刷新。改成模块自己数帧。
2. **面板在列表最底部**：原来要滚动才能看到，改成有模块运行时提到库列表下方。

## 9 下一步

- 让测试游戏真的画点东西（`Window`/`Layer` 要等 `system/initialize.tjs` 之后才可用）
- 用真实 `.xp3` 游戏验证解码、脚本、存档


## 10 真实归档验证（2026-11-13）

用 AetherKiri 的 KAG3 演示包（`data.xp3`，17 MB，63 个条目）做真实归档测试。
探针在引擎实际解析的那个流上取到了数据：

| 脚本 | 大小 | 前 8 字节 | 判定 |
|---|---|---|---|
| `startup.tjs` | 449 | `2f 2f 20 73 74 61 72 74`（`// start`） | 正确 |
| `system_polyfill/polyfillinitialize.tjs` | 1637 | `ef bb bf 0a 40 69 66 20`（UTF-8 BOM + `@if `） | 正确 |

结论：

1. **xp3 挂载与读取完全正确**——文件定位、大小、字节都对得上；
2. 该演示包的脚本用了 **`@if` 预处理器语法**，vanilla KiriKiri2 的 TJS2 解析器不支持，报 `syntax error`；
3. 解析失败后引擎**没有干净地终止**，而是在 `TypeOfMemberDirect` 里 SIGSEGV，把宿主一起带走。

第 3 条用 `TestGames/broken-krkr`（故意写错的 `@if`）复现时，引擎**没有崩**：
模块如实返回 `io_error`，宿主进程存活，界面状态回到 READY。

所以"坏脚本直接弄崩宿主"这个结论不成立。为定位它做了四组最小复现，全部**没有崩**：

| 复现 | 形状 | 结果 |
|---|---|---|
| `broken-krkr` | startup.tjs 里直接写 `@if` | 返回 io_error，进程存活 |
| `bad-nested` | startup.tjs `execStorage` 一个坏脚本 | 两个脚本都加载，进程存活 |
| `bad-bom` | 嵌套 + UTF-8 BOM + `@if/@endif` | 同上 |
| `bad-then-good` | 坏脚本之后接着 exec 一个好脚本 | 同上，第二个脚本正常执行 |

四组都覆盖了演示包 `startup.tjs` 的结构（"exec 一个失败的脚本，然后继续 exec 下一个"），
说明崩溃需要该包更具体的内容，不是脚本结构本身。

**结论**：该演示包崩溃按"第三方包不兼容、无法独立复现"记录，不再耗轮次；
vanilla KiriKiri2 兼容的包（纯 TJS、无 `@if` 预处理器）不受影响，已由
`minimal-krkr` 验证。


### 10.1 演示包失败的真正原因是预处理器

把归档里的 `system_polyfill/PolyfillInitialize.tjs` 原文件取出来，开头是：

```tjs
﻿
@if (kirikiriz)
property _dummyProp { getter {} setter (v) {} }
...
@endif
```

`@if` 是 KiriKiri 的**脚本预处理器**指令（KAG3 的 `Config.tjs` 也大量使用）。
当前构建的 TJS2 在解析到它时报 `syntax error`——预处理器没有生效。

这意味着：

- **纯 TJS 的游戏可以跑**（`minimal-krkr` 已验证）；
- **依赖 `@if` 的 KAG3 风格脚本跑不了**，这会影响相当一部分吉里吉里游戏。

这是一个功能缺口，不是本项目的 bug，也不在这次修复范围内：要支持它需要给 TJS2 接上预处理器
（或在执行前预处理脚本），工作量与风险都不小，需要单独评估。
