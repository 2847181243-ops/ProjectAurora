import SwiftUI

struct ContentView: View {
    @ObservedObject var host: AuroraHost

    var body: some View {
        NavigationStack {
            List {
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
                Button("Refresh") { host.refresh() }
            }
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

    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            if let image = host.frame {
                Image(uiImage: image)
                    .resizable()
                    .interpolation(.none)
                    .aspectRatio(480.0 / 320.0, contentMode: .fit)
                    .clipShape(RoundedRectangle(cornerRadius: 8))
            } else {
                Text(host.runningModule.isEmpty ? "No module running" : "Waiting for the first frame")
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
