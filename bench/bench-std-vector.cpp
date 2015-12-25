#include <vector>
#include <stdio.h>

int main()
{
    int val;
    for (int m = 0; m < 10000; m++) {
        std::vector<int> vec;

        for (int i = 0; i < 100000; i++) {
            vec.push_back(i);
        }

        for (int i = 0; i < 100000; i++) {
            val = vec[i];
        }

        for (int i = 0; i < 100000; i++) {
            vec.pop_back();
        }
        
        if (m % 1000 == 0) printf("%d\n", m / 100);
    }

    printf("%d\n", val);
}