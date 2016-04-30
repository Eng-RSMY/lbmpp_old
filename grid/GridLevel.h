#pragma once

// Holds all cells of same dimension, 
// regardless of connectivity or spatial proximity.

#include <vector>
#include <string>
#include <iostream>
#include <omp.h>

#include "../cell/Cell.h"
#include "BoundaryConditions.h"

class GridLevel
{
public:
  // Performs one whole iteration at this grid level.
  void iteration( std::size_t relax_model, std::size_t vc_model );
  void refined_cell_bc() { bcs.refined_cell_bc(); }
  
  // Basic
  std::vector<Cell>& get_cells() { return cells; }
  const Cell* cell( std::size_t index ) const { return &cells[index]; }
  Cell& get_cell( std::size_t index ) { return cells[index]; }

  // Initialization
  void initialize( double scale_increase, double nu, double nuc, 
    char sides[4], char bc[4], double U, GridLevel* next, GridLevel* parent );
  void create_coarse_grid( 
    std::size_t cell_count_x, std::size_t cell_count_y, Cell& default_cell );

  // BCs
  BoundaryConditions* get_bcs() { return &bcs; }
  
  // Dynamic grid
  GridLevel* get_next_grid_level() { return child_grid; }
  std::vector<Cell>* get_child_grid() { return &child_grid->get_cells(); }
  std::vector<Cell>* get_parent_grid() { return &parent_grid->get_cells(); }
  std::vector<Cell>* get_grandchild_grid()
    { return &( child_grid->get_next_grid_level()->get_cells() ); }

  void refresh_active_cells();
  void set_interface( std::size_t i )
    { cells[i].state.interface = true; }
  void identify_interfaces();

  // Testing functions.
  void refine_all();
  // void refine_range(std::size_t start_index, std::size_t end_index);
  void refine_half( std::size_t i_cells, std::size_t j_cells );
  
  // Mainly for post-processing purposes.
  std::size_t get_active_cells() const { return active_cells; }
  double max_mag() const;
  double min_mag() const;
  double max_rho() const;
  double min_rho() const;
  double mag(std::size_t cell_index) const;

  // Operators
  Cell& operator[](std::size_t i){ return cells[i]; }
private:
  // Member variables.
  // Basic
  std::vector<Cell> cells;
  GridLevel* child_grid = nullptr;
  GridLevel* parent_grid = nullptr;

  // BCs
  BoundaryConditions bcs;
  
  // Quantities scaled for this grid level.
  double scale_decrease = 1; // 2^-(tree depth)
  double scale_increase = 1; // 2^(tree depth)
  double tau = 0; // relaxation time.
  double omega = 0; // inverse of tau. relaxation frequency.
  double nu = 0; // viscosity.
  double nuc = 0; // viscosity buffer.
  
  // Dynamic grid
  std::size_t active_cells = 0;
  
  // Member functions.
  // Basic.
  Cell& get_neighbour( std::size_t ci, std::size_t ni ) 
    { return cells[ cells[ci].local.neighbours[ni] ];}

  // Iteration
  void reconstruct_macro();
  void bufferize_parallel();
  void stream_parallel();

  // Dynamic grid
  void link_marked();
  void refine_marked();
  std::size_t compute_active_cells() const;

  // VC
  void stream_body_force_parallel();
  void bufferize_body_force_parallel();
  void apply_advected_vc_body_force( 
    double omega, double scale_decrease, double scale_increase, double nuc );
};
