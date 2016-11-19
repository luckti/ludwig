/*****************************************************************************
 *
 *  phi_force_colloid.c
 *
 *  The case of force from the thermodynamic sector on both fluid and
 *  colloid via the divergence of the chemical stress.
 *
 *  In the absence of solid, the force on the fluid is related
 *  to the divergence of the chemical stress
 *
 *  F_a = - d_b P_ab
 *
 *  Note that the stress is potentially antisymmetric, so this
 *  really is F_a = -d_b P_ab --- not F_a = -d_b P_ba.
 *
 *  The divergence is discretised as, e.g., in the x-direction,
 *  the difference between the interfacial values
 *
 *  d_x P_ab ~= P_ab(x+1/2) - P_ab(x-1/2)
 *
 *  and the interfacial values are based on linear interpolation
 *  P_ab(x+1/2) = 0.5 (P_ab(x) + P_ab(x+1))
 *
 *  etc (and likewise in the other directions).
 *
 *  At an interface, we are not able to interpolate, and we just
 *  use the value of P_ab from the fluid side.
 *
 *  The stress must be integrated over the colloid surface and the
 *  result added to the net force on a given colloid.
 *
 *  The procedure ensures total momentum is conserved, ie., that
 *  leaving the fluid enters the colloid and vice versa.
 *
 *  $Id$
 *
 *  Edinburgh Soft Matter and Statistical Physics Group and
 *  Edinburgh Parallel Computing Centre
 *
 *  Kevin Stratford (kevin@epcc.ed.ac.uk)
 *  (c) 2010-2016 The University of Edinburgh
 *
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pe.h"
#include "coords.h"
#include "kernel.h"
#include "wall.h"
#include "hydro_s.h" 
#include "colloids_s.h"
#include "map_s.h"
#include "pth_s.h"
#include "phi_force_colloid.h"
#include "timer.h"

int phi_force_driver(pth_t * pth, colloids_info_t * cinfo,
		     hydro_t * hydro, map_t * map, wall_t * wall);

__global__ void phi_force_kernel(kernel_ctxt_t * ktx, pth_t * pth,
				 hydro_t * hydro, map_t * map);
__global__ void phi_force_wall_kernel(kernel_ctxt_t * ktx, pth_t * pth,
				      map_t * map, wall_t * wall,
				      double fw[3]);

/*****************************************************************************
 *
 *  phi_force_colloid
 *
 *  If no colloids, and no hydrodynamics, no action is required.
 *
 *****************************************************************************/

__host__ int phi_force_colloid(pth_t * pth, fe_t * fe, colloids_info_t * cinfo,
			       hydro_t * hydro, map_t * map, wall_t * wall) {

  int ncolloid;

  assert(pth);

  colloids_info_ntotal(cinfo, &ncolloid);

  if (hydro == NULL && ncolloid == 0) return 0;

  if (pth->method == PTH_METHOD_DIVERGENCE) {
    pth_stress_compute(pth, fe);
    phi_force_driver(pth, cinfo, hydro, map, wall);
  }

  return 0;
}

/*****************************************************************************
 *
 *  phi_force_driver
 *
 *  TODO: hydro == NULL case for relaxational dynamicss?
 *  TODO: if no wall, wall kernel not required!
 *  TODO: Fix up a kernel for the colloids.
 *
 *****************************************************************************/

__host__ int phi_force_driver(pth_t * pth, colloids_info_t * cinfo,
			      hydro_t * hydro, map_t * map, wall_t * wall) {
  int ia;
  int nlocal[3];
  dim3 nblk, ntpb;
  wall_t * wallt = NULL;
  kernel_info_t limits;
  kernel_ctxt_t * ctxt = NULL;

  /* Net momentum balance for wall */
  double fw[3] = {0.0, 0.0, 0.0};

  assert(pth);
  assert(cinfo);
  assert(hydro);
  assert(map);

  coords_nlocal(nlocal);
  wall_target(wall, &wallt);

  limits.imin = 1; limits.imax = nlocal[X];
  limits.jmin = 1; limits.jmax = nlocal[Y];
  limits.kmin = 1; limits.kmax = nlocal[Z];

  kernel_ctxt_create(NSIMDVL, limits, &ctxt);
  kernel_ctxt_launch_param(ctxt, &nblk, &ntpb);

  TIMER_start(TIMER_PHI_FORCE_CALC);

  __host_launch(phi_force_kernel, nblk, ntpb, ctxt->target,
		pth->target, hydro->target, map->target);

  __host_launch(phi_force_wall_kernel, nblk, ntpb, ctxt->target,
		pth->target, map->target, wallt, fw);

  cudaDeviceSynchronize(); 
  kernel_ctxt_free(ctxt);

  wall_momentum_add(wall, fw);

  /* A separate kernel is requred to allow reduction of the
   * force on each particle. A truly parallel version is
   pending... */

  TIMER_start(TIMER_FREE3);

#ifdef __NVCC__
  /* Get stress back! */
  pth_memcpy(pth, cudaMemcpyDeviceToHost);
#endif

  colloid_t * pc;
  colloid_link_t * p_link;

  /* All colloids, including halo */
  colloids_info_all_head(cinfo, &pc);
 
  for ( ; pc; pc = pc->nextall) {

    p_link = pc->lnk;

    for (; p_link; p_link = p_link->next) {

      if (p_link->status == LINK_FLUID) {
	int id, p;
	int cmod;

	p = p_link->p;
	cmod = cv[p][X]*cv[p][X] + cv[p][Y]*cv[p][Y] + cv[p][Z]*cv[p][Z];

	if (cmod != 1) continue;
	if (cv[p][X]) id = X;
	if (cv[p][Y]) id = Y;
	if (cv[p][Z]) id = Z;

	for (ia = 0; ia < 3; ia++) {
	  pc->force[ia] += 1.0*cv[p][id]
	    *pth->str[addr_rank2(pth->nsites, 3, 3, p_link->i, ia, id)];
	}
      }
    }
  }

  TIMER_stop(TIMER_FREE3);
  TIMER_stop(TIMER_PHI_FORCE_CALC);

  return 0;
}

