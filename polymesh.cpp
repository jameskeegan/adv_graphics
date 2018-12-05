/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "polymesh.h"
#include <math.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

PolyMesh::PolyMesh(char *file, Transform &transform)
{
  int count;
  ifstream meshfile(file);
  next = 0;

  cerr << "Opening meshfile: " << file << endl;

  if (!meshfile.is_open())
  {
    cerr << "Problem reading meshfile (not found)." << endl;
    meshfile.close();
    exit(-1);
  }

  string line;

  try {
    getline(meshfile, line);
  } catch(ifstream::failure e)
  {
    cerr << "Problem reading meshfile (getline failed)." << endl;
  }

  if (line.compare("kcply") != 0)
  {
    cerr << "Problem reading meshfile (not kcply)." << endl;
    meshfile.close();
    exit(-1);
  }

  try {
    getline(meshfile, line);
  } catch(ifstream::failure e)
  {
    cerr << "Problem reading meshfile (getline failed)." << endl;
    exit(-1);
  }

  istringstream vertex_iss(line);
  string vertex_element;
  string vertex_label;

  vertex_iss >> vertex_element >> vertex_label >> vertex_count;

  if ((vertex_element.compare("element") != 0)||(vertex_label.compare("vertex") != 0))
  {
    cerr << "Problem reading meshfile (element vertex)."<< endl;
    meshfile.close();
    exit(-1);
  }

  cerr << "Expect " << vertex_count << " vertices." << endl;
  
  try {
    getline(meshfile, line);
  } catch(ifstream::failure e)
  {
    cerr << "Problem reading meshfile (getline failed)." << endl;
    exit(-1);
  }

  istringstream triangle_iss(line);
  string triangle_element;
  string triangle_label;

  triangle_iss >> triangle_element >> triangle_label >> triangle_count;

  if ((triangle_element.compare("element") != 0)||(triangle_label.compare("face") != 0))
  {
    cerr << "Problem reading meshfile (element triangle)."<< endl;
    meshfile.close();
    exit(-1);
  }

  cerr << "Expect " << triangle_count << " triangles." << endl;

  vertex = new Vertex[vertex_count];
  
  triangle = new TriangleIndex[triangle_count];

  int i;

  for (i = 0; i < vertex_count; i += 1)
  {
    try {
      getline(meshfile, line);
    } catch(ifstream::failure e)
    {
      cerr << "Problem reading meshfile (getline failed)." << endl;
      exit(-1);
    }

    vertex_iss.clear();
    vertex_iss.str(line);

    vertex_iss >> vertex[i].x >> vertex[i].y >>vertex[i].z;

    transform.apply(vertex[i]);
  }

  for (i = 0; i < triangle_count; i += 1)
  {
    try {
      getline(meshfile, line);
    } catch(ifstream::failure e)
    {
      cerr << "Problem reading meshfile (getline failed)." << endl;
      exit(-1);
    }

    triangle_iss.clear();
    triangle_iss.str(line);
    
    triangle_iss >> count >> triangle[i][0] >> triangle[i][1] >> triangle[i][2];

    if (count != 3)
    {
      cerr << "Problem reading meshfile (non-triangle present)." << endl;
      exit(-1);
    }
  }
  
  meshfile.close();
  cerr << "Meshfile read." << endl;

  set_bounding();
}

float PolyMesh::test_edge(Vector &normal, Vertex &p, Vertex &v1, Vertex &v0)
{
  Vector edge;

  edge.x = v1.x - v0.x;
  edge.y = v1.y - v0.y;
  edge.z = v1.z - v0.z;

  Vector vp;

  vp.x = p.x - v0.x;
  vp.y = p.y - v0.y;
  vp.z = p.z - v0.z;

  Vector c;

  edge.cross(vp,c);

  return normal.dot(c);
}

void PolyMesh::triangle_intersection(Ray ray, Hit &hit, int which_triangle)
{
  Vector v0v1, v0v2;
  float t;

  hit.flag = false;

  v0v1.x = vertex[triangle[which_triangle][1]].x - vertex[triangle[which_triangle][0]].x;
  v0v1.y = vertex[triangle[which_triangle][1]].y - vertex[triangle[which_triangle][0]].y;
  v0v1.z = vertex[triangle[which_triangle][1]].z - vertex[triangle[which_triangle][0]].z;

  v0v2.x = vertex[triangle[which_triangle][2]].x - vertex[triangle[which_triangle][0]].x;
  v0v2.y = vertex[triangle[which_triangle][2]].y - vertex[triangle[which_triangle][0]].y;
  v0v2.z = vertex[triangle[which_triangle][2]].z - vertex[triangle[which_triangle][0]].z;

  Vector normal;

  v0v1.cross(v0v2, normal);
  normal.normalise();

  float ndotdir = normal.dot(ray.direction);

  if (fabs(ndotdir) < 0.00001f)
  {
    // ray is parallel to triangle so does not intersect
    return;
  }

  Vector v0;

  v0.x = vertex[triangle[which_triangle][0]].x;
  v0.y = vertex[triangle[which_triangle][0]].y;
  v0.z = vertex[triangle[which_triangle][0]].z;

  float d = normal.dot(v0);

  Vector pos;
  pos.x = ray.position.x;
  pos.y = ray.position.y;
  pos.z = ray.position.z;

  t = (normal.dot(pos) + d) / ndotdir;

  if (t < 0.0f)
  {
    // intersection is behind start of ray
    return;
  }

  Vertex p;

  p.x = ray.position.x + t * ray.direction.x;
  p.y = ray.position.y + t * ray.direction.y;
  p.z = ray.position.z + t * ray.direction.z;

  if (test_edge(normal, p,vertex[triangle[which_triangle][1]], vertex[triangle[which_triangle][0]]) < 0.0f)
  {
    return;
  }

  if (test_edge(normal, p,vertex[triangle[which_triangle][2]], vertex[triangle[which_triangle][1]]) < 0.0f)
  {
    return;
  }

  if (test_edge(normal, p,vertex[triangle[which_triangle][0]], vertex[triangle[which_triangle][2]]) < 0.0f)
  {
    return;
  }

  hit.t = t;
  hit.flag = true;
  hit.what = this;
  hit.position = p;
  hit.normal = normal;
  return;
}

