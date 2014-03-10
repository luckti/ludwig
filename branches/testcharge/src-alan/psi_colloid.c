/*****************************************************************************
 *
 *  psi_colloid.c
 *
 *****************************************************************************/

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "util.h"
#include "coords.h"
#include "psi.h"
#include "colloids.h"

static int psi_colloid_charge_accum(psi_t * psi,
                                    int index, double * rho, double * weight);

/*****************************************************************************
 *
 *  psi_colloid_rho_set
 *
 *  Distribute the total charge of the colloid to the lattice sites
 *  depending on the current discrete volume.
 *
 *  It is envisaged that this is called before a halo exchange of
 *  the lattice psi->rho values. However, the process undertaken
 *  here could be extended into the halo regions if a halo swap
 *  is not required for other reasons.
 *
 *  A more effiecient version would probably compute and store
 *  1/volume for each particle in advance before trawling through
 *  the lattice.
 *
 *****************************************************************************/

int psi_colloid_rho_set(psi_t * obj) {

  int ic, jc, kc, index;
  int nlocal[3];

  double rho0, rho1, volume;
  colloid_t * pc = NULL;

  assert(obj);

  coords_nlocal(nlocal);

  for (ic = 1; ic <= nlocal[X]; ic++) {
    for (jc = 1; jc <= nlocal[Y]; jc++) {
      for (kc = 1; kc <= nlocal[Z]; kc++) {

	index = coords_index(ic, jc, kc);
	pc = colloid_at_site_index(index);

	if (pc) {
	  util_discrete_volume_sphere(pc->s.r, pc->s.a0, &volume);
	  rho0 = pc->s.q0/volume;
	  rho1 = pc->s.q1/volume;
	  psi_rho_set(obj, index, 0, rho0);
	  psi_rho_set(obj, index, 1, rho1);
	}

	/* Next site */
      }
    }
  }

  return 0;
}

/*****************************************************************************
 *
 *  psi_colloid_electroneutral
 *
 *  To ensure overall electroneutrality, we consider the following:
 *
 *    (1) assume some number of colloids has been initialised, each
 *    with a given charge.
 *    (2) assume the fluid is initialised to be overall electroneutral
 *    independently of the presence of colloids.
 *
 *  We can then:
 *
 *    (1) compute the total charge for the colloids, along with the
 *    total discrete solid volume.
 *    (2) Add the appropriate countercharge to the fluid.
 *
 *  Note:
 *    (1) net colloid charge \sum_k z_k q_k is computed for k = 2.
 *    (2) the countercharge is distributed only in one fluid species.
 *
 *  This is a collective call in MPI.
 *
 *****************************************************************************/

int psi_colloid_electroneutral(psi_t * obj) {

  int ic, jc, kc, index;
  int nlocal[3];

  int n, nk;
  int nc;                    /* Species for countercharge */
  int valency[2];
  double qvlocal[3], qv[3];  /* q[2] and v for a single Allreduce */
  double qtot;               /* net colloid charge */
  double vf;                 /* total volume of fluid */
  double rho, rhoi;          /* charge and countercharge densities */

  MPI_Comm comm;

  psi_nk(obj, &nk);
  assert(nk == 2);

  comm = cart_comm();
  colloids_q_local(qvlocal);
  colloids_v_local(qvlocal + 2);

  MPI_Allreduce(qvlocal, qv, 3, MPI_DOUBLE, MPI_SUM, comm);

  /* Volume of fluid, assuming no other solid is present */

  vf = L(X)*L(Y)*L(Z) - qv[2];

  /* Net colloid charge is 'qtot'; the required countercharge density
   * is 'rhoi' */

  qtot = 0.0;
  for (n = 0; n < nk; n++) {
    psi_valency(obj, n, valency + n);
    qtot += valency[n]*qv[n];
  }

  rhoi = fabs(qtot) / vf;

  /* Depending on the sign of qtot, the counter charge needs to
   * be put into the 'other' species 'nc' */

  nc = -1;
  if (qtot*valency[0] >= 0) nc = 1;
  if (qtot*valency[1] >= 0) nc = 0;
  assert(nc == 0 || nc == 1);

  /* Loop over lattice and accumulate the countercharge */

  coords_nlocal(nlocal);

  for (ic = 1; ic <= nlocal[X]; ic++) {
    for (jc = 1; jc <= nlocal[Y]; jc++) {
      for (kc = 1; kc <= nlocal[Z]; kc++) {

	index = coords_index(ic, jc, kc);

	if (colloid_at_site_index(index) == NULL) {
	  psi_rho(obj, index, nc, &rho);
	  rho += rhoi;
	  psi_rho_set(obj, index, nc, rho);
	}

	/* Next site */
      }
    }
  }

  return 0;
}

/*****************************************************************************
 *
 *  psi_colloid_remove_charge
 *
 *  Accumulate charge densities removed from previously fluid site index.
 *  This is for nk = 2 only.
 *
 *****************************************************************************/

int psi_colloid_remove_charge(psi_t * psi, colloid_t * colloid, int index) {

  double rho0, rho1;

  assert(psi);
  assert(colloid);

  psi_rho(psi, index, 0, &rho0);
  colloid->s.deltaq0 += rho0;
  psi_rho(psi, index, 1, &rho1);
  colloid->s.deltaq1 += rho1;

  return 0;
}

/*****************************************************************************
 *
 *  psi_colloid_replace_charge
 *
 *  Compute charge densities around newly fluid site at index. This is
 *  based on an average of surrounding fluid sites.
 *
 *****************************************************************************/

