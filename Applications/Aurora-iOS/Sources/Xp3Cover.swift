// SPDX-License-Identifier: GPL-3.0-or-later
import Compression
import Foundation
import UIKit

/* KiriKiri games pack everything into an XP3 archive, so a cover that the game
 * does ship never reaches the library as a loose file. This reads the archive
 * index and pulls out the most cover-like picture it can find.
 *
 * Format (as this game ships it):
 *   header    "XP3\r\n \n\x1a\x8bg\x01" then the index offset as u64 LE
 *   index     chunk: type(1) + packed size(8) + raw size(8) + zlib data
 *   entries   chunks tagged "File", each holding "info" and "segm" sub-chunks
 *   info      flags(4) + raw size(8) + packed size(8) + name length(2 chars) + UTF-16 name
 *   segm      count(4) then (offset u64, size u64) per segment
 */
enum Xp3Cover {
    private static let magic: [UInt8] = [0x58, 0x50, 0x33, 0x0d, 0x0a, 0x20, 0x0a, 0x1a, 0x8b, 0x67, 0x01]

    struct Entry {
        let name: String
        let rawSize: Int
        let segments: [(offset: Int, size: Int)]
    }

    /* Names that usually mean "this is the game's face". */
    private static let preferred = ["logo", "title", "cover", "package", "icon"]

    static func image(for gamePath: String) -> UIImage? {
        var folder = URL(fileURLWithPath: gamePath)
        var isDirectory: ObjCBool = false
        if FileManager.default.fileExists(atPath: folder.path, isDirectory: &isDirectory), !isDirectory.boolValue {
            folder = folder.deletingLastPathComponent()
        }
        guard let files = try? FileManager.default.contentsOfDirectory(atPath: folder.path) else { return nil }
        let archives = files.filter { $0.lowercased().hasSuffix(".xp3") }
            .sorted { ($0.lowercased() == "data.xp3" ? 0 : 1) < ($1.lowercased() == "data.xp3" ? 0 : 1) }

        for archive in archives {
            if let image = image(inArchive: folder.appendingPathComponent(archive)) { return image }
        }
        return nil
    }

    private static func image(inArchive url: URL) -> UIImage? {
        guard let handle = try? FileHandle(forReadingFrom: url) else { return nil }
        defer { try? handle.close() }

        guard let head = try? handle.read(upToCount: 19), head.count == 19 else { return nil }
        guard Array(head.prefix(11)) == magic else { return nil }
        let indexOffset = Int(readUInt64(head, 11))
        guard indexOffset > 0 else { return nil }
        try? handle.seek(toOffset: UInt64(indexOffset))
        guard let tail = try? handle.readToEnd(), tail.count > 17 else { return nil }
        guard let index = inflate(tail.subdata(in: 17..<tail.count)) else { return nil }

        let entries = parseEntries(index)
        let images = entries.filter { name in
            ["png", "jpg", "jpeg", "bmp"].contains((name.name as NSString).pathExtension.lowercased())
        }
        guard !images.isEmpty else { return nil }
        let ranked = images.sorted { a, b in
            let pa = preferred.contains { a.name.lowercased().contains($0) } ? 0 : 1
            let pb = preferred.contains { b.name.lowercased().contains($0) } ? 0 : 1
            if pa != pb { return pa < pb }
            return a.rawSize > b.rawSize
        }

        for entry in ranked.prefix(6) {
            guard let data = data(of: entry, in: handle), let image = UIImage(data: data) else { continue }
            if image.size.width >= 100, image.size.height >= 100 { return image }
        }
        return nil
    }

    private static func data(of entry: Entry, in handle: FileHandle) -> Data? {
        var out = Data()
        for segment in entry.segments {
            try? handle.seek(toOffset: UInt64(segment.offset))
            guard let chunk = try? handle.read(upToCount: segment.size), !chunk.isEmpty else { return nil }
            if chunk[0] == 0x78, let inflated = inflate(chunk) {
                out.append(inflated)
            } else {
                out.append(chunk)
            }
        }
        return out.isEmpty ? nil : out
    }

    private static func parseEntries(_ index: Data) -> [Entry] {
        var entries: [Entry] = []
        for chunk in chunks(index) where chunk.tag == "File" {
            let subs = chunks(chunk.body)
            guard let info = subs.first(where: { $0.tag == "info" }),
                  let segm = subs.first(where: { $0.tag == "segm" }) else { continue }
            let nameLength = Int(readUInt16(info.body, 20))
            let nameData = info.body.subdata(in: 22..<min(info.body.count, 22 + nameLength * 2))
            let name = String(data: nameData, encoding: .utf16LittleEndian) ?? ""
            let rawSize = Int(readUInt64(info.body, 4))

            var segments: [(offset: Int, size: Int)] = []
            let count = Int(readUInt32(segm.body, 0))
            if count > 0, count < 4096 {
                for i in 0..<count {
                    let base = 4 + i * 16
                    guard base + 16 <= segm.body.count else { break }
                    segments.append((offset: Int(readUInt64(segm.body, base)),
                                     size: Int(readUInt64(segm.body, base + 8))))
                }
            }
            if !name.isEmpty, !segments.isEmpty {
                entries.append(Entry(name: name, rawSize: rawSize, segments: segments))
            }
        }
        return entries
    }

    private struct Chunk { let tag: String; let body: Data }

    private static func chunks(_ data: Data) -> [Chunk] {
        var out: [Chunk] = []
        var offset = 0
        while offset + 12 <= data.count {
            let tag = String(data: data.subdata(in: offset..<(offset + 4)), encoding: .isoLatin1) ?? ""
            let length = Int(readUInt64(data, offset + 4))
            let start = offset + 12
            guard length >= 0, start + length <= data.count else { break }
            out.append(Chunk(tag: tag, body: data.subdata(in: start..<(start + length))))
            offset = start + length
        }
        return out
    }

    private static func readUInt16(_ data: Data, _ offset: Int) -> UInt16 {
        guard offset + 2 <= data.count else { return 0 }
        return data.subdata(in: offset..<(offset + 2)).withUnsafeBytes { $0.loadUnaligned(as: UInt16.self) }
    }

    private static func readUInt32(_ data: Data, _ offset: Int) -> UInt32 {
        guard offset + 4 <= data.count else { return 0 }
        return data.subdata(in: offset..<(offset + 4)).withUnsafeBytes { $0.loadUnaligned(as: UInt32.self) }
    }

    private static func readUInt64(_ data: Data, _ offset: Int) -> UInt64 {
        guard offset + 8 <= data.count else { return 0 }
        return data.subdata(in: offset..<(offset + 8)).withUnsafeBytes { $0.loadUnaligned(as: UInt64.self) }
    }

    private static func inflate(_ data: Data) -> Data? {
        guard !data.isEmpty else { return nil }
        var capacity = max(data.count * 8, 64 * 1024)
        for _ in 0..<3 {
            var output = Data(count: capacity)
            let written = output.withUnsafeMutableBytes { destination -> Int in
                guard let destinationBase = destination.bindMemory(to: UInt8.self).baseAddress else { return 0 }
                return data.withUnsafeBytes { source -> Int in
                    guard let sourceBase = source.bindMemory(to: UInt8.self).baseAddress else { return 0 }
                    return compression_decode_buffer(destinationBase, capacity, sourceBase, data.count,
                                                     nil, COMPRESSION_ZLIB)
                }
            }
            if written > 0, written < capacity {
                return output.prefix(written)
            }
            capacity *= 4
        }
        return nil
    }
}