/*****************************************************************************
 *
 *  phi_force_kernel
 *
 *  This computes the force on the fluid, but not the colloids.
 *
 *****************************************************************************/

__global__ void phi_force_kernel(kernel_ctxt_t * ktx, pth_t * pth,
				 hydro_t * hydro, map_t * map) {

  int kindex;
  __shared__ int kiterations;

  assert(ktx);
  assert(pth);
  assert(hydro);
  assert(map);

  kiterations = kernel_iterations(ktx);

  __target_simt_parallel_for(kindex, kiterations, 1) {

    int ic, jc, kc;
    int ia, ib;
    int index, index1;

    double pth0[3][3];
    double pth1[3][3];
    double force[3];

    ic = kernel_coords_ic(ktx, kindex);
    jc = kernel_coords_jc(ktx, kindex);
    kc = kernel_coords_kc(ktx, kindex);
    index = kernel_coords_index(ktx, ic, jc, kc);

    if (map->status[index] == MAP_FLUID) {

      /* Compute pth at current point */

      for (ia = 0; ia < 3; ia++) {
	for (ib = 0; ib < 3; ib++) {
	  pth0[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index,ia,ib)];
	}
      }
      
      /* Compute differences */
      
      index1 = kernel_coords_index(ktx, ic+1, jc, kc);
      
      if (map->status[index1] != MAP_FLUID) {
	/* Compute the fluxes at solid/fluid boundary */
	for (ia = 0; ia < 3; ia++) {
	  force[ia] = -pth0[ia][X];
	}
      }
      else {
	/* This flux is fluid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  for (ib = 0; ib < 3; ib++) {
	    pth1[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index1,ia,ib)];
	  }
	}
	for (ia = 0; ia < 3; ia++) {
	  force[ia] = -0.5*(pth1[ia][X] + pth0[ia][X]);
	}
      }
      
      index1 = kernel_coords_index(ktx, ic-1, jc, kc);
      
      if (map->status[index1] != MAP_FLUID) {
	/* Solid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  force[ia] += pth0[ia][X];
	}
      }
      else {
	/* Fluid - fluid */
	for (ia = 0; ia < 3; ia++) {
	  for (ib = 0; ib < 3; ib++) {
	    pth1[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index1,ia,ib)];
	  }
	}
	for (ia = 0; ia < 3; ia++) {
	  force[ia] += 0.5*(pth1[ia][X] + pth0[ia][X]);
	}
      }
      
      index1 = kernel_coords_index(ktx, ic, jc+1, kc);
      
      if (map->status[index1] != MAP_FLUID) {
	/* Solid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  force[ia] -= pth0[ia][Y];
	}
      }
      else {
	/* Fluid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  for (ib = 0; ib < 3; ib++) {
	    pth1[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index1,ia,ib)];
	  }
	}
	for (ia = 0; ia < 3; ia++) {
	  force[ia] -= 0.5*(pth1[ia][Y] + pth0[ia][Y]);
	}
      }
      
      index1 = kernel_coords_index(ktx, ic, jc-1, kc);
      
      if (map->status[index1] != MAP_FLUID) {
	/* Solid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  force[ia] += pth0[ia][Y];
	}
      }
      else {
	/* Fluid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  for (ib = 0; ib < 3; ib++) {
	    pth1[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index1,ia,ib)];
	  }
	}
	for (ia = 0; ia < 3; ia++) {
	  force[ia] += 0.5*(pth1[ia][Y] + pth0[ia][Y]);
	}
      }
      
      index1 = kernel_coords_index(ktx, ic, jc, kc+1);
      
      if (map->status[index1] != MAP_FLUID) {
	/* Fluid-solid */
	for (ia = 0; ia < 3; ia++) {
	  force[ia] -= pth0[ia][Z];
	}
      }
      else {
	/* Fluid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  for (ib = 0; ib < 3; ib++) {
	    pth1[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index1,ia,ib)];
	  }
	}
	for (ia = 0; ia < 3; ia++) {
	  force[ia] -= 0.5*(pth1[ia][Z] + pth0[ia][Z]);
	}
      }
      
      index1 = kernel_coords_index(ktx, ic, jc, kc-1);
      
      if (map->status[index1] != MAP_FLUID) {
	/* Fluid-solid */
	for (ia = 0; ia < 3; ia++) {
	  force[ia] += pth0[ia][Z];
	}
      }
      else {
	/* Fluid-fluid */
	for (ia = 0; ia < 3; ia++) {
	  for (ib = 0; ib < 3; ib++) {
	    pth1[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index1,ia,ib)];
	  }
	}
	for (ia = 0; ia < 3; ia++) {
	  force[ia] += 0.5*(pth1[ia][Z] + pth0[ia][Z]);
	}
      }
      
      /* Store the force on lattice */

      for (ia = 0; ia < 3; ia++) {
	hydro->f[addr_rank1(hydro->nsite, NHDIM, index, ia)] += force[ia];
      }
    }
    /* Next site */
  }

  return;
}

