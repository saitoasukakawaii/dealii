// ---------------------------------------------------------------------
//
// Copyright (C) 2019 - 2020 by the deal.II authors
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

// Test the functionality of the ParsedConvergenceTable class for differences
// between finite element solutions

#include <deal.II/base/function_lib.h>
#include <deal.II/base/parsed_convergence_table.h>

#include <deal.II/dofs/dof_handler.h>

#include <deal.II/fe/fe_q.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria.h>

#include <deal.II/lac/vector.h>

#include <deal.II/numerics/vector_tools.h>

#include <map>

#include "../tests.h"

int
main()
{
  initlog();

  ParsedConvergenceTable table;

  ParameterHandler prm;
  table.add_parameters(prm);
  prm.set("Rate key", "");
  prm.set("Rate mode", "none");

  Triangulation<2> tria;
  GridGenerator::hyper_cube(tria);

  FE_Q<2>       fe(1);
  DoFHandler<2> dh(tria);

  Functions::CosineFunction<2> exact(1);

  for (unsigned int i = 0; i < 5; ++i)
    {
      tria.refine_global(1);
      dh.distribute_dofs(fe);
      Vector<double> sol(dh.n_dofs());
      Vector<double> sol2(dh.n_dofs());
      VectorTools::interpolate(dh, exact, sol);
      sol2 = 1e-3;
      sol2 -= sol;
      table.difference(dh, sol, sol2);
    }
  table.output_table(deallog.get_file_stream());
}
