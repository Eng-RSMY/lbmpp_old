#include "Cell.h"


Cell::Cell(int level_, double u_, double v_, double rho_) : 
  level(level_), u(u_), v(v_), rho(rho_)
{
  parent = nullptr;
  for(int i = 0; i < 4; ++i) children[i] = nullptr;
  e = nullptr;
  ne = nullptr;
  n = nullptr;
  nw = nullptr;
  w = nullptr;
  sw = nullptr;
  s = nullptr;
  se = nullptr;
  f = 0;
  fe = 0;
  fne = 0;
  fn = 0;
  fnw = 0;
  fw = 0;
  fsw = 0;
  fs = 0;
  fse = 0;
  fbuffer = 0;
  double b, be, bne, bn, bnw, bw, bsw, bs, bse;
  // for(int i = 0; i < 8; ++i) neighbours[i] = nullptr;
  // for(int i = 0; i < 9; ++i) f[i] = 0;
  // Iniitalize f via equilibrium distribution function.
}

void Grid::recurse()
{
  if ( physical )
  {
    if ( not interface ) collide();
    explode();
    stream_all();
  }
  if ( children[0] != nullptr ) children[0]->recurse();
  if ( children[1] != nullptr ) children[1]->recurse();
  if ( children[2] != nullptr ) children[2]->recurse();
  if ( children[3] != nullptr ) children[3]->recurse();
}

void Grid::explode()
{

}

void Grid::collide()
{
  double usq = u*u;
  double vsq = v*v;
  double uv = u*v;
  double msq = usq + vsq;
  double ucomp, feq;
  // Center
  feq = WCENTER*rho*( 1 - 1.5*msq );
  f = omega*feq + (1-omega)*f;
  // East
  feq = WORTHO*rho*( 1 + 3*u + 4.5*usq - 1.5*msq );
  fe = omega*feq + (1-omega)*fe;
  // North
  feq = WORTHO*rho*( 1 + 3*v + 4.5*vsq - 1.5*msq );
  fn = omega*feq + (1-omega)*fn;
  // West
  feq = WORTHO*rho*( 1 + 3*-u + 4.5*usq - 1.5*msq );
  fw = omega*feq + (1-omega)*fw;
  // South
  feq = WORTHO*rho*( 1 + 3*-v + 4.5*vsq - 1.5*msq );
  fs = omega*feq + (1-omega)*fs;
  // Northeast
  feq = WDIAG*rho*( 1 + 3*(u+v) + 4.5*(msq + 2*uv) - 1.5*msq );
  fne = omega*feq + (1-omega)*fne;
  // Northwest
  feq = WDIAG*rho*( 1 + 3*(-u+v) + 4.5*(msq - 2*uv) - 1.5*msq );
  fnw = omega*feq + (1-omega)*fnw;
  // Southwest
  feq = WDIAG*rho*( 1 + 3*(-u-v) + 4.5*(msq + 2*uv) - 1.5*msq );
  fsw = omega*feq + (1-omega)*fsw;
  // Southeast
  feq = WDIAG*rho*( 1 + 3*(u-v) + 4.5*(msq - 2*uv) - 1.5*msq );
  fse = omega*feq + (1-omega)*fse;
}

// Recursive.
void Grid::stream_all()
{
  be = (w != nullptr) ? w->fe : 0;
  bn = (s != nullptr) ? s->fn : 0;
  bw = (e != nullptr) ? e->fw : 0;
  bs = (n != nullptr) ? n->fs : 0;
  bne = (sw != nullptr) ? sw->fne : 0;
  bnw = (se != nullptr) ? se->fnw : 0;
  bsw = (ne != nullptr) ? ne->fsw : 0;
  bse = (nw != nullptr) ? nw->fse : 0;
}

void Grid::bufferize()
{
  fe = be;
  fn = bn;
  fw = bw;
  fs = bs;
  fne = bne;
  fnw = bnw;
  fsw = bsw;
  fse = bse;
}

double Cell::get_velocity_magnitude()
{
  return sqrt(u*u + v*v);
}










