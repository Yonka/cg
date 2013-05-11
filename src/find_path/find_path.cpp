#include "cg/find_path/find_path.h"

vector<edge> edges;
vector<my_point> points;
vector<int> order;
vector<vector<int> > graph_edge;
my_point _cl, _dir;
mpq_class sq_dist(my_point p0, my_point p1)
{
    mpq_class x0(p0.x), y0(p0.y), x1(p1.x), y1(p1.y);
    return ((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

mpf_class dist(my_point p0, my_point p1)
{
    mpf_class x0(p0.x), y0(p0.y), x1(p1.x), y1(p1.y);
    return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

class angle_sort
{
    my_point o, d;

public:
    angle_sort(const my_point &o_, const my_point &d_): o(o_), d(d_) {}

    bool cmp(my_point a, my_point b) const
    {
        int v1 = vp(d, a, o);
        int v2 = vp(d, b, o);
        if (v1 == v2 || v1 * v2 == 0)
        {
            int v = vp(a, b, o);
            if (v == 0 || v1 == 0 || v2 == 0)
            {
                int s1 = sp(d, a, o);
                int s2 = sp(d, b, o);
                if (v != 0)
                {
                    if (v1 == 0)
                    {
                        if (s1 > 0)
                            return true;
                        else
                            return v < 0;
                    }
                    if (v2 == 0)
                    {
                        if (s2 > 0)
                            return false;
                        else
                            return v < 0;
                    }
                }
                if (s1 == s2)
                    return (sq_dist(o, a) < sq_dist(o, b));
                else
                    return (s1 >= 0);// ? true : false;
            }
            else
                return (v < 0);
        }
        else
            return (v1 < v2);
    }

    bool operator()(int t1, int t2) const
    {
        my_point a = points[t1];
        my_point b = points[t2];
        return cmp(a, b);
    }
};

struct edge_sort
{
    bool operator()(const edge &e1, const edge &e2) const
    {
        my_point p1 = e1.first();
        my_point p2 = e2.first();
        if (!(p1 == p2))
        {
            if (e1.second() == p2)
                p1 = e1.second();
            else
            {
                p2 = e2.second();
                if (!(p1 == p2))
                    p1 = e1.second();
            }
        }
        if (p1 == p2)
        {
            my_point tmp_o = p1;
            p1 = e1.another(p1);
            p2 = e2.another(p2);
            if (p1 == p2)
                return false;
            return (vp(p1, p2, tmp_o) * vp(_cl, p1, tmp_o) > 0);
        }

        int v1 = vp(_cl, e1.first(), e1.second());
        int v2 = vp(_cl, e2.first(), e2.second());

        my_point a = e1.first(), b = e1.second(), c = e2.first(), d = e2.second();

        if (v1 == -1)
            std::swap(a, b);
        if (v2 == -1)
            std::swap(c, d);
        v1 = vp(a, b, c);
        v2 = vp(a, b, d);
        int v3 = vp(c, d, a);
        int v4 = vp(c, d, b);
        return ((v1 < 0 && v2 < 0) || (v3 > 0 && v4 > 0));
    }
};

void fill_order()
{
    for (size_t i = 0; i < order.size(); i++)
        order[i] = i;
}

void add(int i, set<edge, edge_sort>& status)
{
    status.insert(edges[i]);
}

void fill_status(const my_point &o, const my_point &d, set<edge, edge_sort> & status)
{
    for (size_t i = 0; i < edges.size(); i++)
    {
        if (edges[i].first() == o || edges[i].second() == o)
            continue;
        int v1 = vp(d, edges[i].first(), o);
        int v2 = vp(d, edges[i].second(), o);
        int s1 = sp(d, edges[i].first(), o);
        int s2 = sp(d, edges[i].second(), o);
        if (v1 == 0 && v2 == 0)
            continue;
        int v = vp(edges[i].first(), edges[i].second(), o);
        if (v1 != 1)
            v *= -1;
        if (v1 * v2 == -1 && v == -1)
            add(i, status);
        if ((v1 == 0 && s1 >= 0 && v2 == 1) ||
            (v2 == 0 && s2 >= 0 && v1 == 1))
            add(i, status);
    }
}

void modify(set<edge, edge_sort> & status, my_point p, my_point o)
{
    edge p1 = edges[p.num_edge_1()];
    edge p2 = edges[p.num_edge_2()];
    int v1 = vp(p, p1.another(p), o);
    int v2 = vp(p, p2.another(p), o);
    if (v1 > 0)
        status.erase(p1);
    if (v2 > 0)
        status.erase(p2);
    if (v1 < 0)
        status.insert(p1);
    if (v2 < 0)
        status.insert(p2);
}

void viz_vertex(my_point p, int num, my_point com_edge)
{
    _cl = p;
    angle_sort a(p, _dir);
    sort(order.begin(), order.end(), a);
    set<edge, edge_sort> status;
    fill_status(p, _dir, status);
    bool viz = false;
    for (size_t i = 0; i < order.size(); i++)
    {
        _dir = points[order[i]];
        if (_dir == com_edge)
        {
            graph_edge[num].push_back(order[i]);
            break;
        }
        if (status.size() == 0)
        viz = true;
        else
        {
            edge tt = *status.begin();
            edge_sort tc;
            if (tc(tt, edges[_dir.num_edge_1()]))//dist(*status.begin()) < sq_dist(p, points[order[i]]))
               viz = false;
           else
               viz = true;
        }
        if (viz)
        {
            graph_edge[num].push_back(order[i]);
        }
        modify(status, points[order[i]], p);
    }
}

void viz_graph()
{
    for (size_t i = 0; i < points.size(); i++)
    {
        fill_order();
        std::swap(order[order.size() - 1], order[i]);
        order.pop_back();
        _dir = edges[points[i].num_edge_2()].another(points[i]);
        viz_vertex(points[i], i, edges[points[i].num_edge_1()].another(points[i]));
        order.push_back(i);
    }
}
namespace cg {
std::vector<point_2> find(const point_2 & pb_t, const point_2 & pe_t, const std::vector<std::vector<point_2> > & points_t)
{
    edges.clear();
    points.clear();
    order.clear();
    graph_edge.clear();
    my_point pb = my_point(pb_t.x, pb_t.y);
    my_point pe = my_point(pe_t.x, pe_t.y);
    int n, m;
    n = points_t.size();
    for (int i = 0; i < n; i ++)
    {
        m = points_t[i].size();
        points.push_back(my_point(points_t[i][0].x, points_t[i][0].y, edges.size() + m - 1, edges.size()));
        for (int j = 1; j < m; j++)
        {
            points.push_back(my_point(points_t[i][j].x, points_t[i][j].y, edges.size(), edges.size() + 1));
            edges.push_back(edge(points[points.size() - 1], points[points.size() - 2]));
        }
        edges.push_back(edge(points[points.size() - 1], points[points.size() - m]));
    }
    order.resize(points.size());
    graph_edge.resize(points.size() + 2);

//add polygons
    viz_graph();

//add two vertices
    int sz = points.size();
    _dir = my_point(pe.x, pe.y + 2);
    viz_vertex(pe, sz, pe);
    for (size_t i = 0; i < graph_edge[sz].size(); i++)
    {
        graph_edge[graph_edge[sz][i]].push_back(sz);
    }
    pe = my_point(pe.x, pe.y, edges.size(), edges.size());
    points.push_back(pe);
    edges.push_back(edge(pe, pe));
    order.push_back(sz);
    ++sz;
    _dir = pe;
    viz_vertex(pb, sz, pb);
    for (size_t i = 0; i < graph_edge[sz].size(); i++)
    {
        graph_edge[graph_edge[sz][i]].push_back(sz);
    }
    points.push_back(pb);

//Here is Dijkstra
    vector<mpf_class> d(points.size(), 0);
    vector<int> from(points.size(), -1);
    from[points.size() - 1] = points.size() - 1;
    set<pair<mpf_class, int> > q;
    q.insert(pair<mpf_class, int>(0, points.size() - 1));
    while (!q.empty())
    {
        pair<mpf_class, int> top = *q.begin();
        q.erase(q.begin());
        int num = top.second;
        mpf_class dt = top.first;
        for (size_t i = 0; i < graph_edge[num].size(); i++)
        {
            int v = graph_edge[num][i];
            mpf_class tmp_dist = dist(points[num], points[v]);
            if (from[v] == -1 || d[v] > d[num] + tmp_dist)
            {
                if (from[v] != -1)
                    q.erase(q.find(pair<mpf_class, int>(d[v], v)));
                d[v] = d[num] + tmp_dist;
                q.insert(pair<mpf_class, int>(d[v], v));
                from[v] = num;
            }
        }
    }
    vector<int> path;
    int tmp = points.size() - 2;
    do
    {
        if (path.size() > 1 && vp(points[path[path.size() - 1]], points[path[path.size() - 2]], points[tmp]) == 0)
        {
            path.pop_back();
        }
        path.push_back(tmp);
        tmp = from[tmp];
    } while (tmp != from[tmp]);
    vector<point_2> path_points;
    path_points.push_back(point_2(pb.x, pb.y));
    for (int i = path.size() - 1; i > 0; i--)
    {
        path_points.push_back(point_2(points[path[i]].x, points[path[i]].y));
    }
    path_points.push_back(point_2(pe.x, pe.y));
    return path_points;
}
}
