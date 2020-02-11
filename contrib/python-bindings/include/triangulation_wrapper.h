// ---------------------------------------------------------------------
//
// Copyright (C) 2016 - 2018 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------

#ifndef dealii_triangulation_wrapper_h
#define dealii_triangulation_wrapper_h

#include <deal.II/base/config.h>

#include <boost/python.hpp>

#include <manifold_wrapper.h>
#include <mapping_wrapper.h>
#include <point_wrapper.h>
#include <quadrature_wrapper.h>

#include <string>
#include <vector>

DEAL_II_NAMESPACE_OPEN

namespace python
{
  class CellAccessorWrapper;

  class TriangulationWrapper
  {
  public:
    typedef std::vector<CellAccessorWrapper>::iterator iterator;

    /**
     * Constructor. Takes a string @p dim with one of the following values
     * "2D", "2d", "3D", or "3d".
     */
    TriangulationWrapper(const std::string &dim);

    /**
     * Constructor. Takes a string @p dim with one of the following values
     * "2D", "2d", "3D", or "3d" and a string @p spacedim with one of the
     * following values "2D", "2d", "3D", or "3d". The dimension of @p spacedim
     * must be larger than the dimension of @p dim
     */
    TriangulationWrapper(const std::string &dim, const std::string &spacedim);

    /**
     * Destructor.
     */
    ~TriangulationWrapper();

    /**
     * Create a triangulation from a list of vertices and a list of indices,
     * each of the latter being a list of 1<<dim vertex indices.
     */
    void
    create_triangulation(const boost::python::list &vertices,
                         const boost::python::list &cells_vertices);

    /**
     * Return the number of active cells.
     */
    unsigned int
    n_active_cells() const;

    /**
     * Generate a hyper cube (square in 2D and cube in 3D) with exactly one
     * cell.
     */
    void
    generate_hyper_cube(const double left     = 0.,
                        const double right    = 1.,
                        const bool   colorize = false);

    /**
     * Generate a simplex with (dim+1) vertices and mesh cells.
     */
    void
    generate_simplex(boost::python::list &vertices);

    /**
     * Same as hyper_cube but not only one cell is created but each coordinate
     * direction is subdivided in @p repetitions cells.
     */
    void
    generate_subdivided_hyper_cube(const unsigned int repetitions,
                                   const double       left  = 0.,
                                   const double       right = 1.);

    /**
     * Generate a coordinate-parallel brick from the two diagonally opposite
     * corners points @p p1 and @p p2.
     */
    void
    generate_hyper_rectangle(PointWrapper &p1,
                             PointWrapper &p2,
                             const bool    colorize = false);

    /**
     * Generate a coordinate-parallel brick from the two diagonally opposite
     * corners points @p p1 and @p p2. In direction i, repetitions[i] cells are
     * created.
     */
    void
    generate_subdivided_hyper_rectangle(boost::python::list &repetitions,
                                        PointWrapper &       p1,
                                        PointWrapper &       p2,
                                        const bool           colorize = false);

    /**
     * Like the previous function. However, here the first argument does not
     * denote the number of subdivisions in each coordinate direction, but a
     * sequence of step sizes for each coordinate direction. This function is
     * therefore the right one to generate graded meshes where cells are
     * concentrated in certains areas, rather than a uniformly subdidived mesh
     * as the previous function generates.
     */
    void
    generate_subdivided_steps_hyper_rectangle(boost::python::list &step_sizes,
                                              PointWrapper &       p1,
                                              PointWrapper &       p2,
                                              const bool colorize = false);

    /**
     * Like the previous function, but with the following twist: the @p
     * material_id argument is a dim-dimensional array that, for each cell,
     * indicates which material_id should be set. In addition, and this is the
     * major new functionality, if the material_id of a cell is (-1), then that
     * cell is deleted from the triangulation, i.e. the domain will have a void
     * there.
     */
    void
    generate_subdivided_material_hyper_rectangle(
      boost::python::list &spacing,
      PointWrapper &       p,
      boost::python::list &material_id,
      const bool           colorize = false);

