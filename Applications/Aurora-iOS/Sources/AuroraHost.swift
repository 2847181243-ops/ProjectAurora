// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

import Foundation
import Metal
import SwiftUI
import UIKit

struct GameRow: Identifiable {
    let id: String
    let title: String
    let module: String
    let state: String
    let detail: String
    let path: String
    let format: String
}

struct ModuleRow: Identifiable {
    let id: String
    let name: String
    let platform: String
    let detail: String
    let state: Int32
    let enabled: Bool
    let compiled: Bool
    let frames: Int32

    var stateName: String {
        guard let pointer = aurora_bridge_state_name(state) else { return "UNKNOWN" }
        return String(cString: pointer)
    }

    var isReady: Bool { state == 4 }
    var isRunning: Bool { state == 5 }
    var isFailed: Bool { state == 7 }
    var isDisabled: Bool { state == 6 }
}

/* Thin model around the C bridge. It owns no logic of its own: the registry and
 * the supervisor are the source of truth, this class only mirrors them. */
final class AuroraHost: ObservableObject {
    @Published var rows: [ModuleRow] = []
    @Published var games: [GameRow] = []
    @Published var frame: UIImage?
    @Published var runningModule: String = ""
    @Published var diagnostics: String = ""

    /* Automated runs (CI simulator, scripted device checks) set AURORA_AUTOSTART to
     * a module id; the bundled smoke-test game is copied in first. */

    /* Every bundled smoke-test game is copied in once, so a module added later does
     * not need its own branch here. Existing folders are left alone: a test run must
     * not overwrite saves the previous run wrote. */
    private func copyBundledTestGames() {
        let manager = FileManager.default
        guard let bundledRoot = Bundle.main.resourceURL?.appendingPathComponent("TestGames") else { return }
        let root = URL(fileURLWithPath: gamesRoot)
        try? manager.createDirectory(at: root, withIntermediateDirectories: true)
        let entries = (try? manager.contentsOfDirectory(at: bundledRoot, includingPropertiesForKeys: nil)) ?? []
        for entry in entries {
            let destination = root.appendingPathComponent(entry.lastPathComponent)
            if manager.fileExists(atPath: destination.path) { continue }
            try? manager.copyItem(at: entry, to: destination)
            makeWritable(destination)
        }
    }


    /* Bundle resources are copied with their read-only permissions, and an engine
     * that wants to create a save folder inside the game tree then fails with a
     * plain "io_error". Give the copy a writable top level and make the save
     * folder up front, which is the only subdirectory an engine creates itself. */
    private func makeWritable(_ root: URL) {
        let manager = FileManager.default
        try? manager.setAttributes([.posixPermissions: 0o755], ofItemAtPath: root.path)
        try? manager.createDirectory(at: root.appendingPathComponent("savedata"),
                                     withIntermediateDirectories: true)
    }

    private func autostartIfRequested() {
        guard let module = ProcessInfo.processInfo.environment["AURORA_AUTOSTART"], !module.isEmpty else { return }
        copyBundledTestGames()
        rescanLibrary()
        BootLog.mark("library scanned: \(games.count) game(s)")
        /* AURORA_AUTOSTART_GAME picks one game by folder name; without it the first
         * game that claims the module wins, which is fine for single-fixture runs. */
        let wanted = ProcessInfo.processInfo.environment["AURORA_AUTOSTART_GAME"] ?? ""
        BootLog.mark("autostart \(module) game=\(wanted.isEmpty ? "first" : wanted)")
        if let game = games.first(where: { $0.module == module && (wanted.isEmpty || $0.title == wanted) }) {
            _ = aurora_bridge_start(module, game.path, game.format)
            NSLog("[aurora] autostart \(module) with \(game.path)")
        } else {
            NSLog("[aurora] autostart: no library entry for \(module)")
        }
        refresh()
    }

    func refreshDiagnostics() {
        diagnostics = String(cString: aurora_bridge_diagnostics(25))
    }
    @Published var framesPresented: Int32 = 0
    @Published var frameWidth: Int = 0
    @Published var frameHeight: Int = 0
    @Published var lastError: String = ""

    /* Metal keeps the pixels on the GPU: the module writes RGBA and the view
     * uploads it straight into a texture. The UIImage path is only a fallback for
     * a device without Metal, which in practice means "never". */
    let useMetal = MTLCreateSystemDefaultDevice() != nil

    private var initialized = false
    private var timer: Timer?

    func start() {
        guard !initialized else { return }
        initialized = true

        let support = FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask)[0]
        let root = support.appendingPathComponent("AuroraData", isDirectory: true)
        try? FileManager.default.createDirectory(at: root, withIntermediateDirectories: true)

        _ = aurora_bridge_init(root.path, nil)
        refresh()

