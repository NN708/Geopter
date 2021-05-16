#ifndef SEQUENTIALMODEL_H
#define SEQUENTIALMODEL_H

#include <vector>
#include <memory>

#include "transform.h"
#include "field.h"

namespace geopter {


class OpticalModel;
class Surface;
class Gap;
class Medium;
class SurfaceProfile;

struct PathComponent{
    Surface* srf = nullptr;
    Gap* gap = nullptr;
    double rndx = 1.0 ;
    Tfrm tfrm;
    double z_dir=1.0;
};

using Path = std::vector<PathComponent>;


class SequentialModel
{
public:
    SequentialModel(OpticalModel* opt_model);
    ~SequentialModel();

    void update_model(bool update_sd=true);

    void clear();
    void initialize_arrays();

    int surface_count() const;
    int gap_count() const;

    Path path(double wl=-1,int start=0, int stop=-1, int step=1) const;

    /** returns a list with refractive indices for all **wvls** */
    std::vector< std::vector<double> > calc_ref_indices_for_spectrum(std::vector<double> wvl);

    /** returns the central wavelength in nm of the model's ``WvlSpec`` */
    double central_wavelength() const;

    /** returns the central refractive index of the model's */
    double central_rndx(int i);


    void set_cur_surface(int s);

    Surface* surface(int n) const;
    Surface* current_surface() const;
    Gap* gap(int n) const;
    double z_dir(int n)  const;


    /** insert surf and gap at the cur_gap edge of the sequential model graph */
    void insert(std::shared_ptr<Surface> s, std::shared_ptr<Gap> g=nullptr);

    /** insert dummy surface at the specified index */
    void insert(int i);

    /** Remove surface and gap from sequence model */
    void remove(int i);

    /** add a surface where surf is a list that contains */
    void add_surface(double radius, double thickness, std::string medium_display_name="");

    void add_surface(double radius, double thickness, std::shared_ptr<Medium> med);

    void add_surface(std::unique_ptr<SurfaceProfile> prf, double thickness, std::string medium_display_name="");

    int set_stop();
    void set_stop(int i);

    int stop_surface() const;

    /** Update semi diameter for all surfaces */
    void set_semi_diameters();

    /** Return global surface coordinates (rot, t) wrt surface glo. */
    std::vector<Tfrm> compute_global_coords(int glo=1);

    /** Return forward surface coordinates (r.T, t) for each interface */
    std::vector<Tfrm> compute_local_transforms();

    /** Compute vignetting factors using current aperture setting */
    void compute_vignetting();

    /** List up model properties */
    void print(std::ostringstream& oss);


    /** Return overall length from s1 to s2 */
    double length(int s1, int s2) const;

private:
    /** generate transform rotation and translation from s1 coords to s2 coords */
    Tfrm forward_transform(const Surface& s1, double zdist, const Surface& s2);

    /** generate transform rotation and translation from s2 coords to s1 coords */
    Tfrm reverse_transform(const Surface& s1, double zdist, const Surface& s2);

private:
    OpticalModel* optical_model_;
    std::vector< std::shared_ptr<Surface> > interfaces_;
    std::vector< std::shared_ptr<Gap> > gaps_;
    std::vector<double> z_dirs_;

    /** forward transform, interface to interface*/
    std::vector<Tfrm> lcl_tfrms_;

    /** global coordinates of each interface wrt the 1st interface */
    std::vector<Tfrm> gbl_tfrms_;

    /** a list with refractive indices for all **wvls** */
    std::vector< std::vector<double> > rndx_; // [ for each surface [(rind for w1), (rind for w2), ...] ]
    std::vector<double> wvlns_;

    /** index of stop interface */
    int stop_surface_;

    /** insertion index for next interface */
    int cur_surface_;

    double compute_vly(Field& fld, double wvl);
    double compute_vuy(Field& fld, double wvl);
    double compute_vlx(Field& fld, double wvl);
    double compute_vux(Field& fld, double wvl);

    double compute_vignetting_factor(Field& fld, double wvl, int pupil_ray_index);
};

} //namespace geopter


#endif // SEQUENTIALMODEL_H
