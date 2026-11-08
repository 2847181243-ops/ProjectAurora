# 14 ONScripter 接入设计

## 1. 引擎的运行方式（读源码得到的事实）

$onscripter.cpp$ 的入口是：

$cpp
ONScripterLabel ons;
parseOptionFile(CFG_FILE, ons, hasArchivePath);
parseOptions(argc, argv, ons, hasArchivePath);
for (;;) {
    if (ons.init()) exit(-1);
    ons.executeLabel();   // 整个游戏循环，阻塞到脚本结束
    SDL_Quit();
}
$

$executeLabel()$ 不返回，直到脚本结束或收到退出。这是**阻塞式主循环**，
和我们"平台每帧调 tick"的模型天然冲突。

参考实现（2017 年的 ONScripter-iOS 移植，third_party/onscripter-ios-old）是这么做的：

$objc
_onscripter = new ONScripter();
_onscripter->setArchivePath(...);
_onscripter->setSaveDir(...);
_onscripter->openScript();
_onscripter->init();
SDL_iPhoneSetEventPump(SDL_TRUE);
_onscripter->executeLabel();   // 阻塞
$

它依赖 SDL 1.2 的 $SDL_iPhoneSetEventPump$（SDL2 里没有）。

## 2. 三条路

| 方案 | 做法 | 代价 |
|---|---|---|
| A. 整段跑在后台线程 | 主线程初始化 SDL 视频/音频，后台线程跑 executeLabel | SDL2 的 iOS 事件泵在主线程，跨线程渲染有风险，需要实测 |
| B. 给引擎加单步接口 | 打补丁把 executeLabel 拆成 stepFrame()，平台每帧调一次 | 要维护上游补丁，但和我们的 tick 模型完全契合 |
| C. 独立进程/子进程 | iOS 上不可行 | 排除 |

**倾向 B**，理由：我们的 ABI 本来就是 $tick(inst, dt)$ 驱动的；
单步接口让 ONScripter 与模拟器模块走同一条路，也让"确定性重放"成为可能。
补丁范围初步估计在 $ONScripterLabel::executeLabel$ 的主循环入口，
把"直到 quit 才返回"改成"跑一帧就返回"。

先做 A 验证可行性（不改上游），拿到真机表现后再决定是否投入 B。

## 3. 与 Aurora 的接线

$
Aurora App (SwiftUI)
  └─ 模块 com.aurora.runtime.onscripter
       ├─ create_instance: 存 host 指针
       ├─ load_game: 记录脚本目录
       ├─ start: 主线程 SDL_Init(VIDEO|AUDIO)，起引擎线程
       ├─ tick: 帧计数 / 状态汇报（方案 B 后改为驱动引擎）
       └─ stop: 投递 SDL_QUIT，等线程退出
$

画面：方案 A 阶段由 SDL 自己开窗；方案 B 阶段改为引擎把像素交回
$acquire_frame$，走我们统一的 Metal 通道。
音频：先用 SDL_mixer 的 CoreAudio 输出，后续迁到 $audio_submit$。

## 4. 当前阻塞

- SDL 全套（SDL2 + sdl12-compat + image + ttf + mixer）的 iOS 构建 spike 在跑
- GitHub API 时不时连不上，需要重试；构建产物走 artifact 下载可用
