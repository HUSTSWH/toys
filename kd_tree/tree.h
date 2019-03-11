// K Dimension_Tree
// Author: SongWenhao
// Date: 03/10/2019

#ifndef KDTREE_H_
#define KDTREE_H_

#include <cstdint>

#include <vector>

const size_t num_dim = 5;

struct Point
{
    int32_t val[num_dim]; // The value of each coordinate.
    int64_t DistFrom(const Point& rhs) {
        int64_t ans = 0;
        for (size_t i=0; i<num_dim; i++) {
            int64_t temp = static_cast<int64_t>(rhs.val[i] - val[i]);
            ans += temp * temp;
        }
        return ans;
    }
};

struct PointWithDist: Point
{
    int64_t dist;
    PointWithDist(const Point &p, int64_t d):
        Point(p),
        dist(d){}
};

class KDTree {
  public:
    using Points = std::vector<Point>;
    /*****constructor & destructors*****/
    KDTree(); // create an empty tree
    explicit KDTree(const Points& points);// create & build
    ~KDTree();

    /***** modify *****/
    // clear the data before(if exists), and build a new one
    void build(const Points& points);
    // clear old data and free
    void clear();

    /***** query *****/
    // query: return k nearest point from the given point
    Points query(Point point, size_t k);

  protected:
    struct Node {
        Point pivot;
        Node* son[2];
        size_t coord;
    } *pImpl_;
    static void recursive_build(Node*& ptr, Points& points, size_t begin, size_t end);
    static void recursive_query(std::vector<PointWithDist>& heap, Node* ptr, Point point, size_t k);
    void build_nocheck(const Points& points);
    void clear_nocheck();
};

#endif
