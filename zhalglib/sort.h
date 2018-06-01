#include <vector>
namespace zh
{
    //交换相邻元素，每一次都弄出一个最大的放到最后
    template <class T, class _Pr>
    void bubleSort(std::vector<T>& container, _Pr cmp, bool bAsc = true) {
        int nSize = (int)(container.size());
        if (nSize > 1)
        {
            for (int i = 0 ; i < nSize - 1; ++i)
            {
                bool bDidSwap = false;
                for (int j = 0 ; j < nSize - 1 - i; ++j)
                {
                    bool bLarger = cmp(container[j + 1], container[j]);
                    if ((bAsc && bLarger)
                        || (!bAsc && !bLarger))
                    {
                        bDidSwap = true;
                        std::swap(container[j + 1], container[j]);
                    }
                }
                if (!bDidSwap)
                {
                    return; 
                }
            }
        }
    }

    template <class T, class _Pr>
    void selectionSort(std::vector<T>& container, _Pr cmp, bool bAsc = true) {
        int nSize = container.size();
        if (nSize > 1)
        {
            for (int i = 0 ; i < nSize; ++i)
            {
                int nTargetIndex = i;
                for (int j = i + 1; j < nSize; ++j)
                {
                    if (bAsc && cmp(container[j], container[nTargetIndex]))
                    {
                        nTargetIndex = j;
                    }
                    else if (!bAsc && cmp(container[nTargetIndex], container[j]))
                    {
                        nTargetIndex = j;
                    }
                }
                std::swap(container[nTargetIndex], container[i]);
            }
        }
    }

    template <class T, class _Pr>
    void insertSort(std::vector<T>& container, _Pr cmp, bool bAsc = true) {
         int nSize = (int)container.size();
         if (nSize > 1)
         {
             for (int i = 1 ; i < nSize; ++i)
             {
                 T oCur = container[i];
                 int j = i - 1;
                 while (j >= 0
                     && (bAsc ? cmp(oCur, container[j]) : cmp(container[j], oCur)))
                 {
                     container[j + 1] = container[j];
                     --j;
                 }
                 container[j + 1] = oCur;
             }
         }
    }

    template <class T, class _Pr>
    void shellSort(std::vector<T>& container, _Pr cmp, bool bAsc = true) {
        int nSize = container.size();
        int nGap = nSize;
        while (nGap >= 1) {
            nGap /= 2;
            for (int i = nGap ; i < nSize; ++i)
            {
                T oCur = container[i];
                int j = i - nGap;
                while (j >= 0 && !(bAsc ^ cmp(oCur, container[j]))) {
                    container[j + nGap] = container[j];
                    j -= nGap;
                }
                container[j + nGap] = oCur;
            }
        }
    }

    template <class T, class _Pr>
    std::vector<T> merge(std::vector<T>&& l, std::vector<T>&& r, _Pr cmp, bool bAsc) {
        std::vector<T> res;
        auto itl = l.begin();
        auto itr = r.begin();
        for (;
            itl < l.end() && itr < r.end();)
        {
            if (!(bAsc ^ cmp(*itl, *itr)))
            {
                res.push_back(*itl);
                ++itl;
            }
            else
            {
                res.push_back(*itr);
                ++itr;
            }
        }
        res.insert(res.end(), itl != l.end() ? itl : itr, itl != l.end() ? l.end() : r.end());
        return res;
    }

    template <class T, class _Pr>
    std::vector<T> mergeSort(std::vector<T>& container, _Pr cmp, bool bAsc = true) {
        int nSize = container.size();
        if (nSize >= 2)
        {
            int m = nSize >> 1;
            std::vector<int> l(container.begin(), container.begin() + m);
            std::vector<int> r(container.begin() + m, container.end());
            return merge(mergeSort(l, cmp, bAsc), mergeSort(r, cmp, bAsc), cmp, bAsc);
        }
        else {
            return container;
        }
    }

    template <class T, class _Pr>
    void innerSort(std::vector<T>& container, _Pr cmp, bool bAsc, int l, int r) {
        if (l >= r)
        {
            return;
        }
        T base = container[l];
        int i = l;
        int j = r;
        while (i < j) {
            while (i < j && !(bAsc ^ cmp(base, container[j]))) --j;
            while (i < j && !(bAsc ^ cmp(container[i], base))) ++i;
            if (i < j)
            {
                std::swap(container[i], container[j]);
            }
        }
        container[i] = base;
        innerSort(container, cmp, bAsc, l, i - 1);
        innerSort(container, cmp, bAsc, i + 1, r);
    }

    template <class T, class _Pr>
    void quickSort(std::vector<T>& container, _Pr cmp, bool bAsc = true) {
        int nSize = container.size();
        if (nSize >= 2)
        {
            innerSort(container, cmp, bAsc, 0, nSize - 1);
        }
    }

    template <class T, class _Pr>
    void buildHeap(std::vector<T>& container, _Pr cmp, int l, int r, bool bAsc = true) {
        int left;
        while (left = 2 * l + 1 < r) {
            int right = 2 * l + 2;
            int choosen = left;
            if (right < r && (bAsc ^ cmp(container[left], container[right]))) {
               choosen = right;
            }
            if (!(bAsc ^ cmp(container[l], container[choosen]))) {
                std::swap(container[l], container[choosen]);
                l = choosen;
            }
            else 
                break;
        }
    }

    template <class T, class _Pr>
    void heapSort(std::vector<T>& container, _Pr cmp, bool bAsc = true) {
        //1. rebuild heap
        std::unique()
        int nSize = container.size();
        if (nSize > 2)
        {
            int nLastRoot = nSize / 2 - 1;
            for (int i = nLastRoot; i >= 0 ; --i)
            {
                buildHeap(container, cmp, i, nSize, bAsc);
            }

            while (nSize >= 0) {
                std::swap(container[0], container[nSize--]);
                buildHeap(container, cmp, 0, nSize, bAsc);
            }
        }
    }

}