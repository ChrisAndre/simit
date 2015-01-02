#include "simit-test.h"

#include "graph.h"
#include "program.h"
#include "function.h"
#include "error.h"

using namespace std;
using namespace simit;

TEST(System, misc_triangle) {
  simit::Set<> verts;
  simit::FieldRef<simit_float> b = verts.addField<simit_float>("b");

  ElementRef v0 = verts.add();
  ElementRef v1 = verts.add();
  ElementRef v2 = verts.add();
  ElementRef v3 = verts.add();

  simit::Set<3> trigs(verts,verts,verts);
  simit::FieldRef<simit_float> a = trigs.addField<simit_float>("a");

  ElementRef t0 = trigs.add(v0,v1,v2);
  ElementRef t1 = trigs.add(v1,v2,v3);

  a.set(t0, 1.0);
  a.set(t1, 0.1);

 // Compile program and bind arguments
  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();

  f->bind("verts", &verts);
  f->bind("trigs", &trigs);

  f->runSafe();

  // Check outputs
  ASSERT_FLOAT_EQ(1.0, b.get(v0));
  ASSERT_FLOAT_EQ(1.1, b.get(v1));
  ASSERT_FLOAT_EQ(1.1, b.get(v2));
  ASSERT_FLOAT_EQ(0.1, b.get(v3));
}

TEST(System, DISABLED_misc_assemble_from_literal_vector) {
  // Points
  Set<> points;
  FieldRef<double> x = points.addField<double>("x");

  ElementRef p0 = points.add();
  ElementRef p1 = points.add();
  ElementRef p2 = points.add();

  x.set(p0, {0.0});
  x.set(p1, {0.0});

  // Springs
  Set<2> springs(points,points);

  springs.add(p0,p1);
  springs.add(p1,p2);

  // Compile program and bind arguments
  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();

  f->bind("points", &points);
  f->bind("springs", &springs);

  f->runSafe();

  std::cout << x << std::endl;

  // Check that outputs are correct
  ASSERT_EQ(1.0, x.get(p0));
  ASSERT_EQ(1.0, x.get(p1));
}

TEST(System, DISABLED_misc_map_one_set) {
  // Points
  Set<> points;
  FieldRef<simit_float> a = points.addField<simit_float>("a");

  ElementRef p0 = points.add();
  ElementRef p1 = points.add();
  ElementRef p2 = points.add();

  a.set(p0, 1.0);
  a.set(p1, 2.0);
  a.set(p2, 3.0);

  // Compile program and bind arguments
  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();

  f->bind("points", &points);
  f->runSafe();

  // Check outputs
  ASSERT_FLOAT_EQ(2, a.get(p0));
  ASSERT_FLOAT_EQ(4, a.get(p1));
  ASSERT_FLOAT_EQ(6, a.get(p2));
}
