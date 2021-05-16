#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "air.h"
#include "glass_catalog.h"

namespace geopter{

class OpticalModel;


class Database
{
public:
    Database();
    ~Database();

    void clear();

    /** Returns current AGF source directory */
    std::string agf_dir() const;

    /** get glass catalog object pointer */
    GlassCatalog* glass_catalog(const std::string catname) const;
    GlassCatalog* glass_catalog(int i) const;

    /** Return number of loaded glass catalogs */
    int catalog_count() const;

    /** load all AGF files in the directory and create catalog list */
    bool load_all_agf(const std::string agf_dir_path);

    std::shared_ptr<Medium> find(std::string medium_name);

private:

    std::string agf_dir_;
    std::vector< std::unique_ptr<GlassCatalog> > catalogs_;

    std::map< std::string, std::shared_ptr<Medium> > glass_hash_;

    void create_glass_hash();
};


} // namespace geopter

#endif // CONFIG_H
