// SPDX-License-Identifier: GPL-3.0-or-later
import Foundation

/* Launch milestones, appended to Documents/boot.log.
 *
 * On a device there is no debugger and the unified log is awkward to fetch, so a
 * crash leaves nothing behind but an .ips. These marks say how far the launch got;
 * the last line is where it stopped. Kept tiny on purpose: one line per milestone.
 */
enum BootLog {
    static let url: URL = {
        let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0]
        return dir.appendingPathComponent("boot.log")
    }()
    private static let queue = DispatchQueue(label: "aurora.bootlog")

    /* Synchronous append for the exit handler: the queue below would never run
     * while the process is tearing down. */
    static func rawAppend(_ text: String) {
        guard let data = ("\(Date().timeIntervalSince1970) \(text)\n").data(using: .utf8) else { return }
        let fd = open(url.path, O_WRONLY | O_APPEND | O_CREAT, 0o644)
        guard fd >= 0 else { return }
        _ = data.withUnsafeBytes { write(fd, $0.baseAddress, $0.count) }
        close(fd)
    }

    static func mark(_ text: String) {
        queue.async {
            let line = "\(Date().timeIntervalSince1970) \(text)\n"
            /* Also goes to the device console: syslog can be streamed from a PC,
             * which is the only channel available when there is no debugger. */
            NSLog("[aurora] boot %@", text)
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
