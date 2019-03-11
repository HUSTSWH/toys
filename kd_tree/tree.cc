// K Dimension_Tree
// Author: SongWenhao
// Date: 03/10/2019

#include "tree.h"

#include <cstdint>
#include <cstdio>

#include <algorithm>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

KDTree::KDTree():
    pImpl_(nullptr){}

KDTree::KDTree(const vector<Point>& points):
    pImpl_(nullptr) {
    build_nocheck(points);
}

KDTree::~KDTree() {
    clear();
}

void KDTree::build(const vector<Point>& points) {
    clear();
    build_nocheck(points);
}

void KDTree::clear() {
    stack<Node*> s;
    s.push(pImpl_);
    while (!s.empty()) {
        Node* pCur = s.top(); s.pop();
        if (pCur == nullptr)
            continue;
        s.push(pCur->son[0]);
        s.push(pCur->son[1]);
        delete pCur;
    }
    pImpl_ = nullptr;
}

void KDTree::clear_nocheck()
{
    clear();
}

// Start at "pImpl_ == nullptr"
// automatically allocate nodes by "new"
void KDTree::build_nocheck(const Points& points) {
    auto temp = points;
    // If an empty data set is given, this can succeessfully leave the tree empty.
    recursive_build(pImpl_, temp, 0, points.size());
}

// This function recursively treat the data set points and build KD Tree nodes.
// It splits data into segments(i.e. points[x] where x~[l,r)if necessarily.
void KDTree::recursive_build(Node*& ptr, vector<Point>& points, size_t begin, size_t end) {
    if (begin == end) {
        ptr = nullptr;
        return;
    }

    ptr = new Node;
    // choose a suitable axis
    Point coord_min, coord_max; // stores the min & max value for each coordinate, not necessarily a specific point.
    coord_min = coord_max = points[begin];
    for (size_t i=begin+1; i<end; i++) {
        for (size_t j=0; j<num_dim; j++) {
            if (coord_min.val[j] > points[i].val[j]) {
                coord_min.val[j] = points[i].val[j];
            }
            if (coord_max.val[j] < points[i].val[j]) {
                coord_max.val[j] = points[i].val[j];
            }
        }
    }

    // find the coordinate with the greatest range
    size_t coord = 0;
    for (size_t j=1; j<num_dim; j++) {
        if (coord_max.val[j] - coord_min.val[j] > 
            coord_max.val[coord] - coord_min.val[coord]) {
            coord = j;
        }
    }

    // split the current segment into two pairs
    int middle = begin + (end-begin)/2;
    nth_element(
        points.begin()+begin, points.begin()+middle, points.begin()+end,
        [&coord](const Point& lhs, const Point& rhs){
            return lhs.val[coord] < rhs.val[coord];
        });

    ptr->pivot = points[middle];
    ptr->coord = coord;
    static int cnt = 0;
    for (int i=0; i<cnt; i++) putchar(' ');
    printf("%c %d %d %d\n", "XYZ"[coord], ptr->pivot.val[0], ptr->pivot.val[1], ptr->pivot.val[2]);
    cnt++;
    // TODO: Tail recursion, can be optimized.
    recursive_build(ptr->son[0], points, begin, middle);
    recursive_build(ptr->son[1], points, middle+1, end);
    cnt--;
}

auto cmp = [](const PointWithDist& lhs, const PointWithDist& rhs) -> bool {
    return lhs.dist < rhs.dist;
};

KDTree::Points KDTree::query(Point point, size_t k)
{
    vector<PointWithDist> heap;
    heap.reserve(k+1);
    recursive_query(heap, pImpl_, point, k);
    sort_heap(heap.begin(), heap.end(), cmp);
    Points ans(heap.begin(), heap.end());
    return ans;
}

void KDTree::recursive_query(vector<PointWithDist>& heap, Node* ptr, Point point, size_t k)
{
    if (ptr == nullptr)
        return;
    size_t coord = ptr->coord;
    bool gt = (point.val[coord] >= ptr->pivot.val[coord]);
    recursive_query(heap, ptr->son[gt], point, k);

    // calculate the distance from the current boundary
    int64_t dist = static_cast<int64_t>(point.val[coord] - ptr->pivot.val[coord]);
    dist = dist * dist;
    
    if (heap.size() == k && heap.front().dist <= dist)
        return;

    heap.push_back({ptr->pivot, point.DistFrom(ptr->pivot)});
    push_heap(heap.begin(), heap.end(), cmp);
    if (heap.size() == k+1) {
        pop_heap(heap.begin(), heap.end(), cmp);
        heap.pop_back();
    }

    recursive_query(heap, ptr->son[!gt], point, k);
}
