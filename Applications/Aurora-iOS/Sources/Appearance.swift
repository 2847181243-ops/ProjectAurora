// SPDX-License-Identifier: GPL-3.0-or-later
import SwiftUI
import UIKit

/* Three ways to sit behind the library: a flat black, a flat white, whatever the
 * system is doing, or a picture the user picked. The picture is stored in the
 * app container so it survives reinstalls of a different build. */
enum BgStyle: String, CaseIterable, Identifiable {
    case system, black, white, custom
    var id: String { rawValue }

    var label: String {
        switch self {
        case .system: return "跟随系统"
        case .black: return "黑色"
        case .white: return "白色"
        case .custom: return "自定义"
        }
    }

    var colorScheme: ColorScheme? {
        switch self {
        case .black: return .dark
        case .white: return .light
        default: return nil
        }
    }
}

extension UIImage {
    /* One-pixel downscale: enough to answer "is this backdrop bright?" and cheap
     * enough to run while the library is on screen. */
    func averageBrightness() -> Double {
        let size = CGSize(width: 1, height: 1)
        let renderer = UIGraphicsImageRenderer(size: size)
        let tiny = renderer.image { _ in
            draw(in: CGRect(origin: .zero, size: size))
        }
        guard let cg = tiny.cgImage, let data = cg.dataProvider?.data,
              let bytes = CFDataGetBytePtr(data) else { return 0.5 }
        let count = CFDataGetLength(data)
        guard count >= 3 else { return 0.5 }
        let r = Double(bytes[0]) / 255.0
        let g = Double(bytes[1]) / 255.0
        let b = Double(bytes[2]) / 255.0
        return 0.2126 * r + 0.7152 * g + 0.0722 * b
    }
}

enum GlassAuto {
    /* A bright backdrop needs a darker pane to keep text readable; a dark one
     * wants a lighter pane so the glass is visible at all. */
    static func alpha(backdropBrightness brightness: Double, scheme: ColorScheme) -> Double {
        let base = 0.06 + (1.0 - brightness) * 0.22
        return scheme == .dark ? min(0.40, base) : min(0.45, base + 0.06)
    }
}

extension Notification.Name {
    /* The background lives on disk, so views cannot depend on it the way they
     * depend on @State. Saving or clearing it has to announce itself. */
    static let backgroundChanged = Notification.Name("aurora.background.changed")
}

enum BackgroundStore {
    static var url: URL {
        let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
        return dir.appendingPathComponent("background.img")
    }

    /* Decoding a wallpaper on every body evaluation is expensive enough to show
     * up as a stutter, so the decoded image is kept until it changes. */
    private static var cache: UIImage?

    static func save(_ image: UIImage) -> Bool {
        guard let data = image.jpegData(compressionQuality: 0.9) else { return false }
        let ok = (try? data.write(to: url)) != nil
        cache = ok ? image : nil
        if ok { NotificationCenter.default.post(name: .backgroundChanged, object: nil) }
        return ok
    }

    static func load() -> UIImage? {
        if let cache { return cache }
        guard let data = try? Data(contentsOf: url), let image = UIImage(data: data) else { return nil }
        cache = image
        return image
    }

    static func clear() {
        cache = nil
        try? FileManager.default.removeItem(at: url)
        NotificationCenter.default.post(name: .backgroundChanged, object: nil)
    }
}

struct AppBackground: View {
    let style: BgStyle
    @State private var image: UIImage?

    var body: some View {
        GeometryReader { geometry in
            switch style {
            case .black:
                Color.black
            case .white:
                Color.white
            case .system:
                Color(uiColor: .systemBackground)
            case .custom:
                if let image {
                    Image(uiImage: image)
                        .resizable()
                        .aspectRatio(contentMode: .fill)
                        .frame(width: geometry.size.width, height: geometry.size.height)
                        .clipped()
                        /* Darken whatever the picture is so text and covers keep
                         * their contrast; the point is the backdrop, not the photo. */
                        .overlay(Color.black.opacity(0.45))
                } else {
                    Color.black
                }
            }
        }
        .ignoresSafeArea()
    }
}

/* Something for the glass to sample. Without this the plates sit on flat black
 * and read as dark rectangles no matter how thin the material is. */
struct BackdropGlow: View {
    let tint: Color
    let image: UIImage?

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                if let image {
                    Image(uiImage: image)
                        .resizable()
                        .scaledToFill()
                        .frame(width: geometry.size.width, height: geometry.size.height)
                        .clipped()
                        .blur(radius: 60)
                        .opacity(0.35)
                }
                Circle()
                    .fill(tint.opacity(0.35))
                    .frame(width: geometry.size.width * 0.9)
                    .blur(radius: 120)
                    .offset(x: -geometry.size.width * 0.25, y: -geometry.size.height * 0.2)
                Circle()
                    .fill(Color.blue.opacity(0.18))
                    .frame(width: geometry.size.width * 0.7)
                    .blur(radius: 140)
                    .offset(x: geometry.size.width * 0.3, y: geometry.size.height * 0.25)
            }
        }
        .ignoresSafeArea()
        .allowsHitTesting(false)
    }
}




/* Text colour. Auto follows the system (white on dark, black on light) and is the
 * default; the rest let somebody pick a reading colour that suits the backdrop. */
enum TextTone: String, CaseIterable, Identifiable {
    case auto, white, black, warm, accent, custom
    var id: String { rawValue }

    var label: String {
        switch self {
        case .auto: return "自动（跟随系统）"
        case .white: return "白色"
        case .black: return "黑色"
        case .warm: return "暖白"
        case .accent: return "暖橙"
        case .custom: return "自定义"
        }
    }

    var color: Color? {
        switch self {
        case .auto: return nil
        case .white: return .white
        case .black: return .black
        case .warm: return Color(red: 0.98, green: 0.94, blue: 0.88)
        case .accent: return Theme.accent
        case .custom: return nil   /* filled in from the stored hex */
        }
    }
}


extension Color {
    /* "#RRGGBB" <-> Color. Kept as text in the defaults so the picker's choice
     * survives a relaunch without inventing another storage format. */
    init?(hex: String) {
        var text = hex.trimmingCharacters(in: .whitespacesAndNewlines)
        if text.hasPrefix("#") { text.removeFirst() }
        guard text.count == 6, let value = UInt32(text, radix: 16) else { return nil }
        let r = Double((value >> 16) & 0xFF) / 255.0
        let g = Double((value >> 8) & 0xFF) / 255.0
        let b = Double(value & 0xFF) / 255.0
        self.init(red: r, green: g, blue: b)
    }

    var hexString: String {
        let ui = UIColor(self)
        var r: CGFloat = 0, g: CGFloat = 0, b: CGFloat = 0, a: CGFloat = 0
        guard ui.getRed(&r, green: &g, blue: &b, alpha: &a) else { return "FFFFFF" }
        return String(format: "%02X%02X%02X",
                      Int((r * 255).rounded()), Int((g * 255).rounded()), Int((b * 255).rounded()))
    }
}
