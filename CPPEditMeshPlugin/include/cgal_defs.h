#ifndef CGAL_DEFS_H
#define CGAL_DEFS_H


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh_deformation.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>

#include <CGAL/Polygon_mesh_processing/locate.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>


namespace PMP = CGAL::Polygon_mesh_processing;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point3;
typedef Kernel::Direction_3 Direction3;
typedef Kernel::Vector_3 Vector3;
typedef Kernel::FT FT;
typedef Kernel::Ray_3 Ray;
typedef Kernel::Line_3 Line3;
typedef Kernel::Triangle_3 Triangle;


typedef CGAL::Surface_mesh<Point3> Mesh;
typedef Mesh::Vertex_index Vertex_index;
typedef Mesh::Face_index Face_index;
typedef Mesh::Halfedge_index Halfedge_index;
typedef Mesh::Edge_index Edge_index;

typedef CGAL::Surface_mesh_deformation<Mesh> Deformation;


typedef CGAL::AABB_face_graph_triangle_primitive<Mesh> MeshPrimitive;
typedef CGAL::AABB_traits<Kernel, MeshPrimitive> AABB_face_graph_traits;
typedef CGAL::AABB_tree<AABB_face_graph_traits> Tree;

typedef PMP::Barycentric_coordinates<FT> Barycentric_coordinates;
typedef PMP::Face_location<Mesh, FT> Face_location;


typedef boost::property_map<Mesh, CGAL::vertex_point_t>::type  VPMap;
typedef Mesh::template Property_map<Vertex_index, Vector3> VNMap;


#endif