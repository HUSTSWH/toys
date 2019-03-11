#include <cstdint>
#include <cstdio>

#include <vector>

#include "tree.h"

using namespace std;

int main ()
{
    int n, ndim;
    vector<Point> points;
    KDTree tree;
    while (scanf("%d %d", &n, &ndim) != EOF) {
        points.clear();
        for (int i=0; i<n; i++) {
            Point point;
            for (int j=0; j<ndim; j++) {
                scanf("%d", &point.val[j]);
            }
            for (int j=ndim; j<num_dim; j++) {
                point.val[j] = 0;
            }
            points.push_back(point);
        }
        tree.build(points);
        int q;
        scanf("%d", &q);
        for (int i=0; i<q; i++) {
            Point point;
            for (int j=0; j<ndim; j++) {
                scanf("%d", &point.val[j]);
            }
            for (int j=ndim; j<num_dim; j++) {
                point.val[j] = 0;
            }
            int k;
            scanf("%d", &k);
            auto ans = tree.query(point, k);
            printf("the closest %d points are:\n", k);
            for (auto p: ans) {
                for (int j=0; j<ndim; j++) {
                    if (j!=0) putchar(' ');
                    printf("%d", p.val[j]);
                }
                putchar('\n');
            }
        }
    }
    return 0;
}


