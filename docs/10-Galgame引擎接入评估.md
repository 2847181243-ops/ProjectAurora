# 10 ONScripter 与 KiriKiri 接入评估

按"先做 krkr 和 ons"的要求，这一份先把两个引擎的可移植性核清楚。
结论：**两个都不是"小模块"，ONScripter 可做，KiriKiri 是另一个量级的工程。**

## 1. ONScripter（已克隆核对：Galladite27/ONScripter-EN）

| 项 | 事实 |
|---|---|
| 代码量 | 4698 个文件，105 个 .cpp |
| 官方支持平台 | **只有 Windows(MSYS2) 和 Linux**（BuildInstructions.md 原话） |
| 依赖 | SDL、SDL_image、SDL_ttf、SDL_mixer、smpeg、libogg、libvorbis、freetype、bzip2、iconv、zlib |
| SDL 版本 | 依赖名对应的是 SDL1.2 时代的一套（mingw 的 SDL / SDL_ttf / SDL_mixer 包） |
| 历史移植 | 仓库里带 $macosx/$（老 Xcode 工程 + 自带 SDL），说明有人做过桌面移植 |
| 授权 | GPLv2 系列，与本项目兼容（需最终核对源码头） |

**结论：可以做，但不是"接一个库"。** 具体障碍：

1. SDL1.2 及其配套（image/ttf/mixer/smpeg）在 iOS 上没有现成的现代构建
2. 更省事的路是迁到 **SDL2**（SDL2_image / SDL2_ttf / SDL2_mixer 都有官方 iOS 支持），
   但 SDL1.2 → SDL2 有 API 差异，需要打一组补丁并长期维护
3. smpeg 是 MPEG1 解码，iOS 上要么换实现，要么关掉视频播放
4. 音频输出要走我们的 ABI（目前还没有音频 ABI）

## 2. KiriKiri（已核对：wamsoft/krkrz）

| 项 | 事实 |
|---|---|
| 代码量 | 仓库约 35 MB，C++ |
| 授权 | GitHub 识别为 NOASSERTION（自有条款），需人工核对能否再分发 |
| 平台耦合 | Windows 为中心：Win32 窗口、DirectX、插件 DLL 模型 |
| iOS 移植 | 没有现成的 |

**结论：比 PPSSPP 更难。** 它没有为嵌入设计过，也没有跨平台抽象层。
真要上 iOS，等于重写平台层（窗口/输入/音频/文件）并替换 DirectX 后端，是月级别的工程。

## 3. 建议顺序

1. **ONScripter**（先做）：先做"无渲染的编译验证"——用 SDL2 + 最小补丁把 105 个 .cpp
   编到 iOS arm64 静态库，证明依赖栈能过；再补渲染、音频、脚本资源加载。
2. **KiriKiri**（后做）：先只做**可行性结论**，不写代码。一旦确认要在 iOS 上跑 krkr，
   必然是独立项目，需要单独立项与排期。

## 4. ONScripter 的第一步（本轮开始做）

- 在 CI 上加一个 spike：为 iOS arm64 构建 SDL2 + SDL2_image + SDL2_ttf + SDL2_mixer 静态库
- 然后让 ONScripter 的源文件针对这套头文件编译，先只要求"能过编译"，
  渲染和脚本运行放到后面
- 产物作为 artifact 留存，供后续把它接成 $com.aurora.runtime.onscripter$ 模块

## 5. 与架构的关系

这两个引擎都会走 VisualNovelAdapter 这条路。ONScripter 接进来之后，
"适配器 + 模块"这条线才算有真东西，而不是只有 Native 一种。
