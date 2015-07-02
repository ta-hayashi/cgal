#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/boost/graph/properties_Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>

#include <boost/property_map/property_map.hpp>

#include <fstream>
#include <map>

typedef CGAL::Simple_cartesian<double>                               K;
typedef K::Point_3                                                   Point;
typedef CGAL::Surface_mesh<Point>                                    Triangle_mesh;

typedef boost::graph_traits<Triangle_mesh>::vertex_descriptor        vertex_descriptor;
typedef boost::graph_traits<Triangle_mesh>::vertex_iterator          vertex_iterator;
typedef boost::graph_traits<Triangle_mesh>::halfedge_descriptor      halfedge_descriptor;

typedef CGAL::Mean_curvature_flow_skeletonization<Triangle_mesh>     Skeletonization;
typedef Skeletonization::Skeleton                                    Skeleton;

typedef boost::graph_traits<Skeleton>::vertex_descriptor             vertex_desc;
typedef boost::graph_traits<Skeleton>::vertex_iterator               vertex_iter;
typedef boost::graph_traits<Skeleton>::edge_iterator                 edge_iter;


int main(int argc, char* argv[])
{
  std::ifstream input((argc>1)?argv[1]:"data/elephant.off");
  Triangle_mesh mesh;
  input >> mesh;

  Skeleton skeleton;

  Skeletonization mcs(mesh);

  // 1. Contract the mesh by mean curvature flow.
  mcs.contract_geometry();

  // 2. Collapse short edges and split bad triangles.
  mcs.remesh();

  // 3. Fix degenerate vertices.
  mcs.detect_degeneracies();

  // Perform the above three steps in one iteration.
  mcs.contract();

  // Iteratively apply step 1 to 3 until convergence.
  mcs.contract_until_convergence();

  // Convert the contracted mesh into a curve skeleton and 
  // get the correspondent surface points
  mcs.convert_to_skeleton(skeleton);

  std::cout << "vertices: " << num_vertices(skeleton) << "\n";
  std::cout << "edges: " << num_edges(skeleton) << "\n";

  // Output all the edges.
  edge_iter ei, ei_end;
  for (boost::tie(ei, ei_end) = edges(skeleton); ei != ei_end; ++ei)
  {
    const Point& s = skeleton[source(*ei, skeleton)].point;
    const Point& t = skeleton[target(*ei, skeleton)].point;
    std::cout << s << " " << t << "\n";
  }

  // Output skeletal points and the corresponding surface points.
  BOOST_FOREACH(vertex_desc i, vertices(skeleton))
  {
    const Point& skel = skeleton[i].point;
    std::cout << skel << ": ";

    for (size_t j = 0; j < skeleton[i].vertices.size(); ++j)
    {
      const Point& surf = mesh.point(skeleton[i].vertices[j]);
      std::cout << surf << " ";
    }
    std::cout << "\n";
  }
  
  return 0;
}