    /*! @copydoc GridGenerator::hyper_cube_with_cylindrical_hole
     */
    void
    generate_hyper_cube_with_cylindrical_hole(
      const double       inner_radius = .25,
      const double       outer_radius = .5,
      const double       L            = .5,
      const unsigned int repetitions  = 1,
      const bool         colorize     = false);

    /**
     * Rectangular domain with rectangular pattern of holes. The domain itself
     * is rectangular, very much as if it had been generated by
     * subdivided_hyper_rectangle(). The argument @p holes specifies how many
     * square holes the domain should have in each coordinate direction. The
     * total number of mesh cells in that direction is then this number plus
     * one. The number of holes in one direction must be at least one.
     */
    void
    generate_cheese(boost::python::list &holes);

    /**
     * A general quadrilateral in 2d or a general hexahedron in 3d. It is the
     * responsibility of the user to provide the vertices in the right order
     * (see the documentation of the GeometryInfo class) because the vertices
     * are stored in the same order as they are given. It is also important to
     * make that the volume of the cell is positive. If the argument @p colorize
     * is false, all boundary indicators are set to zero ("not colorized") for
     * 2d and 3d. If it is true, the boundary is colorized as in
     * hyper_rectangle(). In 1d, the indicators are always colorized.
     */
    void
    generate_general_cell(boost::python::list &vertices,
                          const bool           colorize = false);

    /**
     * A parallelogram. The first corner point is the origin. The @tparam dim
     * adjacent points are the ones given in the second argument and the fourth
     * point will be the sum of these two vectors. Colorizing is done in the
     * same way as in hyper_rectangle().
     * @note This function is implemented in 2d only.
     */
    void
    generate_parallelogram(boost::python::list &corners,
                           const bool           colorize = false);

    /**
     * A parallelepiped. The first corner point is the origin. The @tparam dim
     * adjacent points are vectors describing the edges of the parallelepiped
     * with respect to the origin. Additional points are sums of these dim
     * vectors. Colorizing is done according to hyper_rectangle().
     * @note This function silently reorders the vertices on the cells to
     * lexicographic ordering (see GridReordering::reoder_grid()). In other
     * words, if reordering of the vertices does occur, the ordering of vertices
     * in the array of @p corners will no longer refer to the same
     * triangulation.
     */
    void
    generate_parallelepiped(boost::python::list &corners,
                            const bool           colorize = false);

    /**
     * A subdivided parallelepiped. The first corner point is the origin. The
     * @tparam dim adjacent points are vectors describing the edges of the
     * parallelepiped with respect to the origin. Additional points are sums of
     * these dim vectors. The variable @p n_subdivisions designates the number
     * of subdivisions in each of the @tparam dim directions. Colorizing is odne
     * according to hyper_rectangle().
     */
    void
    generate_fixed_subdivided_parallelepiped(const unsigned int n_subdivisions,
                                             boost::python::list &corners,
                                             const bool colorize = false);

    /**
     * A subdivided parallelepided, i.e., the same as above, but where the
     * number of subdivisions in each of the @tparam dim directions may vary.
     * Colorizing is done according to hyper_rectangle().
     */
    void
    generate_varying_subdivided_parallelepiped(
      boost::python::list &n_subdivisions,
      boost::python::list &corners,
      const bool           colorize = false);

    /**
     * Hypercube with a layer of hypercubes around it. The first two parameters
     * give the lower and upper bound of the inner hypercube in all coordinate
     * directions. @p thickness marks the size of the layer cells. If the flag
     * @p colorize is set, the outer cells get material id's according to the
     * following scheme: extending over the inner cube (+/-) x-direction: 1/2.
     * In y-direction 4/8, in z-direction 16/32. The cells at corners and edges
     * (3d) get these values bitwise or'd.
     */
    void
    generate_enclosed_hyper_cube(const double left      = 0.,
                                 const double right     = 1.,
                                 const double thickness = 1.,
                                 const bool   colorize  = false);

