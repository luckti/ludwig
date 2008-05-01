#ifndef LCPARALLEL_CC
#define LCPARALLEL_CC

#include "LCParallel.hh"
#include "mpi.h"

#ifndef _COMM_3D_
/* See below for updated version */
  
void exchangeMomentumAndQTensor()
{
  int ix,iy,iz;
  double t0, t1;
  //extern double total_exch_;

  t0 = MPI_Wtime();

  // --------------------
  // Sends densities
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=density[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=density[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends Qxx
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qxx[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qxx[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends Qxy
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qxy[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qxy[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends Qyy
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qyy[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qyy[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends Qxz
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qxz[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qxz[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends Qyz
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qyz[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=Qyz[ix][iy][iz];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends ux
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=u[ix][iy][iz][0];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=u[ix][iy][iz][0];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends uy
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=u[ix][iy][iz][1];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=u[ix][iy][iz][1];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);

  // --------------------
  // Sends uz
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=u[ix][iy][iz][2];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      tmpBuf[iy*Lz+iz]=u[ix][iy][iz][2];
  MPI_Bsend(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);



  // --------------------
  // Receives densities
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      density[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      density[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  // --------------------
  // Receives Qxx
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qxx[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qxx[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  // --------------------
  // Receives Qxy
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qxy[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qxy[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  // --------------------
  // Receives Qyy
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qyy[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qyy[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  // --------------------
  // Receives Qxz
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qxz[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qxz[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  // --------------------
  // Receives Qyz
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qyz[ix][iy][iz]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      Qyz[ix][iy][iz]=tmpBuf[iy*Lz+iz];

  // --------------------
  // Receives ux
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      u[ix][iy][iz][0]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      u[ix][iy][iz][0]=tmpBuf[iy*Lz+iz];
  // --------------------
  // Receives uy
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      u[ix][iy][iz][1]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      u[ix][iy][iz][1]=tmpBuf[iy*Lz+iz];
  // --------------------
  // Receives uz
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      u[ix][iy][iz][2]=tmpBuf[iy*Lz+iz];
  MPI_Recv(tmpBuf,Lz*Ly,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++)
      u[ix][iy][iz][2]=tmpBuf[iy*Lz+iz];


//  MPI_Buffer_detach(&com, &buffer_size);
//  free(com);

  t1 = MPI_Wtime();
  //total_exch_ += (t1-t0);

}

void communicateOldDistributions(double ****fold)
{
  int ix,iy,iz;
  double t0, t1;

  t0 = MPI_Wtime();


  // --------------------
  // Sends fold
  ix=1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++) {
      tmpBuf[iy*Lz*5+iz*5]=fold[ix][iy][iz][3];
      tmpBuf[iy*Lz*5+iz*5+1]=fold[ix][iy][iz][8];
      tmpBuf[iy*Lz*5+iz*5+2]=fold[ix][iy][iz][9];
      tmpBuf[iy*Lz*5+iz*5+3]=fold[ix][iy][iz][12];
      tmpBuf[iy*Lz*5+iz*5+4]=fold[ix][iy][iz][13];
    }
  MPI_Bsend(tmpBuf,Lz*Ly*5,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD);
  ix=Lx2-2;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++) {
      tmpBuf[iy*Lz*5+iz*5]=fold[ix][iy][iz][1];
      tmpBuf[iy*Lz*5+iz*5+1]=fold[ix][iy][iz][7];
      tmpBuf[iy*Lz*5+iz*5+2]=fold[ix][iy][iz][10];
      tmpBuf[iy*Lz*5+iz*5+3]=fold[ix][iy][iz][11];
      tmpBuf[iy*Lz*5+iz*5+4]=fold[ix][iy][iz][14];
    }
  MPI_Bsend(tmpBuf,Lz*Ly*5,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD);


  // ========================================

  // --------------------
  // Receives fold
  MPI_Recv(tmpBuf,Lz*Ly*5,MPI_DOUBLE,rightNeighbor,0,MPI_COMM_WORLD,&status);
  ix=Lx2-1;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++) {
      fold[ix][iy][iz][3]=tmpBuf[iy*Lz*5+iz*5];
      fold[ix][iy][iz][8]=tmpBuf[iy*Lz*5+iz*5+1];
      fold[ix][iy][iz][9]=tmpBuf[iy*Lz*5+iz*5+2];
      fold[ix][iy][iz][12]=tmpBuf[iy*Lz*5+iz*5+3];
      fold[ix][iy][iz][13]=tmpBuf[iy*Lz*5+iz*5+4];
    }
  MPI_Recv(tmpBuf,Lz*Ly*5,MPI_DOUBLE,leftNeighbor,0,MPI_COMM_WORLD,&status);
  ix=0;
  for (iy=0;iy<Ly;iy++)
    for (iz=0;iz<Lz;iz++) {
      fold[ix][iy][iz][1]=tmpBuf[iy*Lz*5+iz*5];
      fold[ix][iy][iz][7]=tmpBuf[iy*Lz*5+iz*5+1];
      fold[ix][iy][iz][10]=tmpBuf[iy*Lz*5+iz*5+2];
      fold[ix][iy][iz][11]=tmpBuf[iy*Lz*5+iz*5+3];
      fold[ix][iy][iz][14]=tmpBuf[iy*Lz*5+iz*5+4];
    }

  t1 = MPI_Wtime();
  //total_comm_ += (t1-t0);

}

#else /* COMM_3D */

/****************************************************************************
 *
 *  exchangeMomentumAndQTensor
 *
 *  Three-dimensional communication
 *
 ****************************************************************************/ 

void exchangeMomentumAndQTensor() {

    extern double total_exch_;
    extern int pe_cartesian_neighbour_[2][3];
    extern MPI_Comm cartesian_communicator_;
    extern int Lx2, Ly2, Lz2;
    extern double *** density;
    extern double *** Qxx;
    extern double *** Qxy;
    extern double *** Qxz;
    extern double *** Qyy;
    extern double *** Qyz;
    extern double **** u;

    const int nquantity = 9;  /* density, Qxx, Qxy etc is 9 items */
    double * buf_sendforw;    /* send data to 'forward' direction */
    double * buf_sendback;    /* send data to 'backward' direction */
    double * buf_recvforw;    /* receive data from 'forward' direction */
    double * buf_recvback;    /* receive data from 'backward' direction */
    double   t0, t1;

    int ncount;
    int ix, iy, iz;
    int n, nforw, nback;

    const int tagb = 1002;
    const int tagf = 1003;

    MPI_Request send_request[2];
    MPI_Request recv_request[2];
    MPI_Status  send_status[2];
    MPI_Status  recv_status[2];
    MPI_Comm comm = cartesian_communicator_;

    t0 = MPI_Wtime();

    /* allocate buffers */

    ncount = nquantity*Ly2*Lz2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    /* X DIRECTION */


    nforw = pe_cartesian_neighbour_[1][0];
    nback = pe_cartesian_neighbour_[0][0];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    ix = 1;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendback[n++] = density[ix][iy][iz];
            buf_sendback[n++] =     Qxx[ix][iy][iz];
            buf_sendback[n++] =     Qxy[ix][iy][iz];
            buf_sendback[n++] =     Qyy[ix][iy][iz];
            buf_sendback[n++] =     Qxz[ix][iy][iz];
            buf_sendback[n++] =     Qyz[ix][iy][iz];
            buf_sendback[n++] =       u[ix][iy][iz][0];
            buf_sendback[n++] =       u[ix][iy][iz][1];
            buf_sendback[n++] =       u[ix][iy][iz][2];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);

    ix = Lx2-2;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendforw[n++] = density[ix][iy][iz];
            buf_sendforw[n++] =     Qxx[ix][iy][iz];
            buf_sendforw[n++] =     Qxy[ix][iy][iz];
            buf_sendforw[n++] =     Qyy[ix][iy][iz];
            buf_sendforw[n++] =     Qxz[ix][iy][iz];
            buf_sendforw[n++] =     Qyz[ix][iy][iz];
            buf_sendforw[n++] =       u[ix][iy][iz][0];
            buf_sendforw[n++] =       u[ix][iy][iz][1];
            buf_sendforw[n++] =       u[ix][iy][iz][2];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    ix = Lx2-1;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            density[ix][iy][iz] = buf_recvforw[n++];
            Qxx[ix][iy][iz]  = buf_recvforw[n++];
            Qxy[ix][iy][iz]  = buf_recvforw[n++];
            Qyy[ix][iy][iz]  = buf_recvforw[n++];
            Qxz[ix][iy][iz]  = buf_recvforw[n++];
            Qyz[ix][iy][iz]  = buf_recvforw[n++];
            u[ix][iy][iz][0] = buf_recvforw[n++];
            u[ix][iy][iz][1] = buf_recvforw[n++];
            u[ix][iy][iz][2] = buf_recvforw[n++];
        }
    }

    ix = 0;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            density[ix][iy][iz] = buf_recvback[n++];
            Qxx[ix][iy][iz]  = buf_recvback[n++];
            Qxy[ix][iy][iz]  = buf_recvback[n++];
            Qyy[ix][iy][iz]  = buf_recvback[n++];
            Qxz[ix][iy][iz]  = buf_recvback[n++];
            Qyz[ix][iy][iz]  = buf_recvback[n++];
            u[ix][iy][iz][0] = buf_recvback[n++];
            u[ix][iy][iz][1] = buf_recvback[n++];
            u[ix][iy][iz][2] = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;

    /* REPEAT FOR Y */

    /* allocate buffers */

    ncount = nquantity*Lx2*Lz2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];


    nforw = pe_cartesian_neighbour_[1][1];
    nback = pe_cartesian_neighbour_[0][1];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    iy = 1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendback[n++] = density[ix][iy][iz];
            buf_sendback[n++] =     Qxx[ix][iy][iz];
            buf_sendback[n++] =     Qxy[ix][iy][iz];
            buf_sendback[n++] =     Qyy[ix][iy][iz];
            buf_sendback[n++] =     Qxz[ix][iy][iz];
            buf_sendback[n++] =     Qyz[ix][iy][iz];
            buf_sendback[n++] =       u[ix][iy][iz][0];
            buf_sendback[n++] =       u[ix][iy][iz][1];
            buf_sendback[n++] =       u[ix][iy][iz][2];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);

    iy = Ly2-2;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendforw[n++] = density[ix][iy][iz];
            buf_sendforw[n++] =     Qxx[ix][iy][iz];
            buf_sendforw[n++] =     Qxy[ix][iy][iz];
            buf_sendforw[n++] =     Qyy[ix][iy][iz];
            buf_sendforw[n++] =     Qxz[ix][iy][iz];
            buf_sendforw[n++] =     Qyz[ix][iy][iz];
            buf_sendforw[n++] =       u[ix][iy][iz][0];
            buf_sendforw[n++] =       u[ix][iy][iz][1];
            buf_sendforw[n++] =       u[ix][iy][iz][2];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    iy = Ly2-1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            density[ix][iy][iz] = buf_recvforw[n++];
            Qxx[ix][iy][iz]  = buf_recvforw[n++];
            Qxy[ix][iy][iz]  = buf_recvforw[n++];
            Qyy[ix][iy][iz]  = buf_recvforw[n++];
            Qxz[ix][iy][iz]  = buf_recvforw[n++];
            Qyz[ix][iy][iz]  = buf_recvforw[n++];
            u[ix][iy][iz][0] = buf_recvforw[n++];
            u[ix][iy][iz][1] = buf_recvforw[n++];
            u[ix][iy][iz][2] = buf_recvforw[n++];
        }
    }

    iy = 0;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            density[ix][iy][iz] = buf_recvback[n++];
            Qxx[ix][iy][iz]  = buf_recvback[n++];
            Qxy[ix][iy][iz]  = buf_recvback[n++];
            Qyy[ix][iy][iz]  = buf_recvback[n++];
            Qxz[ix][iy][iz]  = buf_recvback[n++];
            Qyz[ix][iy][iz]  = buf_recvback[n++];
            u[ix][iy][iz][0] = buf_recvback[n++];
            u[ix][iy][iz][1] = buf_recvback[n++];
            u[ix][iy][iz][2] = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;

    /* REPEAT FOR Z */

    /* allocate buffers */

    ncount = nquantity*Lx2*Ly2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    nforw = pe_cartesian_neighbour_[1][2];
    nback = pe_cartesian_neighbour_[0][2];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    iz = 1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            buf_sendback[n++] = density[ix][iy][iz];
            buf_sendback[n++] =     Qxx[ix][iy][iz];
            buf_sendback[n++] =     Qxy[ix][iy][iz];
            buf_sendback[n++] =     Qyy[ix][iy][iz];
            buf_sendback[n++] =     Qxz[ix][iy][iz];
            buf_sendback[n++] =     Qyz[ix][iy][iz];
            buf_sendback[n++] =       u[ix][iy][iz][0];
            buf_sendback[n++] =       u[ix][iy][iz][1];
            buf_sendback[n++] =       u[ix][iy][iz][2];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);

    iz = Lz2-2;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            buf_sendforw[n++] = density[ix][iy][iz];
            buf_sendforw[n++] =     Qxx[ix][iy][iz];
            buf_sendforw[n++] =     Qxy[ix][iy][iz];
            buf_sendforw[n++] =     Qyy[ix][iy][iz];
            buf_sendforw[n++] =     Qxz[ix][iy][iz];
            buf_sendforw[n++] =     Qyz[ix][iy][iz];
            buf_sendforw[n++] =       u[ix][iy][iz][0];
            buf_sendforw[n++] =       u[ix][iy][iz][1];
            buf_sendforw[n++] =       u[ix][iy][iz][2];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    iz = Lz2-1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            density[ix][iy][iz] = buf_recvforw[n++];
            Qxx[ix][iy][iz]  = buf_recvforw[n++];
            Qxy[ix][iy][iz]  = buf_recvforw[n++];
            Qyy[ix][iy][iz]  = buf_recvforw[n++];
            Qxz[ix][iy][iz]  = buf_recvforw[n++];
            Qyz[ix][iy][iz]  = buf_recvforw[n++];
            u[ix][iy][iz][0] = buf_recvforw[n++];
            u[ix][iy][iz][1] = buf_recvforw[n++];
            u[ix][iy][iz][2] = buf_recvforw[n++];
        }
    }

    iz = 0;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            density[ix][iy][iz] = buf_recvback[n++];
            Qxx[ix][iy][iz]  = buf_recvback[n++];
            Qxy[ix][iy][iz]  = buf_recvback[n++];
            Qyy[ix][iy][iz]  = buf_recvback[n++];
            Qxz[ix][iy][iz]  = buf_recvback[n++];
            Qyz[ix][iy][iz]  = buf_recvback[n++];
            u[ix][iy][iz][0] = buf_recvback[n++];
            u[ix][iy][iz][1] = buf_recvback[n++];
            u[ix][iy][iz][2] = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;

    t1 = MPI_Wtime();
    total_exch_ += (t1-t0);

    return;
}

/*****************************************************************************
 *
 *  communicateOldDistributions
 *
 *****************************************************************************/

void communicateOldDistributions(double **** fold) {

    extern double total_comm_;
    extern int pe_cartesian_neighbour_[2][3];
    extern MPI_Comm cartesian_communicator_;
    extern int Lx2, Ly2, Lz2;

    const int nquantity = 5;   /* 5 propagating distributionss */
    double * buf_sendforw;     /* send data to 'forward' direction */
    double * buf_sendback;     /* send data to 'backward' direction */
    double * buf_recvforw;     /* receive data from 'forward' direction */
    double * buf_recvback;     /* receive data from 'backward' direction */
    double   t0, t1;
    //extern double total_comm_;

    int ncount;
    int ix, iy, iz;
    int n, nforw, nback;

    const int tagb = 1014;
    const int tagf = 1015;

    MPI_Request send_request[2];
    MPI_Request recv_request[2];
    MPI_Status  send_status[2];
    MPI_Status  recv_status[2];
    MPI_Comm    comm = cartesian_communicator_;

    t0 = MPI_Wtime();

    /* X DIRECTION */
    /* Allocate buffers */

    ncount = nquantity*Ly2*Lz2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    nforw = pe_cartesian_neighbour_[1][0];
    nback = pe_cartesian_neighbour_[0][0];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    ix = 1;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendback[n++] = fold[ix][iy][iz][3];
            buf_sendback[n++] = fold[ix][iy][iz][8];
            buf_sendback[n++] = fold[ix][iy][iz][9];
            buf_sendback[n++] = fold[ix][iy][iz][12];
            buf_sendback[n++] = fold[ix][iy][iz][13];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);


    ix = Lx2-2;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendforw[n++] = fold[ix][iy][iz][1];
            buf_sendforw[n++] = fold[ix][iy][iz][7];
            buf_sendforw[n++] = fold[ix][iy][iz][10];
            buf_sendforw[n++] = fold[ix][iy][iz][11];
            buf_sendforw[n++] = fold[ix][iy][iz][14];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    ix = Lx2-1;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            fold[ix][iy][iz][3]  = buf_recvforw[n++];
            fold[ix][iy][iz][8]  = buf_recvforw[n++];
            fold[ix][iy][iz][9]  = buf_recvforw[n++];
            fold[ix][iy][iz][12] = buf_recvforw[n++];
            fold[ix][iy][iz][13] = buf_recvforw[n++];
        }
    }

    ix = 0;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            fold[ix][iy][iz][1]  = buf_recvback[n++];
            fold[ix][iy][iz][7]  = buf_recvback[n++];
            fold[ix][iy][iz][10] = buf_recvback[n++];
            fold[ix][iy][iz][11] = buf_recvback[n++];
            fold[ix][iy][iz][14] = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;


    /* Y DIRECTION */
    /* Allocate buffers */

    ncount = nquantity*Lx2*Lz2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    /* Y DIRECTION */

    nforw = pe_cartesian_neighbour_[1][1];
    nback = pe_cartesian_neighbour_[0][1];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    iy = 1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendback[n++] = fold[ix][iy][iz][4];
            buf_sendback[n++] = fold[ix][iy][iz][9];
            buf_sendback[n++] = fold[ix][iy][iz][10];
            buf_sendback[n++] = fold[ix][iy][iz][13];
            buf_sendback[n++] = fold[ix][iy][iz][14];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);


    iy = Ly2-2;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendforw[n++] = fold[ix][iy][iz][2];
            buf_sendforw[n++] = fold[ix][iy][iz][7];
            buf_sendforw[n++] = fold[ix][iy][iz][8];
            buf_sendforw[n++] = fold[ix][iy][iz][11];
            buf_sendforw[n++] = fold[ix][iy][iz][12];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    iy = Ly2-1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            fold[ix][iy][iz][4]  = buf_recvforw[n++];
            fold[ix][iy][iz][9]  = buf_recvforw[n++];
            fold[ix][iy][iz][10]  = buf_recvforw[n++];
            fold[ix][iy][iz][13] = buf_recvforw[n++];
            fold[ix][iy][iz][14] = buf_recvforw[n++];
        }
    }

    iy = 0;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            fold[ix][iy][iz][2]  = buf_recvback[n++];
            fold[ix][iy][iz][7]  = buf_recvback[n++];
            fold[ix][iy][iz][8] = buf_recvback[n++];
            fold[ix][iy][iz][11] = buf_recvback[n++];
            fold[ix][iy][iz][12] = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;


    /* Z DIRECTION */
    /* Allocate buffers */

    ncount = nquantity*Lx2*Ly2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    nforw = pe_cartesian_neighbour_[1][2];
    nback = pe_cartesian_neighbour_[0][2];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    iz = 1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            buf_sendback[n++] = fold[ix][iy][iz][6];
            buf_sendback[n++] = fold[ix][iy][iz][11];
            buf_sendback[n++] = fold[ix][iy][iz][12];
            buf_sendback[n++] = fold[ix][iy][iz][13];
            buf_sendback[n++] = fold[ix][iy][iz][14];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);


    iz = Lz2-2;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            buf_sendforw[n++] = fold[ix][iy][iz][5];
            buf_sendforw[n++] = fold[ix][iy][iz][7];
            buf_sendforw[n++] = fold[ix][iy][iz][8];
            buf_sendforw[n++] = fold[ix][iy][iz][9];
            buf_sendforw[n++] = fold[ix][iy][iz][10];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    iz = Lz2-1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            fold[ix][iy][iz][6]  = buf_recvforw[n++];
            fold[ix][iy][iz][11]  = buf_recvforw[n++];
            fold[ix][iy][iz][12]  = buf_recvforw[n++];
            fold[ix][iy][iz][13] = buf_recvforw[n++];
            fold[ix][iy][iz][14] = buf_recvforw[n++];
        }
    }

    iz = 0;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            fold[ix][iy][iz][5]  = buf_recvback[n++];
            fold[ix][iy][iz][7]  = buf_recvback[n++];
            fold[ix][iy][iz][8] = buf_recvback[n++];
            fold[ix][iy][iz][9] = buf_recvback[n++];
            fold[ix][iy][iz][10] = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;

    t1 = MPI_Wtime();
    total_comm_ += (t1-t0);

    return;
}

