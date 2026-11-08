// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef AURORA_LIBRARY_H
#define AURORA_LIBRARY_H

#include <cstddef>
#include <string>
#include <vector>

#include "aurora/registry.h"

namespace aurora {

/* One game the library knows about.
 *
 * A game can be recognised without its module being compiled into this build:
 * that is deliberate, because "you have this game, the PSP module is not in this
 * build" is more useful than silently hiding it. */
struct GameEntry {
    std::string game_id;      /* stable hash of the normalised path */
    std::string title;        /* file or folder name */
    std::string path;         /* absolute path to the file or the folder */
    std::string module_id;    /* empty when nothing claimed it */
    std::string format;       /* extension without the dot */
    std::string detected_by;  /* "magic:5850330A" / "filename:nscript.dat" / "extension:xp3" */
};

/* Rule matching lives in the manifests, not here. The library only applies them,
 * so adding an engine is a data change. */
class GameLibrary {
public:
    explicit GameLibrary(const ModuleRegistry& registry) : registry_(registry) {}

    /* Matches one path. Directories are inspected one level deep for an entry
     * file (a script or an archive), which is how most galgame folders look. */
    GameEntry classify(const std::string& path) const;

    /* Walks |root| up to |max_depth| levels, returns one entry per game. */
    std::vector<GameEntry> scan(const std::string& root, int max_depth = 3) const;

private:
    const ModuleRegistry& registry_;
};

/* Identity is content based, not path based: moving or renaming a game must not
 * create a second library entry. The id hashes the first 64 KB of the file the
 * detector matched plus its size, which is cheap and stable for real games. */
std::string gameIdForContent(const std::string& entry_path);

/* Library persistence. The file is a plain JSON array so it stays inspectable. */
bool saveGames(const std::string& json_path,
               const std::vector<GameEntry>& games,
               std::string& error);
bool loadGames(const std::string& json_path,
               std::vector<GameEntry>& games,
               std::string& error);

} // namespace aurora

#endif
