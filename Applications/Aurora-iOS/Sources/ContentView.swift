// SPDX-FileCopyrightText: 2026 Project Aurora contributors
// SPDX-License-Identifier: GPL-3.0-or-later

import SwiftUI

struct ContentView: View {
    @ObservedObject var host: AuroraHost
    @State private var showImporter = false

    var body: some View {
        NavigationStack {
            List {
                Section {
                    if host.games.isEmpty {
                        Text("Put game folders in Files → On My iPad → Aurora, or use Import.")
                            .font(.footnote)
                            .foregroundStyle(.secondary)
                    }
                    ForEach(host.games) { game in
                        GameRowView(game: game)
                    }
                } header: {
                    Text("Library")
                } footer: {
                    Text(host.gamesRoot)
                        .font(.caption2)
                        .foregroundStyle(.secondary)
                }

                Section {
                    ForEach(host.rows) { row in
                        ModuleRowView(
                            row: row,
                            running: host.runningModule == row.id,
                            onEnabled: { host.setEnabled(row, enabled: $0) },
                            onRun: { host.run(row) },
                            onStop: { host.stopRunning() },
                            onReset: { host.resetFailure(row) }
                        )
                    }
                } header: {
                    Text("Modules")
                } footer: {
                    Text("READY is computed from three things: the manifest, the build table and the runtime probe. A manifest that says \"ready\" proves nothing.")
                        .font(.footnote)
                }

                Section("Runtime output") {
                    FramePanel(host: host)
                }
            }
            .navigationTitle("Aurora")
            .toolbar {
                Button("Rescan") { host.refresh() }
                Button("Import") { showImporter = true }
            }
            .fileImporter(
                isPresented: $showImporter,
                allowedContentTypes: [.folder, .item],
                allowsMultipleSelection: true
            ) { result in
                if case .success(let urls) = result {
                    host.importGames(from: urls)
                }
            }
        }
    }
}

private struct GameRowView: View {
    let game: GameRow

    var body: some View {
        HStack {
            VStack(alignment: .leading, spacing: 4) {
                Text(game.title).font(.body)
                Text(game.module.isEmpty ? "no module claims this file" : game.module)
                    .font(.caption2)
                    .foregroundStyle(.secondary)
                if !game.detail.isEmpty {
                    Text(game.detail)
                        .font(.caption2)
                        .foregroundStyle(.secondary)
                }
            }
            Spacer()
            Text(game.state)
                .font(.caption.weight(.semibold))
                .padding(.horizontal, 8)
                .padding(.vertical, 3)
                .background(stateColor.opacity(0.16))
                .foregroundStyle(stateColor)
                .clipShape(Capsule())
        }
    }

    private var stateColor: Color {
        switch game.state {
        case "READY": return .green
        case "RUNNING": return .blue
        case "DISABLED": return .orange
        case "FAILED": return .red
        default: return .gray
        }
    }
}

private struct ModuleRowView: View {
    let row: ModuleRow
    let running: Bool
    let onEnabled: (Bool) -> Void
    let onRun: () -> Void
    let onStop: () -> Void
    let onReset: () -> Void

    var body: some View {
        VStack(alignment: .leading, spacing: 6) {
            HStack(spacing: 8) {
                Text(row.name.isEmpty ? row.id : row.name)
                    .font(.headline)
                Spacer()
                Text(row.stateName)
                    .font(.caption.weight(.semibold))
                    .padding(.horizontal, 8)
                    .padding(.vertical, 3)
                    .background(color.opacity(0.16))
                    .foregroundStyle(color)
                    .clipShape(Capsule())
            }

            Text(row.id)
                .font(.caption)
                .foregroundStyle(.secondary)

            if !row.detail.isEmpty {
                Text(row.detail)
                    .font(.caption2)
                    .foregroundStyle(.secondary)
            }

            if row.compiled {
                HStack {
                    Toggle("Enabled", isOn: Binding(
                        get: { row.enabled },
                        set: { onEnabled($0) }
                    ))
                    .toggleStyle(.switch)
                    .labelsHidden()

                    Spacer()

                    if running {
                        Button("Stop", action: onStop)
                            .buttonStyle(.bordered)
                    } else if row.isFailed {
                        Button("Retry", action: onReset)
                            .buttonStyle(.bordered)
                    } else {
                        Button("Run", action: onRun)
                            .buttonStyle(.borderedProminent)
                            .disabled(!row.isReady)
                    }
                }
            }
        }
        .padding(.vertical, 4)
    }

    private var color: Color {
        switch row.state {
        case 4: return .green
        case 5: return .blue
        case 6: return .orange
        case 7: return .red
        default: return .gray
        }
    }
}

private struct FramePanel: View {
    @ObservedObject var host: AuroraHost

    private var aspectRatio: CGFloat {
        guard host.frameWidth > 0, host.frameHeight > 0 else { return 4.0 / 3.0 }
        return CGFloat(host.frameWidth) / CGFloat(host.frameHeight)
    }

    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            if host.runningModule.isEmpty {
                Text("No module running")
                    .font(.footnote)
                    .foregroundStyle(.secondary)
            } else if host.useMetal {
                MetalFrameView(framesPresented: host.framesPresented)
                    .aspectRatio(aspectRatio, contentMode: .fit)
                    .clipShape(RoundedRectangle(cornerRadius: 8))
            } else if let image = host.frame {
                Image(uiImage: image)
                    .resizable()
                    .interpolation(.none)
                    .aspectRatio(image.size.width / image.size.height, contentMode: .fit)
                    .clipShape(RoundedRectangle(cornerRadius: 8))
            } else {
                Text("Waiting for the first frame")
                    .font(.footnote)
                    .foregroundStyle(.secondary)
            }

            HStack {
                Text("frames presented: \(host.framesPresented)")
                    .font(.caption)
                    .monospaced()
                Spacer()
                if !host.runningModule.isEmpty {
                    Text(host.runningModule)
                        .font(.caption2)
                        .foregroundStyle(.secondary)
                }
            }

            if !host.lastError.isEmpty {
                Text(host.lastError)
                    .font(.caption2)
                    .foregroundStyle(.red)
            }
        }
    }
}