void PolyMesh::intersection(Ray ray, Hit &hit)
{
  Hit current_hit;
  int i;

  hit.flag = false;

  /*
  if(bounding){
    bounding_sphere.intersection(ray, hit);

    if(hit.flag){
      // Check each triangle any find closest if any intersecion
      for(i = 0; i < triangle_count; i += 1)
      {
        triangle_intersection(ray, current_hit, i);

        if (current_hit.flag)
        {
          if (!hit.flag)
          {
            hit = current_hit;

          } else if (current_hit.t < hit.t)
          {
            hit = current_hit;
          }
        }
      }

      if (hit.flag)
      {
        if(hit.normal.dot(ray.direction) > 0.0)
        {
          hit.normal.negate();
        }
      }
    }


  }

  */


}

long double* PolyMesh::get_largest_x(long double &radius) {

  long double largest_x = -10000000;
  long double largest_y = -10000000;
  long double largest_z = -10000000;

  for(int i=0; i<vertex_count; i++){
    if(vertex[i].x > largest_x){
      largest_x = vertex[i].x;
    }
    if(vertex[i].y > largest_y){
      largest_y = vertex[i].y;
    }
    if(vertex[i].z > largest_z){
      largest_z = vertex[i].z;
    }

  }

  cerr << largest_x << endl;
  cerr << largest_y << endl;
  cerr << largest_z << endl;

  long double smallest_x = 10000000;
  long double smallest_y = 10000000;
  long double smallest_z = 10000000;

  for(int i=0; i<vertex_count; i++){
    if(vertex[i].x < smallest_x){
      smallest_x = vertex[i].x;
    }
    if(vertex[i].y < smallest_y){
      smallest_y = vertex[i].y;
    }
    if(vertex[i].z < smallest_z){
      smallest_z = vertex[i].z;
    }

  }

  cerr << smallest_x << endl;
  cerr << smallest_y << endl;
  cerr << smallest_z << endl;

  cerr << "X = " << largest_x - smallest_x << endl;
  cerr << "Y = " << largest_y - smallest_y << endl;
  cerr << "Z = " << largest_z - smallest_z << endl;

  long double differences[3] = {largest_x-smallest_x, largest_y-smallest_y, largest_z-smallest_z};
  long double r;

  long double centre_x = smallest_x + differences[0]/2;
  long double centre_y = smallest_y + differences[1]/2;
  long double centre_z = smallest_z + differences[2]/2;

  if((differences[0]>differences[1]) && (differences[0]>differences[2])){
    long double choice[2] = {smallest_x, largest_x};
    r = differences[0]/2;
  }else if((differences[1]>differences[0]) && (differences[1]>differences[2])){
    long double choice[2] = {smallest_y, largest_y};
    r = differences[1]/2;
  }else{
    long double choice[2] = {smallest_z, largest_z};
    r = differences[2]/2;
  }

  cerr << "Largest difference = " << differences[0] << endl;

  // make new sphere, radius = r, centre = midpoint of all of these

  long double centre[3] = {centre_x, centre_y, centre_z};

  bounding_sphere->center.x = centre_x;
  bounding_sphere->center.y = centre_y;
  bounding_sphere->center.z = centre_z;

  cerr << "Centre [" << centre_x << ", " << centre_y << ", " << centre_z << "]" << endl;

  cerr << "radius = " << r << endl;

  radius = r;

  // set radius, add a little extra to ensure bunny is hidden
  bounding_sphere->radius = r*1.2;

  cerr << "Centre [" << bounding_sphere->center.x << ", " << bounding_sphere->center.y << ", " << bounding_sphere->center.z << "]" << endl;


  return centre;
}

void PolyMesh::set_bounding() {
  long double radius;

  long double* centre_ords = get_largest_x(radius);


  bounding_sphere->center.x = centre_ords[0];
  bounding_sphere->center.y = centre_ords[1];
  bounding_sphere->center.z = centre_ords[2];
  bounding_sphere->radius = radius;

  bounding = true;

}
