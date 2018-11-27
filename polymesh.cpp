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

  // Check each triangle any find closest if any intersecion

  for(i = 0; i < triangle_count; i += 1)
  {
    triangle_intersection(ray, current_hit, i);

    if (current_hit.flag != false)
    {
      if (hit.flag == false)
      {
	hit = current_hit;

      } else if (current_hit.t < hit.t)
      {
        hit = current_hit;
      }
    }
  }

  if (hit.flag == true)
  {
    if(hit.normal.dot(ray.direction) > 0.0)
    {
      hit.normal.negate();
    }
  }
}
