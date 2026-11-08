# 09 PPSSPP 接入调研

结论先行：**PPSSPP 能接，但不是"包一层适配器"就完事的模块，是一个独立工程。**
这一份把已经核实的源码事实、接入路线、成本和风险写清楚，避免用乐观估计推进。

## 1. 已核实的源码事实（2026-11-08，upstream master）

- iOS 目标：$if(IOS AND NOT LIBRETRO) set(NO_OPENGL ON)$ —— 官方 iOS 版是 **Metal only**
  （GPU/Metal 存在，GLES / Vulkan / D3D11 / Software 也都在）
- 官方构建产物是**应用**，不是可复用库：CMakeLists 里是
  $add_executable($${TargetBin} MACOSX_BUNDLE ... $${NativeAppSource})$，
  源码直接列在 CMakeLists 中，**没有 Core 这类可单独链接的静态库目标**
- 嵌入式入口是 **libretro 核心**：$-DLIBRETRO=ON$ 会构建 libretro 目录下的核心目标
- 依赖规模：.gitmodules 里 21 个子模块（glslang、SPIRV-Cross、zstd、freetype、
  rapidjson、armips、libchdr、rcheevos、OpenXR-SDK 等），完整构建要全部拉取
- 授权：GPL-2.0-or-later，与本项目 GPL-3.0-or-later 兼容

## 2. 三条路，选哪条

| 路线 | 说明 | 判断 |
|---|---|---|
| A. 直接嵌官方 iOS 应用的核心代码 | 需要 fork 上游，把源码集合拆成库，自己实现 UI 和 Metal 上下文 | 改动面大，跟随上游成本高 |
| B. libretro 核心 + 自研前端 | 上游留的入口，前端回调由我们实现 | **推荐**，边界干净，符合 LibretroAdapter 设计 |
| C. 重写模拟器 | 不现实 | 排除 |

## 3. 选 B 之后的具体工作

1. **构建**：CMake 交叉编译 libretro 核心到 iOS arm64（静态库），随 App 链接。
   已挂非阻塞 spike workflow 验证这一步。
2. **前端**：在模块里实现 retro_set_environment / retro_set_video_refresh /
   retro_set_audio_sample_batch / retro_set_input_poll / retro_set_input_state。
3. **视频**：核心会要求 GL/Vulkan 硬件渲染上下文。我们的通道是 RGBA → Metal 纹理：
   - 先让核心走软件渲染回调，像素直接进我们的通道（简单，PSP 软渲染慢）
   - 性能不够再给核心真实 GL/Vulkan 上下文
4. **音频**：**我们还没有音频 ABI**，这是硬前置。
5. **存档**：SRAM / State 目录映射到模块自己的 Saves / States 桶。
6. **输入**：retro_input_state 接 AuroraInputState。

## 4. 性能与 JIT

PPSSPP 官方说 PSP 在现在的 iOS 设备上不开 JIT 也能全速，但那是它自己 Metal 后端
加 ARM64 解释器的结论。走 libretro 软件渲染路径时，瓶颈会变成软件光栅化，
不是 CPU 模拟。这一条必须实测，不能拿官方结论套。

## 5. 成本估计

| 阶段 | 内容 | 估计 |
|---|---|---|
| P1 | 交叉编译核心并链接进 App | 1–2 天 |
| P2 | 前端回调 + 软件帧进 Metal 通道 | 2–3 天 |
| P3 | 音频 ABI + 音频输出 | 1–2 天 |
| P4 | 输入映射、存档、BIOS 无关性 | 1–2 天 |
| P5 | 真机跑通一个游戏并测帧率 | 1–2 天 |

合计约两周，P1 有不确定性（21 个子模块 + 交叉编译），所以先 spike。

## 6. 建议

先跑 spike。编不出来就不接 PPSSPP，改接一个更小的、纯软件渲染的引擎，
先把"真实模块接入"这条主线走通，再回头啃模拟器。
