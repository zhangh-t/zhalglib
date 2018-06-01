// zhalglib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sort.h"
#include <iostream>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <set>
#include "hugeInteger.h"
#include "multithread.h"

/**
* Definition for a binary tree node.
* 
*/

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    int numTrees(int n) {
        int res = 0;
        if (n > 0) {
            res = treeCount(1, n);
        }
        return res;
    }
private:
    int treeCount(int l, int r) {
        if (l == r) return 1;
        int res = 0;
        for (int i = l; i <= r; ++i) {
            int lc = treeCount(l, i - 1);
            int rc = treeCount(i + 1, r);
            res += (lc == 0 ? rc : (rc == 0 ? lc : lc * rc));
        }
        return res;
    }
};

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
    Solution s;
    s.numTrees(19);
   /* std::set<int> s1;
    s1.insert(1);
    s1.insert(2);
    std::set<int> s2;
    s2.insert(3);
    s2.insert(4);
    std::vector<int> r;
    std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), r.begin());
    asyncoperation as;
    as.execute();*/
}

