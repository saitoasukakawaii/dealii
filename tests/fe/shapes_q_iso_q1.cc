// ---------------------------------------------------------------------
//
// Copyright (C) 2013 - 2020 by the deal.II authors
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


#include <deal.II/fe/fe_q_iso_q1.h>
#include <deal.II/fe/mapping_q1.h>

#include <string>

#include "../tests.h"

#include "shapes.h"

#define PRECISION 8


template <int dim>
void
plot_FE_Q_shape_functions()
{
  MappingQ<dim> m(1);

  FE_Q_iso_Q1<dim> q1(1);
  plot_shape_functions(m, q1, "Q1");
  plot_face_shape_functions(m, q1, "Q1");
  test_compute_functions(m, q1, "Q1");

  FE_Q_iso_Q1<dim> q2(2);
  plot_shape_functions(m, q2, "Q2_iso_Q1");
  plot_face_shape_functions(m, q2, "Q2_iso_Q1");
  test_compute_functions(m, q2, "Q2_iso_Q1");

  FE_Q_iso_Q1<dim> q1_gl(QGaussLobatto<1>(2).get_points());
  plot_shape_functions(m, q1_gl, "Q1_gl");
  plot_face_shape_functions(m, q1_gl, "Q1_gl");
  test_compute_functions(m, q1_gl, "Q1_gl");

  FE_Q_iso_Q1<dim> q2_gl(QGaussLobatto<1>(3).get_points());
  plot_shape_functions(m, q2_gl, "Q2_iso_Q1_gl");
  plot_face_shape_functions(m, q2_gl, "Q2_iso_Q1_gl");
  test_compute_functions(m, q2_gl, "Q2_iso_Q1_gl");

  // skip the following tests to
  // reduce run-time
  if (dim < 3)
    {
      FE_Q_iso_Q1<dim> q3(3);
      plot_shape_functions(m, q3, "Q3_iso_Q1");
      plot_face_shape_functions(m, q3, "Q3_iso_Q1");
      test_compute_functions(m, q3, "Q3_iso_Q1");

      FE_Q_iso_Q1<dim> q4(4);
      plot_shape_functions(m, q4, "Q4_iso_Q1");
      plot_face_shape_functions(m, q4, "Q4_iso_Q1");
      test_compute_functions(m, q4, "Q4_iso_Q1");

      FE_Q_iso_Q1<dim> q3_gl(QGaussLobatto<1>(4).get_points());
      plot_shape_functions(m, q3_gl, "Q3_iso_Q1_gl");
      plot_face_shape_functions(m, q3_gl, "Q3_iso_Q1_gl");
      test_compute_functions(m, q3_gl, "Q3_iso_Q1_gl");

      FE_Q_iso_Q1<dim> q4_gl(QGaussLobatto<1>(5).get_points());
      plot_shape_functions(m, q4_gl, "Q4_iso_Q1_gl");
      plot_face_shape_functions(m, q4_gl, "Q4_iso_Q1_gl");
      test_compute_functions(m, q4_gl, "Q4_iso_Q1_gl");
    };
}


int
main()
{
  std::ofstream logfile("output");
  deallog << std::setprecision(PRECISION) << std::fixed;
  deallog.attach(logfile);

  plot_FE_Q_shape_functions<1>();
  plot_FE_Q_shape_functions<2>();
  plot_FE_Q_shape_functions<3>();

  return 0;
}
