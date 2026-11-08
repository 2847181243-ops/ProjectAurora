/* Storage isolation.
 *
 * A module may only touch its own tree. The path is not a hint: the platform
 * resolves it, rejects escapes, and creates the directories itself. Two modules
 * therefore cannot collide, and deleting a module never touches user saves
 * because the saves live in a directory the platform owns, not the module.
 */
#ifndef AURORA_STORAGE_H
#define AURORA_STORAGE_H

#include <string>

#include "aurora/module_api.h"

namespace aurora {

class StorageLayout {
public:
    explicit StorageLayout(std::string rootDirectory);

    const std::string& root() const { return root_; }
    void setRoot(std::string rootDirectory) { root_ = std::move(rootDirectory); }

    static bool isSafeModuleId(const std::string& moduleId);
    static bool isSafeRelativePath(const std::string& relativePath);

    /* <root>/RuntimeModules/<module_id>/<Bucket> */
    std::string bucketDirectory(const std::string& moduleId, AuroraStorageKind kind) const;

    /* Resolves and rejects anything that would leave the module's tree.
     * Returns an empty string and fills |error| when the request is not allowed. */
    std::string resolve(const std::string& moduleId,
                        AuroraStorageKind kind,
                        const std::string& relativePath,
                        std::string& error) const;

    /* Creates the bucket directory on demand. Returns false with |error| set. */
    bool ensureBucket(const std::string& moduleId, AuroraStorageKind kind, std::string& error) const;

private:
    std::string root_;
};

} // namespace aurora

#endif