    /**
     * Generate a hyperball, i.e. a circle or a ball around @p center with
     * given @p radius. In order to avoid degenerate cells at the boundaries,
     * the circle is triangulated by five cells, the ball by seven cells. The
     * diameter of the center cell is chosen so that the aspect ratio of the
     * boundary cells after one refinement is optimized. You should attach a
     * SphericalManifold to the cells and faces for correct placement of
     * vertices upon refinement and to be able to use higher order mappings.
     */
    void
    generate_hyper_ball(PointWrapper &center, const double radius = 1.);

    /**
     * Generate a hyper sphere, i.e., a surface of a ball in @tparam spacedim
     * dimensions. This function only exists for dim+1=spacedim in 2 and 3 space
     * dimensions. You should attach a SphericalManifold to the cells and faces
     * for correct placement of vertices upon refinement and to be able to use
     * higher order mappings.
     */
    void
    generate_hyper_sphere(PointWrapper &center, const double radius = 1.);

    /**
     * Generate a hyper-ball intersected with the positive orthant relate to @p
     * center, which contains three elements in 2d and four in 3d. The boundary
     * indicators for the final triangulations are 0 for the curved boundary
     * and 1 for the cut plane. The appropriate manifold class is
     * SphericalManifold.
     */
    void
    generate_quarter_hyper_ball(PointWrapper &center, const double radius = 1.);

    /**
     * Generate a half hyper-ball around @p center, which contains four elements
     * in 2d and 6 in 3d. The cut plane is perpendicular to the x-axis. The
     * boundary indicators for the final triangulation are 0 for the curved
     * boundary and 1 for the cut plane. The appropriate manifold class is
     * SphericalManifold.
     */
    void
    generate_half_hyper_ball(PointWrapper &center, const double radius = 1.);

    /**
     * Produce a hyper-shell, the region between two spheres around center,
     * with given inner_radius and outer_radius. The number n_cells indicates
     * the number of cells of the resulting triangulation, i.e., how many
     * cells form the ring (in 2d) or the shell (in 3d).
     * The appropriate manifold class is SphericalManifold.
     */
    void
    generate_hyper_shell(PointWrapper & center,
                         const double   inner_radius,
                         const double   outer_radius,
                         const unsigned n_cells  = 0,
                         bool           colorize = false);

    /**
     * Shift each vertex of the Triangulation by the given @p shift_list.
     */
    void
    shift(boost::python::list &shift_list);

    /**
     * Scale the entire triangulation by the given factor. To preserve
     * the orientation of the triangulation, the factor must be positive.
     */
    void
    scale(const double scaling_factor);

    /**
     * Given two triangulations, create the triangulation that contains the
     * cells of both triangulations.
     */
    void
    merge_triangulations(TriangulationWrapper &triangulation_1,
                         TriangulationWrapper &triangulation_2);

    /**
     * Create a new flat triangulation @param out_tria which contains a single
     * level with all active cells of the input triangulation. If the spacedim
     * are different, only the smalled spacedim components of the vertices are
     * copied over. This is useful to create a Triangulation<2,3> out of a
     * Triangulation<2,2>, or to project a Triangulation<2,3> into a
     * Triangulation<2,2>, by neglecting the z component of the vertices. No
     * internal checks are performed on the vertices, which are assumed to make
     * sense topologically in the target spacedim dimensional space. If this is
     * not the case, you will encounter problems when using the triangulation
     * later on. All information about cell manifold_ids and material ids are
     * copied from one triangulation to the other, and only the boundary
     * manifold_ids and boundary_ids are copied over from the faces of the
     * triangulation to the faces of @p out_tria. If you need to specify
     * manifold ids on interior faces, they have to be specified manually after
     * the triangulation is created. This function will fail the input
     * Triangulation contains hanging nodes.
     */
    void
    flatten_triangulation(TriangulationWrapper &tria_out);

    /**
     * Take a 2d Triangulation that is being extruded in z direction by
     * the total height of height using n_slices slices (minimum is 2).
     * The boundary indicators of the faces of input are going to be
     * assigned to the corresponding side walls in z direction. The
     * bottom and top get the next two free boundary indicators.
     */
    void
    extrude_triangulation(const unsigned int    n_slices,
                          const double          height,
                          TriangulationWrapper &tria_out);

