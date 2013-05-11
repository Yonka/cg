#include <vector>
#include <set>
#include <algorithm>
#include "cg/find_path/adaptive.h"
#include "cg/find_path/edge.h"
#include "cg/primitives/point.h"

using cg::point_2;
using std::vector;
using std::set;
using std::pair;

namespace cg 
{
std::vector<point_2> find(const point_2 & pb_t, const point_2 & pe_t, const std::vector<std::vector<point_2> > & points_t);
}
