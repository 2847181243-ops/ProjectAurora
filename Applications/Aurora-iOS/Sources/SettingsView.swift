// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI
import UIKit

/* Anything a player never needs lives here: where games are stored, and — behind
 * a five-tap unlock — the module list, log and diagnostics used for debugging. */
struct SettingsView: View {
    @ObservedObject var host: AuroraHost
    @Environment(\.dismiss) private var dismiss
    @State private var showImporter = false
    @State private var developerMode = false
    @State private var versionTaps = 0
    @AppStorage("appearance.background") private var backgroundRaw = BgStyle.black.rawValue
    @State private var showImagePicker = false
    @State private var customMissing = false

    var body: some View {
        NavigationStack {
            Form {
                Section("外观") {
                    Picker("背景", selection: $backgroundRaw) {
                        ForEach(BgStyle.allCases) { style in
                            Text(style.label).tag(style.rawValue)
                        }
                    }
                    if BgStyle(rawValue: backgroundRaw) == .custom {
                        Button("选择图片") { showImagePicker = true }
                        Button("清除图片", role: .destructive) {
                            BackgroundStore.clear()
                            customMissing = true
                        }
                        if customMissing || BackgroundStore.load() == nil {
                            Text("还没有选择图片，背景暂时是黑色")
                                .font(.caption2)
                                .foregroundStyle(.secondary)
                        }
                    }
                }

                Section("游戏库") {
                    Text(host.gamesRoot)
                        .font(.caption.monospaced())
                        .foregroundStyle(.secondary)
                        .lineLimit(2)
                    Button("重新扫描") { host.refresh() }
                    Button("导入游戏") { showImporter = true }
                }

                if developerMode {
                    Section("引擎") {
                        ForEach(host.rows) { row in
                            VStack(alignment: .leading, spacing: 4) {
                                Text(row.name.isEmpty ? row.id : row.name)
                                Text(row.detail.isEmpty ? row.id : row.detail)
                                    .font(.caption2)
                                    .foregroundStyle(.secondary)
                            }
                        }
                    }

                    Section("诊断") {
                        if !host.lastError.isEmpty {
                            Text(host.lastError).font(.caption).foregroundStyle(.red)
                        }
                        Text(host.diagnostics.isEmpty ? "no log lines" : host.diagnostics)
                            .font(.caption2.monospaced())
                            .lineLimit(40)
                        Button("刷新诊断") { host.refreshDiagnostics() }
                    }
                }

                Section {
                    Button {
                        versionTaps += 1
                        if versionTaps >= 5 { developerMode = true }
                    } label: {
                        HStack {
                            Text("版本")
                            Spacer()
                            Text("0.1.0").foregroundStyle(.secondary)
                        }
                    }
                } footer: {
                    Text(developerMode ? "开发者选项已解锁" : "连续点击版本五次可打开开发者选项")
                        .font(.caption2)
                }
            }
            .navigationTitle("设置")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .topBarTrailing) {
                    Button("完成") { dismiss() }
                }
            }
            .fileImporter(isPresented: $showImagePicker,
                          allowedContentTypes: [.image],
                          allowsMultipleSelection: false) { result in
                if case .success(let urls) = result, let url = urls.first {
                    let accessing = url.startAccessingSecurityScopedResource()
                    if let data = try? Data(contentsOf: url), let image = UIImage(data: data) {
                        _ = BackgroundStore.save(image)
                        customMissing = false
                    }
                    if accessing { url.stopAccessingSecurityScopedResource() }
                }
            }
            .fileImporter(isPresented: $showImporter,
                          allowedContentTypes: [.folder, .item],
                          allowsMultipleSelection: true) { result in
                if case .success(let urls) = result {
                    host.importGames(from: urls)
                }
            }
        }
    }
}