    /**
     * Distort the given triangulation by randomly moving around all the
     * vertices of the grid. The direction of movement of each vertex is
     * random, while the length of the shift vector has a value of factor
     * times the minimal length of the active edges adjacent to this vertex.
     * Note that factor should obviously be well below 0.5.
     */
    void
    distort_random(const double factor, const bool keep_boundary = true);

    /**
     * Transform the vertices of the given triangulation by applying the
     * function object provided as first argument to all its vertices.
     */
    void
    transform(boost::python::object &transformation);

    /**
     * Find and return an active cell that surrounds a given point p.
     * The mapping is used to determine whether the given point is inside a
     * given cell.
     */
    CellAccessorWrapper
    find_active_cell_around_point(
      PointWrapper &         p,
      MappingQGenericWrapper mapping = MappingQGenericWrapper());

    /**
     * Find and return a list of active cells that surround a given vertex with
     * index vertex_index.
     */
    boost::python::list
    find_cells_adjacent_to_vertex(const unsigned int vertex_index);

    /**
     * Assign a manifold object to a certain part of the triangulation.
     * If an object with manifold number is refined, this object
     * is used to find the location of new vertices (see the results
     * section of step-49 for a more in-depth discussion of this, with
     * examples). It is also used for non-linear (i.e.: non-Q1)
     * transformations of cells to the unit cell in shape function
     * calculations.
     */
    void
    set_manifold(const int number, ManifoldWrapper &manifold);

    /**
     * Reset those parts of the triangulation with the given manifold_number to
     * use a FlatManifold object. This is the default state of a non-curved
     * triangulation, and undoes assignment of a different Manifold object by
     * the function Triangulation::set_manifold().
     */
    void
    reset_manifold(const int number);

    /**
     * Refine all the cells @p n times.
     */
    void
    refine_global(const unsigned int n);

    /**
     * Execute both refinement and coarsening of the Triangulation.
     */
    void
    execute_coarsening_and_refinement();

    /**
     * Return the list of active cell accessors associated to the underlying
     * Triangulation.
     */
    boost::python::list
    active_cells();

    /**
     * Return the diameter of the smallest active cell of a triangulation
     */
    double
    minimal_cell_diameter() const;

    /**
     * Return the diameter of the largest active cell of a triangulation
     */
    double
    maximal_cell_diameter() const;

    /**
     * Computes an aspect ratio measure for all active cells and fills a vector
     * with one entry per cell.
     */
    boost::python::list
    compute_aspect_ratio_of_cells(const MappingQGenericWrapper &mapping,
                                  const QuadratureWrapper &     quadrature);

    /**
     * Write mesh to the output file @filename according to the given data
     * format.
     */
    void
    write(const std::string &filename, const std::string format) const;

    /**
     * Read mesh from the file @filename using the given data
     * format.
     */
    void
    read(const std::string &filename, const std::string format) const;

    /**
     * Write the Triangulation in file.
     */
    void
    save(const std::string &filename) const;

    /**
     * Load the Triangulation from a file.
     */
    void
    load(const std::string &filename);

    /**
     * Return the dimension of the underlying Triangulation object.
     */
    int
    get_dim() const;

    /**
     * Return the space dimension of the underlying Triangulation object.
     */
    int
    get_spacedim() const;

    /**
     * Return a pointer that can be casted to the underlying Triangulation
     * object.
     */
    void *
    get_triangulation() const;

  private:
    /**
     * Helper function for the constructors.
     */
    void
    setup(const std::string &dimension, const std::string &spacedimension);

    /**
     * Dimension of the underlying Triangulation object.
     */
    int dim;

    /**
     * Space dimension of the underlying Triangulation object.
     */
    int spacedim;

    /**
     * Pointer that can be casted to the underlying Triangulation object.
     */
    void *triangulation;
  };


  //-------------------- inline functions -----------------------//



  inline int
  TriangulationWrapper::get_dim() const
  {
    return dim;
  }



  inline int
  TriangulationWrapper::get_spacedim() const
  {
    return spacedim;
  }



  inline void *
  TriangulationWrapper::get_triangulation() const
  {
    return triangulation;
  }
} // namespace python

DEAL_II_NAMESPACE_CLOSE

#endif