        autostartIfRequested()

        let ticker = Timer(timeInterval: 1.0 / 30.0, repeats: true) { [weak self] _ in
            self?.tick()
        }
        RunLoop.main.add(ticker, forMode: .common)
        timer = ticker
    }

    func refresh() {
        BootLog.mark("refresh")
        let count = aurora_bridge_module_count()
        var list: [ModuleRow] = []
        var index: Int32 = 0
        while index < count {
            list.append(ModuleRow(
                id: String(cString: aurora_bridge_module_id(index)),
                name: String(cString: aurora_bridge_module_name(index)),
                platform: String(cString: aurora_bridge_module_platform(index)),
                detail: String(cString: aurora_bridge_module_detail(index)),
                state: aurora_bridge_module_state(index),
                enabled: aurora_bridge_module_enabled(index) != 0,
                compiled: aurora_bridge_module_compiled(index) != 0,
                frames: aurora_bridge_module_frames(index)
            ))
            index += 1
        }
        rows = list
        runningModule = String(cString: aurora_bridge_running_module())
        lastError = String(cString: aurora_bridge_last_error())
        rescanLibrary()
        if runningModule.isEmpty {
            frame = nil
            framesPresented = 0
        }
    }

    /* Library ------------------------------------------------------------- */

    var gamesRoot: String { String(cString: aurora_bridge_games_root()) }

    func rescanLibrary() {
        _ = aurora_bridge_library_scan()
        let count = aurora_bridge_library_count()
        var list: [GameRow] = []
        var index: Int32 = 0
        while index < count {
            let title = String(cString: aurora_bridge_library_title(index))
            let module = String(cString: aurora_bridge_library_module(index))
            list.append(GameRow(
                id: "\(index)-\(title)",
                title: title,
                module: module,
                state: String(cString: aurora_bridge_library_state(index)),
                detail: String(cString: aurora_bridge_library_detail(index)),
                path: String(cString: aurora_bridge_library_path(index)),
                format: String(cString: aurora_bridge_library_format(index))
            ))
            index += 1
        }
        games = list
    }

    func importGames(from urls: [URL]) {
        for url in urls {
            let accessing = url.startAccessingSecurityScopedResource()
            _ = url.path.withCString { aurora_bridge_library_import($0) }
            if accessing { url.stopAccessingSecurityScopedResource() }
        }
        rescanLibrary()
    }

    func setEnabled(_ row: ModuleRow, enabled: Bool) {
        _ = aurora_bridge_set_enabled(row.id, enabled ? 1 : 0)
        refresh()
    }

    func run(_ row: ModuleRow) {
        /* Hand the module the first game that belongs to it: ONScripter cannot
         * start without knowing which script directory to open. */
        let game = games.first { $0.module == row.id }
        BootLog.mark("manual run \(row.id) game=\(game?.title ?? "none")")
        let status = game == nil
            ? aurora_bridge_start(row.id, nil, nil)
            : aurora_bridge_start(row.id, game!.path, game!.format)
        BootLog.mark("start returned \(status)")
        if status != 0 {
            lastError = String(cString: aurora_bridge_last_error())
            BootLog.mark("start error: \(lastError)")
        }
        refresh()
    }

    func stopRunning() {
        if !runningModule.isEmpty {
            _ = aurora_bridge_stop(runningModule)
        }
        refresh()
    }

    func resetFailure(_ row: ModuleRow) {
        _ = aurora_bridge_reset_failure(row.id)
        refresh()
    }

    private func tick() {
        refreshDiagnostics()
        guard !runningModule.isEmpty else { return }
        aurora_bridge_tick(1.0 / 30.0)
        framesPresented = aurora_bridge_frames_presented()
        frameWidth = Int(aurora_bridge_frame_width())
        frameHeight = Int(aurora_bridge_frame_height())
        if !useMetal { updateFrame() }
    }

    private func updateFrame() {
        guard let pointer = aurora_bridge_frame_data() else { return }
        let width = Int(aurora_bridge_frame_width())
        let height = Int(aurora_bridge_frame_height())
        guard width > 0, height > 0 else { return }

        let data = Data(bytes: pointer, count: width * height * 4)
        guard let provider = CGDataProvider(data: data as CFData) else { return }
        let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)
        guard let image = CGImage(
            width: width,
            height: height,
            bitsPerComponent: 8,
            bitsPerPixel: 32,
            bytesPerRow: width * 4,
            space: CGColorSpaceCreateDeviceRGB(),
            bitmapInfo: bitmapInfo,
            provider: provider,
            decode: nil,
            shouldInterpolate: false,
            intent: .defaultIntent
        ) else { return }
        frame = UIImage(cgImage: image)
    }
}