/*****************************************************************************
 *
 *  exchangeTau
 *
 *****************************************************************************/

void exchangeTau() {

    extern double total_comm_;
    extern int pe_cartesian_neighbour_[2][3];
    extern MPI_Comm cartesian_communicator_;
    extern int Lx2, Ly2, Lz2;

    extern double *** tauxy;  /* X and Y exchange required */
    extern double *** tauxz;  /* X and Z exchange required */
    extern double *** tauyz;  /* Y and Z exchange required */

    const int nquantity = 2;   /* 2 components tau each direction */
    double * buf_sendforw;     /* send data to 'forward' direction */
    double * buf_sendback;     /* send data to 'backward' direction */
    double * buf_recvforw;     /* receive data from 'forward' direction */
    double * buf_recvback;     /* receive data from 'backward' direction */

    int ncount;
    int ix, iy, iz;
    int n, nforw, nback;

    const int tagb = 1016;
    const int tagf = 1017;

    MPI_Request send_request[2];
    MPI_Request recv_request[2];
    MPI_Status  send_status[2];
    MPI_Status  recv_status[2];
    MPI_Comm    comm = cartesian_communicator_;

    /* X direction - tauxy and tauxz */
    /* Allocate buffers */

    ncount = nquantity*Ly2*Lz2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    nforw = pe_cartesian_neighbour_[1][0];
    nback = pe_cartesian_neighbour_[0][0];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    ix = 1;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendback[n++] = tauxy[ix][iy][iz];
            buf_sendback[n++] = tauxz[ix][iy][iz];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);


    ix = Lx2-2;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendforw[n++] = tauxy[ix][iy][iz];
            buf_sendforw[n++] = tauxz[ix][iy][iz];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    ix = Lx2-1;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            tauxy[ix][iy][iz]  = buf_recvforw[n++];
            tauxz[ix][iy][iz]  = buf_recvforw[n++];
        }
    }

    ix = 0;
    n  = 0;

    for (iy = 0; iy < Ly2; iy++) {
        for (iz = 0; iz < Lz2; iz++) {
            tauxy[ix][iy][iz]  = buf_recvback[n++];
            tauxz[ix][iy][iz]  = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;


    /* Y DIRECTION - tauxy and tauyz */
    /* Allocate buffers */

    ncount = nquantity*Lx2*Lz2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    nforw = pe_cartesian_neighbour_[1][1];
    nback = pe_cartesian_neighbour_[0][1];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    iy = 1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendback[n++] = tauxy[ix][iy][iz];
            buf_sendback[n++] = tauyz[ix][iy][iz];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);


    iy = Ly2-2;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            buf_sendforw[n++] = tauxy[ix][iy][iz];
            buf_sendforw[n++] = tauyz[ix][iy][iz];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    iy = Ly2-1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            tauxy[ix][iy][iz]  = buf_recvforw[n++];
            tauyz[ix][iy][iz]  = buf_recvforw[n++];
        }
    }

    iy = 0;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iz = 0; iz < Lz2; iz++) {
            tauxy[ix][iy][iz]  = buf_recvback[n++];
            tauyz[ix][iy][iz]  = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;

    /* Z DIRECTION - tauxz and tauyz */
    /* Allocate buffers */

    ncount = nquantity*Lx2*Ly2;
    buf_sendforw = new double[ncount];
    buf_sendback = new double[ncount];
    buf_recvforw = new double[ncount];
    buf_recvback = new double[ncount];

    nforw = pe_cartesian_neighbour_[1][2];
    nback = pe_cartesian_neighbour_[0][2];

    /* post receives */

    MPI_Irecv(buf_recvforw, ncount, MPI_DOUBLE, nforw, tagb, comm,
              recv_request);
    MPI_Irecv(buf_recvback, ncount, MPI_DOUBLE, nback, tagf, comm,
              recv_request+1);

    /* load send buffers and non-blocking sends */

    iz = 1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            buf_sendback[n++] = tauxz[ix][iy][iz];
            buf_sendback[n++] = tauyz[ix][iy][iz];
        }
    }

    MPI_Issend(buf_sendback, ncount, MPI_DOUBLE, nback, tagb, comm,
               send_request);


    iz = Lz2-2;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            buf_sendforw[n++] = tauxz[ix][iy][iz];
            buf_sendforw[n++] = tauyz[ix][iy][iz];
        }
    }

    MPI_Issend(buf_sendforw, ncount, MPI_DOUBLE, nforw, tagf, comm,
               send_request+1);

    /* wait for receives to complete and unload buffers */

    MPI_Waitall(2, recv_request, recv_status);

    iz = Lz2-1;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            tauxz[ix][iy][iz] = buf_recvforw[n++];
            tauyz[ix][iy][iz] = buf_recvforw[n++];
        }
    }

    iz = 0;
    n  = 0;

    for (ix = 0; ix < Lx2; ix++) {
        for (iy = 0; iy < Ly2; iy++) {
            tauxz[ix][iy][iz] = buf_recvback[n++];
            tauyz[ix][iy][iz] = buf_recvback[n++];
        }
    }

    /* mop up the sends */

    MPI_Waitall(2, send_request, send_status);

    delete buf_sendforw;
    delete buf_sendback;
    delete buf_recvforw;
    delete buf_recvback;


  return;
}

#endif /* COMM_3D */

#endif
