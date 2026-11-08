# 02 Module SDK 技术规范

对应任务书 Phase 2 交付物 04、05、06。以 `SDK/AuroraRuntimeAPI/include/aurora/module_api.h`
和 `SDK/AuroraModuleSDK/include/aurora/manifest.h` 为准；本文解释为什么这样定，以及接入方要做什么。

---

## 1. ABI 版本策略

- `AURORA_ABI_MAJOR` / `AURORA_ABI_MINOR` 决定二进制兼容。
- 主版本不同：直接拒绝加载，返回 `AURORA_E_ABI_MISMATCH`。模块必须重新编译。
- 次版本升级：只允许**在结构体尾部追加字段**，并且每个结构体都带 `struct_size`。
  老模块的 `struct_size` 较小，平台按它判断字段到哪为止。
- 删除字段、改变字段含义、改变函数签名 = 主版本 +1。

`aurora_vtable_validate()` 在注册时就检查 `struct_size`、主版本号和必需函数指针；不合格的模块
不会进入注册表，而不是等到运行到一半才崩。

## 2. 模块入口

每个模块导出**一个**符号：

`c
AURORA_MODULE_ENTRY(ppsspp) { return &kTable; }   /* 导出的符号是 aurora_module_entry_ppsspp */
`

符号名带模块后缀，是为了两个静态库同时链接时不冲突。构建表由 `Modules/modules.list` 生成，
CMake 和 iOS 侧的 Python 生成器读同一份清单。

## 3. 生命周期函数语义

| 调用 | 前置状态 | 成功后的期望 | 常见错误 |
|---|---|---|---|
| `create_instance(host, &out)` | 无 | 返回实例句柄，登记 host 指针 | OUT_OF_MEMORY、INVALID_ARGUMENT |
| `load_game(inst, game)` | 已创建 | 校验格式、打开资源，不动线程 | UNSUPPORTED_GAME、MISSING_FIRMWARE、IO |
| `start(inst)` | 已加载 | 开始出帧 | INIT_FAILED、STATE |
| `pause` / `resume` | 运行中 | 停/继续出帧 | STATE |
| `stop(inst)` | 运行中 | 停循环、刷存档 | IO |
| `query_status(inst, &out)` | 任意 | 填帧数、fps、名字 | 无（失败返回 status） |
| `release_instance(inst)` | 任意 | 释放全部资源 | 无返回值 |
| `tick(inst, dt)`（可选） | 运行中 | 推进一帧 | STATE、INTERNAL |

约定：

1. **不得跨 ABI 抛异常。** 平台会包 try/catch，但那是兜底，不是许可。
2. **不得在 ABI 调用里长时间阻塞。** 阻塞超过一帧的时间会直接体现为掉帧。
3. **句柄由模块自己管理**，平台只保存指针，绝不 delete。
4. `release_instance` 之后平台不再访问该句柄；模块也不得保留 host 指针继续回调。

## 4. 错误码

| 码 | 含义 | 平台处理 |
|---|---|---|
| OK | 成功 | — |
| E_INVALID_ARGUMENT | 调用方 bug | 记录，视为模块缺陷 |
| E_UNSUPPORTED | 本模块没实现这个功能 | 降级，不算失败 |
| E_UNSUPPORTED_GAME | 不认这个游戏/格式 | 拆实例，模块回 READY |
| E_MISSING_FIRMWARE | 缺 BIOS/固件 | 拆实例，提示用户放置固件 |
| E_IO | 读写失败 | 拆实例，模块回 READY |
| E_OUT_OF_MEMORY | 内存不足 | 致命，置 FAILED |
| E_INIT_FAILED | 初始化失败 | 致命，置 FAILED |
| E_STATE | 状态非法 | 平台逻辑错误，置 FAILED |
| E_TIMEOUT | 超时 | 拆实例 |
| E_INTERNAL | 模块内部错误 | 致命，置 FAILED |
| E_CANCELLED | 用户取消 | 拆实例，模块回 READY |
| E_ABI_MISMATCH | 版本不符 | 拒绝加载 |

"致命"与"可重试"的分界线在 `isFatalStatus()` 里，只有一个地方定义，避免各处判断不一致。

## 5. Manifest 规范

### 5.1 字段

| 字段 | 类型 | 必填 | 校验 |
|---|---|---|---|
| `module_id` | string | 是 | 反域名，小写 `[a-z0-9_.]`，段非空，长度 3–128 |
| `name` | string | 是 | 非空 |
| `version` | string | 是 | MAJOR.MINOR.PATCH |
| `module_type` | string | 是 | console_emulator / galgame_engine / other |
| `platform` | string | 是 | `[a-z0-9_+-]{1,16}` |
| `adapter` | string | 是 | native / libretro / visual_novel / web |
| `entry_symbol` | string | 是 | 必须以 `aurora_module_entry_` 开头 |
| `license` | string | 是 | SPDX 标识符（如 GPL-2.0-or-later） |
| `sdk_version` | string | 是 | 编译时所用 SDK 版本 |
| `minimum_sdk_version` | string | 是 | 不得高于 `sdk_version` |
| `supported_formats` | string[] | 是 | 至少一项，无点号、无重复 |
| `capabilities` | object | 否 | 键必须是已知能力名，未知键只警告 |
| `dependencies` | string[] | 否 | 合法 module_id，不能自依赖，不能重复 |
| `status` | string | 否 | 仅记录用，不参与状态计算 |
| `upstream` | object | 否 | {project, version, commit} |

### 5.2 关于 status

Manifest 里的 `status` 只是作者自述。平台永远不会因为它是 "ready" 就显示 READY。
有专门的测试（`registry_ignores_a_claimed_ready_status`）盯这一条。

