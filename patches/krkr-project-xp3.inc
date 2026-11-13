void TVPAutoMountProjectXP3Archives() {
    if(TVPProjectDir.GetLastChar() != TJS_W('/'))
        return;

    ttstr projStorage = TVPProjectDir;

    ttstr nativeProj = projStorage;
    try {
        TVPGetLocalName(nativeProj);
    } catch(...) {
        spdlog::error("AutoMountProjectXP3: TVPGetLocalName threw for {}",
                      projStorage.AsStdString());
        return;
    }
    std::string projPath = nativeProj.AsStdString();

    DIR *dirp = opendir(projPath.c_str());
    if(!dirp) {
        spdlog::error("AutoMountProjectXP3: opendir failed for: {}, errno={}",
                      projPath, errno);
        return;
    }

    std::vector<std::string> xp3Names;
    struct dirent *dp;
    while((dp = readdir(dirp))) {
        std::string name = dp->d_name;
        if(name.size() < 5) continue;
        std::string ext = name.substr(name.size() - 4);
        for(auto &c : ext) c = (char)tolower((unsigned char)c);
        if(ext != ".xp3") continue;
        xp3Names.push_back(name);
    }
    closedir(dirp);

    std::sort(xp3Names.begin(), xp3Names.end());

    if(xp3Names.empty()) {
        TVPAddImportantLog(TJS_W("(info) No XP3 archives in project dir"));
        return;
    }

    tjs_int mountedCount = 0;

    for(const auto &xp3Name : xp3Names) {
        ttstr archivePath = projStorage + ttstr(xp3Name.c_str());
        archivePath = TVPNormalizeStorageName(archivePath);

        tTVPArchive *arc = nullptr;
        try {
            arc = TVPOpenArchive(archivePath, true);
        } catch(...) {
            TVPAddImportantLog(
                ttstr(TJS_W("(warn) Failed to open project archive: ")) +
                archivePath);
            continue;
        }
        if(!arc) continue;
        mountedCount++;

        std::set<std::u16string> dirPaths;
        dirPaths.insert(std::u16string());

        tjs_uint fileCount = arc->GetCount();
        for(tjs_uint i = 0; i < fileCount; i++) {
            ttstr fname = arc->GetName(i);
            const tjs_char *s = fname.c_str();
            tjs_int len = fname.GetLen();
            for(tjs_int j = 0; j < len; j++) {
                if(s[j] == TJS_W('/')) {
                    std::u16string d(
                        reinterpret_cast<const char16_t *>(s),
                        static_cast<size_t>(j + 1));
                    dirPaths.insert(d);
                }
            }
        }
        arc->Release();

        tjs_char delimStr[2] = { TVPArchiveDelimiter, 0 };
        ttstr archiveBase = archivePath + ttstr(delimStr);
        for(const auto &d : dirPaths) {
            ttstr dirStr(reinterpret_cast<const tjs_char *>(d.c_str()),
                         static_cast<tjs_int>(d.size()));
            ttstr autoPath = archiveBase + dirStr;
            try {
                TVPAddAutoPath(autoPath);
                TVPAutoMountedPaths.push_back(TVPNormalizeStorageName(autoPath));
            } catch(...) {}
        }

        TVPAddImportantLog(
            ttstr(TJS_W("(info) Auto-mounted project archive: ")) +
            archivePath + ttstr(TJS_W(" (")) +
            ttstr((tjs_int)dirPaths.size()) + ttstr(TJS_W(" dirs, ")) +
            ttstr((tjs_int)fileCount) + ttstr(TJS_W(" files)")));
    }

    TVPAddImportantLog(
        ttstr(TJS_W("(info) AutoMountProjectXP3: found ")) +
        ttstr((tjs_int)xp3Names.size()) + ttstr(TJS_W(" xp3, mounted ")) +
        ttstr(mountedCount) + ttstr(TJS_W(" archive(s) in project dir")));
}