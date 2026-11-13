// SPDX-License-Identifier: GPL-3.0-or-later
import Foundation

/* Launch milestones, appended to Documents/boot.log.
 *
 * On a device there is no debugger and the unified log is awkward to fetch, so a
 * crash leaves nothing behind but an .ips. These marks say how far the launch got;
 * the last line is where it stopped. Kept tiny on purpose: one line per milestone.
 */
enum BootLog {
    private static let url: URL = {
        let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
        return dir.appendingPathComponent("boot.log")
    }()
    private static let queue = DispatchQueue(label: "aurora.bootlog")

    static func mark(_ text: String) {
        queue.async {
            let line = "\(Date().timeIntervalSince1970) \(text)\n"
            guard let data = line.data(using: .utf8) else { return }
            if let handle = try? FileHandle(forWritingTo: url) {
                handle.seekToEndOfFile()
                handle.write(data)
                try? handle.close()
            } else {
                try? data.write(to: url)
            }
        }
    }
}
