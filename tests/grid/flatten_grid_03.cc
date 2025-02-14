// ---------------------------------------------------------------------
//
// Copyright (C) 2004 - 2018 by the deal.II authors
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

// Generate a grid, refine it once, flatten it and output the result.
//
// This test checks that we are also copying the manifold
// ids of internal faces

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/tria.h>

#include "../tests.h"

template <int dim>
void
test()
{
  deallog << dim << 'd' << std::endl;

  // Create a triangulation, then set the manifold ids of
  // non-boundary faces and, in 3d, of non-boundary edges
  Triangulation<dim> tria1;
  GridGenerator::hyper_cube(tria1);
  tria1.refine_global(1);

  for (const auto &cell : tria1.active_cell_iterators())
    for (const auto &face : cell->face_iterators())
      if (face->at_boundary() == false)
        {
          face->set_manifold_id(102);

          if (dim == 3)
            for (unsigned int l = 0; l < face->n_lines(); ++l)
              if (face->line(l)->at_boundary() == false)
                {
                  face->line(l)->set_manifold_id(202);
                }
        }


  // Then flatten the triangulation, and output the boundary and
  // manifold ids of all objects on the boundary
  Triangulation<dim> tria2;
  GridGenerator::flatten_triangulation(tria1, tria2);

  for (const auto &cell : tria2.active_cell_iterators())
    for (const auto &face : cell->face_iterators())
      if (face->at_boundary() == false)
        {
          Assert(face->boundary_id() == numbers::internal_face_boundary_id,
                 ExcInternalError());
          deallog << "Face " << face << std::endl;
          deallog << "  center = " << face->center() << std::endl;
          deallog << "  manifold id = " << face->manifold_id() << std::endl;

          if (dim == 3)
            for (unsigned int l = 0; l < face->n_lines(); ++l)
              if (face->line(l)->at_boundary() == false)
                {
                  Assert(face->line(l)->boundary_id() ==
                           numbers::internal_face_boundary_id,
                         ExcInternalError());
                  deallog << "  Edge " << face->line(l) << std::endl;
                  deallog << "    manifold id = "
                          << face->line(l)->manifold_id() << std::endl;
                }
        }

  deallog << std::endl << std::endl;
}

int
main()
{
  initlog();

  test<2>();
  test<3>();
}
