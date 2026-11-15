// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI

/* Full-screen playback. The frame is the whole point, so the controls stay out of
 * the way and only appear when the screen is touched. */
struct PlayerView: View {
    @ObservedObject var host: AuroraHost
    @ObservedObject private var gamepad = Gamepad.shared
    let game: GameRow
    var onExit: () -> Void

    @State private var showControls = true
    @State private var hideTask: Task<Void, Never>?

    private var aspectRatio: CGFloat {
        guard host.frameWidth > 0, host.frameHeight > 0 else { return 4.0 / 3.0 }
        return CGFloat(host.frameWidth) / CGFloat(host.frameHeight)
    }

    var body: some View {
        ZStack {
            Color.black.ignoresSafeArea()

            Group {
                if host.useMetal {
                    MetalFrameView(framesPresented: host.framesPresented,
                                  frameWidth: host.frameWidth,
                                  frameHeight: host.frameHeight)
                } else if let frame = host.frame {
                    Image(uiImage: frame).resizable().aspectRatio(contentMode: .fit)
                } else {
                    Text("waiting for the first frame")
                        .font(.footnote)
                        .foregroundStyle(.secondary)
                }
            }
            .aspectRatio(aspectRatio, contentMode: .fit)
            .frame(maxWidth: .infinity, maxHeight: .infinity)

            /* Where a controller's virtual pointer sits. Only drawn while one is
             * connected, so touch players never see it. */
            if !gamepad.connectedName.isEmpty {
                GeometryReader { geometry in
                    Circle()
                        .fill(Color.white.opacity(0.35))
                        .overlay(Circle().strokeBorder(Color.white.opacity(0.8), lineWidth: 1.5))
                        .frame(width: 18, height: 18)
                        .position(x: gamepad.pointer.x * geometry.size.width,
                                  y: gamepad.pointer.y * geometry.size.height)
                        .allowsHitTesting(false)
                }
            }

            if showControls {
                controls.transition(.opacity)
            }
        }
        .statusBarHidden()
        .contentShape(Rectangle())
        .onTapGesture { flashControls() }
        .onAppear { flashControls() }
        .onDisappear {
            hideTask?.cancel()
            host.stopRunning()
        }
    }

    private var controls: some View {
        VStack {
            HStack {
                Button {
                    onExit()
                } label: {
                    Image(systemName: "chevron.left")
                        .font(.headline)
                        .padding(11)
                        .glassSurface(cornerRadius: 22)
                }
                .accessibilityLabel("返回")

                Spacer()

                Button {
                    onExit()
                } label: {
                    Image(systemName: "stop.fill")
                        .font(.headline)
                        .padding(11)
                        .glassSurface(cornerRadius: 22)
                }
                .accessibilityLabel("停止")
            }
            .padding(.horizontal, 20)
            .padding(.top, 12)

            Spacer()

            HStack(spacing: 12) {
                Text(game.title).font(.caption).foregroundStyle(.white)
                Text("\(host.framesPresented) 帧").font(.caption2).foregroundStyle(.white.opacity(0.6))
                if !host.lastError.isEmpty {
                    Text(host.lastError).font(.caption2).foregroundStyle(.red).lineLimit(1)
                }
            }
            .padding(.horizontal, 14)
            .padding(.vertical, 8)
            .glassSurface(cornerRadius: 22)
            .padding(.bottom, 24)
        }
        .foregroundStyle(.white)
    }

    /* Fade the controls out again so they do not sit on top of the picture. */
    private func flashControls() {
        withAnimation(.easeInOut(duration: 0.18)) { showControls = true }
        hideTask?.cancel()
        hideTask = Task {
            try? await Task.sleep(nanoseconds: 3_000_000_000)
            if Task.isCancelled { return }
            withAnimation(.easeInOut(duration: 0.3)) { showControls = false }
        }
    }
}
