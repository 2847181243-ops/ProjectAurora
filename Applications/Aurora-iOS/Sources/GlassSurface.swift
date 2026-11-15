// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI

/* One glass treatment, two implementations.
 *
 * iOS 26 ships the real thing; everything older gets a material that is tuned to
 * look close. The new API is only referenced when the compiler is new enough to
 * know it, so building with an older toolchain still succeeds and simply uses
 * the fallback.
 */
struct GlassSurface: ViewModifier {
    var cornerRadius: CGFloat = 16
    @Environment(\.glassAlpha) private var alpha

    func body(content: Content) -> some View {
        let shape = RoundedRectangle(cornerRadius: cornerRadius, style: .continuous)
        content
            .background(.ultraThinMaterial, in: shape)
            .background(Color.white.opacity(alpha), in: shape)
            .overlay(
                shape.strokeBorder(
                    LinearGradient(colors: [Color.white.opacity(0.55), Color.white.opacity(0.06)],
                                   startPoint: .topLeading, endPoint: .bottomTrailing),
                    lineWidth: 0.9)
            )
            .shadow(color: .black.opacity(0.35), radius: 14, y: 8)
    }
}

extension View {
    /* Set AURORA_CLASSIC_GLASS=1 to force the fallback on a new system, which is
     * how the two can be compared side by side. */
    @ViewBuilder
    func glassSurface(cornerRadius: CGFloat = 16) -> some View {
        if GlassMode.usesNative {
            #if compiler(>=6.2)
            if #available(iOS 26.0, *) {
                self.glassEffect(.regular, in: RoundedRectangle(cornerRadius: cornerRadius, style: .continuous))
            } else {
                self.modifier(GlassSurface(cornerRadius: cornerRadius))
            }
            #else
            self.modifier(GlassSurface(cornerRadius: cornerRadius))
            #endif
        } else {
            self.modifier(GlassSurface(cornerRadius: cornerRadius))
        }
    }
}

/* How much white the plate adds. Higher reads as a frosted pane, lower as clear
 * glass; the player picks, because it depends on the wallpaper behind. */
struct GlassAlphaKey: EnvironmentKey {
    static let defaultValue: Double = 0.04
}

extension EnvironmentValues {
    var glassAlpha: Double {
        get { self[GlassAlphaKey.self] }
        set { self[GlassAlphaKey.self] = newValue }
    }
}

enum GlassMode {
    static var usesNative: Bool {
        if ProcessInfo.processInfo.environment["AURORA_CLASSIC_GLASS"] == "1" { return false }
        if UserDefaults.standard.bool(forKey: "appearance.classicGlass") { return false }
        return true
    }
}
