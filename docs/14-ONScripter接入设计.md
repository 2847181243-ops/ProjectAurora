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

## 2. 三条路（读完 executeLabel 源码后修正）

@@@executeLabel()@@@ 的循环是**逐条解析脚本命令**，不是逐帧：

@@@cpp
while (current_line < current_label_info.num_of_lines) {
    if (SDL_PumpEvents(), SDL_PeepEvents(NULL, 1, SDL_PEEKEVENT, SDL_QUITMASK))
        endCommand();
    int ret = ScriptParser::parseLine();
    if (ret == RET_NOMATCH) ret = this->parseLine();
    ...
}
@@@

关键在于：引擎的"等待"发生在**命令内部**（显示文字后等点击、等待动画等），
主循环本身并不按帧返回。所以"每帧调一次 step"不是一个能简单切出来的函数——
要做就得把 wait/event 那一整套逻辑改成分段状态机，侵入性太大。

| 方案 | 做法 | 结论 |
|---|---|---|
| A. 引擎跑在自己的线程 | 主线程建 SDL 窗口，引擎线程跑 executeLabel，输入用 SDL_PushEvent 投递 | **采用**：不改上游，先跑起来 |
| B. 给引擎加单步接口 | 把 executeLabel 拆成每帧返回 | 否决：循环卡在命令内部，改动面比想象大得多 |
| C. 独立进程 | iOS 不可行 | 排除 |

也就是说：ONScripter 这类"自带主循环"的引擎，**平台让出控制权**，
模块负责自己的线程与窗口；平台只做生命周期、输入投递、音频与存储。
这与"平台驱动 tick"的模拟器模块是两种形态，ABI 里 @@@tick@@@ 是可选回调正是为此留的口子。

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
