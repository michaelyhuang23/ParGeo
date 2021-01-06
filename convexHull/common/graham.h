// Copyright (c) 2020 Yiqiu Wang and the Pargeo Team
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef GRAHAM_H
#define GRAHAM_H

#include "pbbs/sampleSort.h"
#include "pbbs/gettime.h"
#include "geometry.h"
#include "hull.h"

_seq<intT> grahamScanSerial(point2d* P, intT n, intT* I=NULL) {
  static const bool verbose = false;
  /* auto angle = [&](point2d& a, point2d& b, point2d& c) { */
  /* 		 point2d ab = b-a; */
  /* 		 point2d bc = c-b; */
  /* 		 return acos(ab.dot(bc) / (ab.length()*bc.length())); */
  /* 	       }; */
  auto rightTurn = [&](point2d& a, point2d& b, point2d& c) {
    auto cross = (b.x()-a.x())*(c.y()-a.y()) - (b.y()-a.y())*(c.x()-a.x());
    return cross <= 0;
  };

  if (!I) {
    I = newA(intT, n);
  }
  intT m=0;

  timing t; t.start();
  auto findLeft = [&](intT i) {return P[i].x();};
  intT si = sequence::minIndexSerial<floatT>(0, n, findLeft);
  point2d s = P[si];
  cout << "init-time = " << t.next() << endl;

  auto sp = point2d(s.x(), s.y()-1);
  auto angleLess = [&](point2d& a, point2d& b) {
    return rightTurn(s, a, b);
  };
  swap(P[0], P[si]);
  sampleSort(&P[1], n-1, angleLess);
  I[m++] = 0;
  I[m++] = 1;
  //cout << "initial = " << P[I[0]] << ", " << P[I[1]] << endl;
  cout << "sort-time = " << t.next() << endl;

  intT pushes = 0;
  intT pops = 0;
  
  auto push = [&](intT i) {I[m++] = i;pushes++;};
  auto pop = [&]() {m--;pops++;};
  auto isEmpty = [&]() {return m==0;};

  for (intT i=2; i<n; ++i) {
    if (!rightTurn(P[I[m-2]], P[I[m-1]], P[i])) {
      pop();
      while (!rightTurn(P[I[m-2]], P[I[m-1]], P[i])) pop();
    }
    push(i);
  }
  cout << "pushes = " << pushes << endl;
  cout << "pops = " << pops << endl;
  cout << "scan-time = " << t.stop() << endl;

  return _seq<intT>(I, m);
}

#endif