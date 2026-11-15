// SPDX-License-Identifier: GPL-3.0-or-later
import PhotosUI
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
    @State private var pickFailed = false
    @State private var pickedPreview: UIImage?
    @State private var photoItem: PhotosPickerItem?
    @AppStorage("appearance.glassAlpha") private var glassAlpha = 0.04
    @AppStorage("appearance.glassAuto") private var glassAuto = false
    @AppStorage("appearance.textTone") private var textToneRaw = TextTone.auto.rawValue
    @AppStorage("appearance.textColorHex") private var textColorHex = "FF9C38"

    var body: some View {
        NavigationStack {
            Form {
                Section("外观") {
                    Toggle("跟随背景自动", isOn: $glassAuto)
                    VStack(alignment: .leading, spacing: 4) {
                        HStack {
                            Text("玻璃透明度")
                            Spacer()
                            Text(String(format: "%.0f%%", glassAlpha * 100))
                                .foregroundStyle(.secondary)
                                .monospacedDigit()
                        }
                        Slider(value: $glassAlpha, in: 0.0...0.45, step: 0.01)
                            .disabled(glassAuto)
                            .opacity(glassAuto ? 0.4 : 1)
                    }
                    Toggle("经典玻璃（用于对比）", isOn: Binding(
                        get: { UserDefaults.standard.bool(forKey: "appearance.classicGlass") },
                        set: { UserDefaults.standard.set($0, forKey: "appearance.classicGlass") }
                    ))
                    Picker("背景", selection: $backgroundRaw) {
                        ForEach(BgStyle.allCases) { style in
                            Text(style.label).tag(style.rawValue)
                        }
                    }
                    if BgStyle(rawValue: backgroundRaw) == .custom {
                        /* Two sources on purpose: the photo library is where a
                         * wallpaper usually lives, Files is where a downloaded
                         * picture lives. */
                        PhotosPicker(selection: $photoItem, matching: .images) {
                            Label("从相册选择", systemImage: "photo")
                        }
                        .onChange(of: photoItem) { item in
                            guard let item else { return }
                            Task {
                                let data = try? await item.loadTransferable(type: Data.self)
                                let image = data.flatMap { UIImage(data: $0) }
                                let saved = image.map { BackgroundStore.save($0) } ?? false
                                await MainActor.run {
                                    if let image, saved {
                                        pickedPreview = image
                                        pickFailed = false
                                        customMissing = false
                                        backgroundRaw = BgStyle.custom.rawValue
                                    } else {
                                        pickFailed = true
                                    }
                                }
                            }
                        }
                        Button("从文件选择") { showImagePicker = true }
                        .fileImporter(isPresented: $showImagePicker,
                                      allowedContentTypes: [.image],
                                      allowsMultipleSelection: false) { result in
                            if case .success(let urls) = result, let url = urls.first {
                                let accessing = url.startAccessingSecurityScopedResource()
                                defer { if accessing { url.stopAccessingSecurityScopedResource() } }
                                if let data = try? Data(contentsOf: url), let image = UIImage(data: data),
                                   BackgroundStore.save(image) {
                                    pickedPreview = image
                                    pickFailed = false
                                    customMissing = false
                                    backgroundRaw = BgStyle.custom.rawValue
                                } else {
                                    pickFailed = true
                                }
                            }
                        }
                        Button("清除图片", role: .destructive) {
                            BackgroundStore.clear()
                            pickedPreview = nil
                            customMissing = true
                        }
                        /* Show what was actually stored. Without this a pick that
                         * silently fails looks identical to a button that does
                         * nothing at all. */
                        if let pickedPreview {
                            Image(uiImage: pickedPreview)
                                .resizable()
                                .aspectRatio(contentMode: .fill)
                                .frame(height: 90)
                                .clipShape(RoundedRectangle(cornerRadius: 8))
                        }
                        if pickFailed {
                            Text("这张图片读不出来，换一张试试")
                                .font(.caption2)
                                .foregroundStyle(.red)
                        } else if customMissing || BackgroundStore.load() == nil {
                            Text("还没有选择图片，背景暂时是黑色")
                                .font(.caption2)
                                .foregroundStyle(.secondary)
                        }
                    }
                    Picker("字体颜色", selection: $textToneRaw) {
                        ForEach(TextTone.allCases) { tone in
                            Text(tone.label).tag(tone.rawValue)
                        }
                    }
                    if TextTone(rawValue: textToneRaw) == .custom {
                        ColorPicker("选择颜色", selection: Binding(
                            get: { Color(hex: textColorHex) ?? .white },
                            set: { textColorHex = $0.hexString }
                        ), supportsOpacity: false)
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
            .onAppear { pickedPreview = BackgroundStore.load() }
            .navigationTitle("设置")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .topBarTrailing) {
                    Button("完成") { dismiss() }
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