int psi_colloid_replace_charge(psi_t * psi, colloid_t * colloid, int index) {

  int n, nk;
  int xs, ys, zs;
  double rho[2];
  double weight;

  assert(psi);
  assert(colloid);

  psi_nk(psi, &nk);
  assert(nk == 2);

  coords_strides(&xs, &ys, &zs);

  weight = 0.0;
  for (n = 0; n < nk; n++) {
    rho[n] = 0.0;
  }

  /* Look at SIX neighbours */

  psi_colloid_charge_accum(psi, index - xs, rho, &weight); 
  psi_colloid_charge_accum(psi, index + xs, rho, &weight);
  psi_colloid_charge_accum(psi, index - ys, rho, &weight);
  psi_colloid_charge_accum(psi, index + ys, rho, &weight);
  psi_colloid_charge_accum(psi, index - zs, rho, &weight);
  psi_colloid_charge_accum(psi, index + zs, rho, &weight);

  /* Add the resultant value to the new fluid site */

  assert(weight > 0.0);

  weight = 1.0 / weight;
  for (n = 0; n < nk; n++) {
    rho[n] *= weight;
    psi_rho_set(psi, index, n, rho[n]);
  }

  /* Set corrections arising from addition of charge density to fluid */

  colloid->s.deltaq0 -= rho[0];
  colloid->s.deltaq1 -= rho[1];

  return 0;
}


/*****************************************************************************
 *
 *  psi_colloid_charge_accum
 *
 *  Accumulate charge densities from site index, along with a
 *  counter of fluid sites 'weight'.
 *
 *  Solid (porous media, wall) sites are excluded by virtue of
 *  never being occupied by a colloid. The site must have been
 *  fluid at the previous step.
 *
 *  This assumes always two charge densities.
 *
 *****************************************************************************/

static int psi_colloid_charge_accum(psi_t * psi,
                                    int index, double * rho, double * weight) {
  int n, nk = 2;
  double rho0;
  colloid_t * pc = NULL;
  colloid_t * colloid_at_site_index(int);

  assert(psi);

  assert(rho);
  assert(weight);

  pc = colloid_at_site_index(index);

  if (pc == NULL) {
    for (n = 0; n < nk; n++) {
      psi_rho(psi, index, n, &rho0);
      *(rho + n) += rho0;
    }
    *weight += 1.0;
  }

  return 0;
}

/*****************************************************************************
 *
 *  psi_colloid_zetapotential
 *
 *  Compute zeta potential, intended for cases where a single
 *  colloid is present.
 *
 *****************************************************************************/

int psi_colloid_zetapotential(psi_t * obj, double * psi_zeta) {

  int ic, jc, kc;
  int index, index1;
  int nlocal[3];

  int nsl_local, nsl_total; /* number of local and total nearest neighbour surface links */

  double psi0; /* potential at fluid site */
  double psi1; /* potential at adjacent solid site */
  double psic_local, psic_total; /* local and global cummulative potential */

  colloid_t * p_c;
  colloid_t * colloid_at_site_index(int);

  MPI_Comm comm = cart_comm();

  assert(obj);
  coords_nlocal(nlocal);

  /* Set result to zero and escape if there is not one particle. */

  *psi_zeta = 0.0;
  if (colloid_ntotal() != 1) return 0;

  nsl_local = 0;
  nsl_total = 0;

  psic_local = 0.0;
  psic_total = 0.0;

  for (ic = 1; ic <= nlocal[X]; ic++) {
    for (jc = 1; jc <= nlocal[Y]; jc++) {
      for (kc = 1; kc <= nlocal[Z]; kc++) {

	index = coords_index(ic, jc, kc);

	/* If this is a solid site, there's no contribution here. */
	p_c = colloid_at_site_index(index);
	if (p_c) continue;

	/* Get potential at fluid site */
	psi_psi(obj, index, &psi0);

	/* Check if adjacent site is solid and add contribution */

	index1 = coords_index(ic+1, jc, kc);
	p_c = colloid_at_site_index(index1);

	if (p_c) {
	  psi_psi(obj, index1, &psi1);
	  psic_local += 0.5*(psi0+psi1);
	  nsl_local ++;
	}

	index1 = coords_index(ic-1, jc, kc);
	p_c = colloid_at_site_index(index1);

	if (p_c) {
	  psi_psi(obj, index1, &psi1);
	  psic_local += 0.5*(psi0+psi1);
	  nsl_local ++;
	}

	index1 = coords_index(ic, jc+1, kc);
	p_c = colloid_at_site_index(index1);

	if (p_c) {
	  psi_psi(obj, index1, &psi1);
	  psic_local += 0.5*(psi0+psi1);
	  nsl_local ++;
	}

	index1 = coords_index(ic, jc-1, kc);
	p_c = colloid_at_site_index(index1);

	if (p_c) {
	  psi_psi(obj, index1, &psi1);
	  psic_local += 0.5*(psi0+psi1);
	  nsl_local ++;
	}
	
	index1 = coords_index(ic, jc, kc+1);
	p_c = colloid_at_site_index(index1);

	if (p_c) {
	  psi_psi(obj, index1, &psi1);
	  psic_local += 0.5*(psi0+psi1);
	  nsl_local ++;
	}

	index1 = coords_index(ic, jc, kc-1);
	p_c = colloid_at_site_index(index1);

	if (p_c) {
	  psi_psi(obj, index1, &psi1);
	  psic_local += 0.5*(psi0+psi1);
	  nsl_local ++;
	}

	/* Next site */
      }
    }
  }

  MPI_Reduce(&nsl_local, &nsl_total, 1, MPI_INT, MPI_SUM, 0, comm);
  MPI_Reduce(&psic_local, &psic_total, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

  if (nsl_total > 0) psi_zeta[0] = psic_total/nsl_total;

  return 0;
}