/*****************************************************************************
 *
 *  phi_force_wall_kernel
 *
 *  Tally contributions of the stress divergence transfered to
 *  the wall. This needs to agree with phi_force_kernel().
 *
 *  This is for accounting only; there's no dynamics.
 *
 *****************************************************************************/

__global__ void phi_force_wall_kernel(kernel_ctxt_t * ktx, pth_t * pth,
				      map_t * map, wall_t * wall,
				      double fw[3]) {

  int kindex;
  int kiterations;
  __shared__ double fx[TARGET_MAX_THREADS_PER_BLOCK];
  __shared__ double fy[TARGET_MAX_THREADS_PER_BLOCK];
  __shared__ double fz[TARGET_MAX_THREADS_PER_BLOCK];

  assert(ktx);
  assert(pth);
  assert(map);
  assert(wall);

  kiterations = kernel_iterations(ktx);

  __target_simt_parallel_region() {

    int ic, jc, kc;
    int ia, ib;
    int index, index1;
    int tid;

    double pth0[3][3];
    double fxb, fyb, fzb;

    __target_simt_threadIdx_init();
    tid = threadIdx.x;

    fx[tid] = 0.0;
    fy[tid] = 0.0;
    fz[tid] = 0.0;

    __target_simt_for(kindex, kiterations, 1) {

      ic = kernel_coords_ic(ktx, kindex);
      jc = kernel_coords_jc(ktx, kindex);
      kc = kernel_coords_kc(ktx, kindex);
      index = kernel_coords_index(ktx, ic, jc, kc);

      if (map->status[index] == MAP_FLUID) {

	/* Compute pth at current point */

	for (ia = 0; ia < 3; ia++) {
	  for (ib = 0; ib < 3; ib++) {
	    pth0[ia][ib] = pth->str[addr_rank2(pth->nsites,3,3,index,ia,ib)];
	  }
	}
      
	/* Contributions to surface stress */
      
	index1 = kernel_coords_index(ktx, ic+1, jc, kc);
      
	if (map->status[index1] == MAP_BOUNDARY) {
	  fx[tid] += -pth0[X][X];
	  fy[tid] += -pth0[Y][X];
	  fz[tid] += -pth0[Z][X];
	}
      
	index1 = kernel_coords_index(ktx, ic-1, jc, kc);
      
	if (map->status[index1] == MAP_BOUNDARY) {
	  fx[tid] += pth0[X][X];
	  fy[tid] += pth0[Y][X];
	  fz[tid] += pth0[Z][X];
	}
      
	index1 = kernel_coords_index(ktx, ic, jc+1, kc);
      
	if (map->status[index1] == MAP_BOUNDARY) {
	  fx[tid] += -pth0[X][Y];
	  fy[tid] += -pth0[Y][Y];
	  fz[tid] += -pth0[Z][Y];
	}
      
	index1 = kernel_coords_index(ktx, ic, jc-1, kc);
      
	if (map->status[index1] == MAP_BOUNDARY) {
	  fx[tid] += pth0[X][Y];
	  fy[tid] += pth0[Y][Y];
	  fz[tid] += pth0[Z][Y];
	}
      
	index1 = kernel_coords_index(ktx, ic, jc, kc+1);
      
	if (map->status[index1] == MAP_BOUNDARY) {
	  fx[tid] += -pth0[X][Z];
	  fy[tid] += -pth0[Y][Z];
	  fz[tid] += -pth0[Z][Z];
	}
      
	index1 = kernel_coords_index(ktx, ic, jc, kc-1);
      
	if (map->status[index1] == MAP_BOUNDARY) {
	  fx[tid] += pth0[X][Z];
	  fy[tid] += pth0[Y][Z];
	  fz[tid] += pth0[Z][Z];
	}      
      }
      /* Next site */
    }

    /* Reduction */

    fxb = target_block_reduce_sum_double(fx);
    fyb = target_block_reduce_sum_double(fy);
    fzb = target_block_reduce_sum_double(fz);

    if (tid == 0) {
      target_atomic_add_double(fw+X, -fxb);
      target_atomic_add_double(fw+Y, -fyb);
      target_atomic_add_double(fw+Z, -fzb);
    }
  }

  return;
}