### 5.3 兼容判定

`checkSdkCompatibility()` 的规则：

1. `minimum_sdk_version` 必须 ≤ `sdk_version`（自相矛盾直接判无效）
2. 主版本必须与宿主相同
3. 宿主的次版本必须 ≥ 模块编译时版本

第 3 条容易被忽略：模块按照 1.2.0 编译，就不能跑在只提供 1.1.0 的宿主上，
否则它调用的新函数是空的。

## 6. 能力声明

能力是**声明**，不是保证。平台展示给用户前应当做运行时探测（例如 JIT 是否真的可用）。

| 能力 | 语义 | 备注 |
|---|---|---|
| save_state | 支持即时存档 | 存档走 States 桶 |
| game_saves | 支持游戏内存档 | 走 Saves 桶 |
| controller | 支持手柄 | — |
| touch_input | 支持触控 | — |
| keyboard | 支持键盘 | — |
| fast_forward | 支持快进 | — |
| audio | 有音频输出 | ABI 1.1 起有 audio_submit / audio_spec |
| video_output | 会出画面 | 必须配合 acquire_frame |
| jit_required | 没 JIT 就跑不动 | 必须在 UI 明说 |
| jit_optional | 有 JIT 更快 | — |
| shaders | 支持自定义着色器 | — |
| multiplayer | 支持联机 | — |
| requires_firmware | 需要外部 BIOS/固件 | 缺固件时状态停在 BUILT |

## 7. Host 服务约定

### 7.1 storage_directory

- 返回绝对路径，UTF-8，NUL 结尾
- 只能访问自己的桶；传别人的 module_id 会被拒绝并记违规
- 相对路径不能是绝对路径、不能含 `..`、不能含盘符和反斜杠
- 缓冲区不足返回 INVALID_ARGUMENT，不截断

### 7.2 acquire_frame / present_frame

- 宿主拥有缓冲，模块只写
- 请求尺寸必须与宿主配置一致，否则 INVALID_ARGUMENT
- 无显示环境返回 UNSUPPORTED，**模块必须继续运行**（日志里说一次即可）

### 7.3 poll_input

返回当前输入快照。触控坐标是帧缓冲像素坐标，越界由模块自己判断。

### 7.4 write_log

日志统一走宿主，模块不写自己的日志文件；平台决定落到哪。

## 8. 接入一个新模块要做的事

1. 在 `Modules/<Name>/src/` 下实现 vtable
2. 在 `Modules/modules.list` 加一行 `module_id|entry_symbol|目录名`
3. 在 `ModuleManifests/` 加一份 JSON
4. 跑 `ctest`
5. 提交 PR，说明新增依赖与许可证

平台层、注册表、游戏库、存档管理都不需要改。这条"不用改"是任务书 13.5 的验收点，
不是口头承诺——现有三个模块就是按这个流程加进来的。

## 9. 音频（ABI 1.1 起）

平台拥有输出设备与混音器，模块只交帧，不自己开设备。

@@@c
typedef struct AuroraAudioBuffer {
    uint32_t struct_size;
    const void* samples;      /* 交错采样，frame_count * channels 个值 */
    uint32_t frame_count;
    uint32_t sample_rate;
    uint32_t channels;        /* 1–8 */
    AuroraAudioFormat format; /* S16 或 F32 */
} AuroraAudioBuffer;
@@@

宿主提供 @@@audio_spec()@@@（自己的输出格式）与 @@@audio_submit()@@@。约定：

- 采样率由模块决定（平台负责重采样），格式必须是 S16 或 F32
- 队列满时平台丢帧，模块必须容忍，不能因此报错
- 模块先用 @@@host->struct_size@@@ 判断这两个回调是否存在（1.0 宿主没有）
- 无输出设备的宿主（测试、CLI）照样接收并计数，模块感知不到差别

## 10. 输入映射（数据驱动）

输入配置是 JSON，不是代码：

$json
{
  "name": "default",
  "bindings": [
    {"action": "a", "pad_mask": 1, "key_code": 90},
    {"action": "start", "touch": {"x": 0.4, "y": 0.9, "w": 0.2, "h": 0.1}}
  ]
}
$

- $action$ 是逻辑按钮（a/b/x/y/up/down/left/right/start/select/l1/r1）
- $pad_mask$ 按 $AuroraButton$ 位；$key_code$ 是键盘码；$touch$ 是归一化区域
- 平台把 OS 事件转成 $RawInputEvent$，再 $applyProfile()$ 出 $AuroraInputState$
- 触控坐标会换算成帧缓冲像素，模块拿到的就是它要的坐标系

一套配置对所有模块生效，用户配一次手柄不用每个引擎重来。

## 11. 显示策略

$computeViewport()$ 是个纯函数：给定帧尺寸、视图尺寸、模式，算出画面落在哪。

| 模式 | 行为 |
|---|---|
| fit | 完整显示，留边 |
| fill | 铺满，裁掉溢出 |
| integer | 整数倍放大，像素不抖 |
| stretch | 拉伸（只用于调试） |

默认 nearest 采样（像素画），要模糊再开 linear。

## 12. 尚未定义的部分

- 音频：ABI 已定义（1.1），宿主侧已实现并按帧计数；iOS 上接 AVAudioEngine 的输出还没做。
- 输入：映射层已实现（配置文件 + 默认触屏布局），手柄死区与轴映射还没做。
- 着色器/滤镜链：Display 层还没实现。
- 帧同步与垂直同步：现在由模块自己控制节奏，tick 由平台驱动。
