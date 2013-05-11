#include <vector>
#include <stack>

#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include "cg/find_path/find_path.h"
#include <cg/operations/orientation.h>

#include <cg/primitives/contour.h>

#include "cg/io/point.h"

using std::cout;
using std::reverse;
using cg::point_2;
using cg::point_2f;
using cg::vector_2f;

struct motion_viewer : cg::visualization::viewer_adapter
{
   motion_viewer()
   {
      st_ = first_point;
   }

   void draw(cg::visualization::drawer_type & drawer) const
   {
      size_t tmp = points_.size();
      drawer.set_color(Qt::blue);
      if (st_ > first_point)
      {
         drawer.draw_point(start, 3);
      }

      if (st_ > second_point)
      {
         drawer.draw_point(fin, 3);
      }

      if (st_ == in_building)
      {
         drawer.set_color(Qt::white);
         tmp--;
         for (size_t j = 1; j < points_[tmp].size(); j++)
         {
            drawer.draw_point(points_[tmp][j - 1], 3);
            drawer.draw_line(points_[tmp][j - 1], points_[tmp][j]);
         }
         drawer.draw_point(points_[tmp][points_[tmp].size() - 1], 3);
      }

      drawer.set_color(Qt::red);

      for (size_t i = 0; i < tmp; ++i)
      {
         for (size_t j = 1; j < points_[i].size(); j++)
         {
            drawer.draw_point(points_[i][j - 1], 3);
            drawer.draw_line(points_[i][j - 1], points_[i][j]);
         }
         drawer.draw_point(points_[i].back(), 3);
         drawer.draw_line(points_[i].front(), points_[i].back());
      }

      drawer.set_color(Qt::green);

      if (st_ == show)
      {
         for (size_t i = 1; i < path_.size(); ++i)
         {
            drawer.draw_line(path_[i - 1], path_[i]);
         } 
      } 
   }

   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "press mouse rbutton for add vertex (click to first point to complete contour)" << cg::visualization::endl
                        << "double-click to clear " << cg::visualization::endl
                        << "press Q to find shortest path" << cg::visualization::endl;
   }

   bool on_double_click(const point_2f & p)
   {
      points_.clear();
      st_ = first_point;
      return true;
   }

   bool on_press(const point_2f & p)
   {
      switch (st_)
      {
      case first_point : start = p; st_ = second_point; return true;
      case second_point : fin = p; st_ = not_in_building; return true;
      case not_in_building : points_.resize(points_.size() + 1);points_[points_.size() - 1].push_back(p); st_ = in_building; return true;
      case in_building : 
         if (points_[points_.size() - 1].size() > 1) 
         {
            if (fabs(points_[points_.size() - 1][0].x - p.x) < 15 && fabs(points_[points_.size() - 1][0].y - p.y) < 15)
            {
               st_ = not_in_building;
	       cg::contour_2 cont(points_[points_.size() - 1]);
               if (!cg::counterclockwise(cont))
               {
                  std::reverse(points_[points_.size() - 1].begin(), points_[points_.size() - 1].end());
               }

               return true;
            }
            
         }
         points_[points_.size() - 1].push_back(p);
         return true;
      default : return true;
      }
   }

   bool on_key(int key)
   {
      if (key == Qt::Key_Q && st_ == not_in_building)
      {
         st_ = show;
         path_ = cg::find(start, fin, points_);
         std::cout << start << " " << fin << "\n";
         std::cout << points_.size() << "\n";
         for (size_t i = 0; i < points_.size(); ++i)
         {
            std::cout << points_[i].size() << "\n";
            for (size_t j = 0; j < points_[i].size(); ++j)
            {
               std::cout << points_[i][j] << "\n"; 
            }
         }
      }
      return true;
   }

private:
   enum state
   {
      first_point = 0,
      second_point = 1,
      in_building = 3,
      not_in_building = 4,
      show = 5
   };
   state st_;
   point_2 start, fin;
   std::vector<std::vector<point_2> > points_;
   std::vector<point_2> path_;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   motion_viewer viewer;
   cg::visualization::run_viewer(&viewer, "motion viewer");
}
