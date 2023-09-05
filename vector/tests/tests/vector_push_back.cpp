#include "pushback_common.h"
#include "executable.h"
#include <vector>

TEST(push_back) {
    Typegen t;

    std::vector<int> gt(last_idx, 0);

    {
        Memhook m;

        Vector<int> vec;
        int el;
        for(size_t p = 0, idx = 0; p < last_idx; p++) {
            const Memstate & s = state[idx];
            
            ASSERT_EQ(p,           vec.size());          
            ASSERT_EQ(s.capacity,  vec.capacity());      
            ASSERT_EQ(s.n_allocs,  m.n_allocs());        
            ASSERT_EQ(s.n_frees,   m.n_frees());         
                                                         
            if(s.capacity != 0) {           
                ASSERT_EQ(                                   
                    s.capacity * sizeof(gt[0]),          
                    m.last_alloc().size                
                );                                           
            }                   

            for(size_t i = 0; i < p; i++) {
                ASSERT_EQ(gt[i], vec[i]);
            }

            el = t.get<int>();
            gt[p] = el;
            vec.push_back(el);

            if(state[idx + 1].idx <= p + 1)
                idx++;
        }
    }
}
