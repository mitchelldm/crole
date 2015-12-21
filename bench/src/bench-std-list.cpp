#include <list>
#include <cstdio>

int main()
{
    int val = 0;

    for (int m = 0; m < 10000; m++) {
        std::list<int> list;
        for (int j = 0; j < 1000; j++) {
            list.push_back(j);
        }
        
        for (int j = 0; j < 1000; j++) {
            list.push_front(j);
        }
        
        while (!list.empty()) {
            list.pop_back();
        }

        while (!list.empty()) {
            list.pop_front();
        }

        for (int i = 0; i < 1000; i++) {
            list.push_back(i);
        }

        for (int i = 0; i < 1000; i++) {
            std::list<int>::iterator it = list.begin();
            std::advance(it, 20);
            val = *it;
        }

        for (int i = 0; i < 1000; i++) {
            std::list<int>::iterator it = list.begin();
            std::advance(it, 20);
            list.insert(it, i);
        }

        for (int i = 0; i < 1000; i++) {
            std::list<int>::iterator it = list.begin();
            std::advance(it, 20);
            list.erase(it);
        }

        for (int i = 0; i < 1000; i++) {
            for (std::list<int>::iterator it = list.begin(), end = list.end(); it != end; ++it) {
                val = *it;
            }

            for (std::list<int>::reverse_iterator it = list.rbegin(), end = list.rend(); it != end; ++it) {
                val = *it;
            }
        }

        if (m % 1000 == 0) printf("%d\n", m / 100);
    }

    printf("%d\n", val);
}