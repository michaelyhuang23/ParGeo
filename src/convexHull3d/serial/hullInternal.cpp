//#include "convexHull3d/hull.h"
#include "convexHull3d/serialHull.h"

#include "parlay/parallel.h"
#include "parlay/sequence.h"
#include "pargeo/getTime.h"
#include "pargeo/point.h"

#include "serialHull.h"
#include "incremental.h"
#include "vertex.h"

parlay::sequence<pargeo::hullInternal::vertex>
pargeo::hullInternal::hull3dSerialInternal1(parlay::slice<pargeo::hullInternal::vertex*, pargeo::hullInternal::vertex*> Q) {
  using namespace std;
  using namespace parlay;
  using pointT = pargeo::fpoint<3>;
  using floatT = pointT::floatT;
  using facetT = facet3d<pointT>;
  using vertexT = pargeo::hullInternal::vertex;

  auto origin = pointOrigin();

  auto linkedHull = new serialHull<linkedFacet3d<vertexT>, vertexT, pointOrigin>(Q, origin);

  incrementHull3dSerial<linkedFacet3d<vertexT>, vertexT>(linkedHull);

  return linkedHull->getHullVertices<vertexT>();
}