/*
 * Tiny Vector Matrix Library
 * Dense Vector Matrix Libary of Tiny size using Expression Templates
 *
 * Copyright (C) 2001 - 2006 Olaf Petzold <opetzold@users.sourceforge.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: TestXprMatrixOperators.h,v 1.2 2006-11-20 20:22:19 opetzold Exp $
 */

#ifndef TVMET_TEST_XPR_MATRIXOPS_H
#define TVMET_TEST_XPR_MATRIXOPS_H

#include <cppunit/extensions/HelperMacros.h>

#include <tvmet/Vector.h>
#include <tvmet/Matrix.h>
#include <tvmet/util/General.h>

template <class T>
class TestXprMatrixOperators : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( TestXprMatrixOperators );
  CPPUNIT_TEST( scalarOps1 );
  CPPUNIT_TEST( scalarOps2 );
  CPPUNIT_TEST( globalMatrixOps );
  CPPUNIT_TEST( negate );
  CPPUNIT_TEST( prodOps );
  CPPUNIT_TEST_SUITE_END();

private:
  typedef tvmet::Vector<T, 3>			vector_type;
  typedef tvmet::Matrix<T, 3, 3>		matrix_type;

public:
  TestXprMatrixOperators()
    : mZero(0), mOne(1), scalar(10), scalar2(2) { }

public: // cppunit interface
  /** cppunit hook for fixture set up. */
  void setUp();

  /** cppunit hook for fixture tear down. */
  void tearDown();

protected:
  void scalarOps1();
  void scalarOps2();
  void globalMatrixOps();
  void negate();
  void prodOps();

private:
  const matrix_type mZero;
  const matrix_type mOne;
  matrix_type m1;
  matrix_type mBig;	/**< matrix 10x bigger than m1 */

private:
  vector_type m1_r0, m1_r1, m1_r2;	// row vectors
  vector_type m1_c0, m1_c1, m1_c2;	// col vectors

private:
  const T scalar;
  const T scalar2;
};

/*****************************************************************************
 * Implementation Part I (cppunit part)
 ****************************************************************************/

template <class T>
void TestXprMatrixOperators<T>::setUp() {
  m1 = 1,4,7,
       2,5,8,
       3,6,9;

  m1_r0 = 1,4,7;
  m1_r1 = 2,5,8;
  m1_r2 = 3,6,9;

  m1_c0 = 1,2,3;
  m1_c1 = 4,5,6;
  m1_c2 = 7,8,9;

  mBig = 10,40,70,
         20,50,80,
         30,60,90;
}

template <class T>
void TestXprMatrixOperators<T>::tearDown() { }

/*****************************************************************************
 * Implementation Part II
 ****************************************************************************/


/*
 * global math operators with scalars
 * Note: checked against member operators since they are allready checked
 */
template <class T>
void
TestXprMatrixOperators<T>::scalarOps1() {
  matrix_type r1(m1), r2(m1);
  matrix_type t1(0), t2(0), t3(0), t4(0);

  r1 += scalar;
  r2 -= scalar;

  t1 = T(1)*m1 + scalar;
  t2 = T(1)*m1 - scalar;
  t3 = T(1)*m1 * scalar;
  t4 = T(1)*mBig / scalar;

  CPPUNIT_ASSERT( all_elements(t1 == r1) );
  CPPUNIT_ASSERT( all_elements(t2 == r2) );
  CPPUNIT_ASSERT( all_elements(t3 == mBig) );
  CPPUNIT_ASSERT( all_elements(t4 == m1) );
}


/*
 * global math operators with scalars, part II
 * Note: checked against member operators since they are allready checked
 */
template <class T>
void
TestXprMatrixOperators<T>::scalarOps2() {
  matrix_type r1(m1), r2(m1);
  matrix_type t1(0), t2(0);

  r1 += scalar;
  r2 *= scalar;

  t1 = scalar + T(1)*m1;
  t2 = scalar * (T(1)*m1);

  CPPUNIT_ASSERT( all_elements(t1 == r1) );
  CPPUNIT_ASSERT( all_elements(t2 == r2) );
}


/*
 * global math operators with matrizes
 */
template <class T>
void
TestXprMatrixOperators<T>::globalMatrixOps() {
    matrix_type t1(0), t2(0), t3(0), t4(0);

    t1 = T(1)*m1 + T(1)*m1;
    t2 = T(1)*m1 - T(1)*m1;

    {
      using namespace tvmet::element_wise;

      //t3 = m1 * mOne; // overloaded by global op*()
      t4 = T(1)*m1 / (T(1)*mOne);
    }

    CPPUNIT_ASSERT( all_elements(t1 == 2*m1) );
    CPPUNIT_ASSERT( all_elements(t2 == T(0)) );
    //CPPUNIT_ASSERT( all_elements(t3 == m1) );
    CPPUNIT_ASSERT( all_elements(t4 == m1) );
}


/*
 * negate operators with matrizes
 */
template <class T>
void
TestXprMatrixOperators<T>::negate() {
  matrix_type m1;

  m1 = -(T(1)*mOne);

  CPPUNIT_ASSERT( all_elements(m1 == T(-1)) );
}


/*
 * product functions with matrizes
 * Note: Take care on aliasing!
 */
template <class T>
void
TestXprMatrixOperators<T>::prodOps() {
  matrix_type t1, t2, t3;
  matrix_type r1, r2, r3;
  matrix_type m2(m1);;

  tvmet::util::Gemm(m1, m1, r1);
  tvmet::util::Gemm(m1, mBig, r2);
  tvmet::util::Gemm(mBig, m1, r3);
  CPPUNIT_ASSERT( all_elements(r2 == r3) );

  t1 = (T(1)*m1) * (T(1)*m2);
  CPPUNIT_ASSERT( all_elements(t1 == r1) );

  t2 = (T(1)*m1) * (T(1)*mBig);
  CPPUNIT_ASSERT( all_elements(t2 == r2) );

  t3 = (T(1))*mBig * (T(1)*m1);
  CPPUNIT_ASSERT( all_elements(t3 == r3) );
}


#endif // TVMET_TEST_XPR_MATRIXOPS_H

// Local Variables:
// mode:C++
// End:
