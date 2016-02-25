#ifndef _VEE_LOCKFREE_LIST_H_
#define _VEE_LOCKFREE_LIST_H_

#include <atomic>
#include <memory>

namespace vee {

namespace lockfree {
    
template <typename DataTy>
class list
{
public:
    using this_t = list<DataTy>;
    using ref_t  = this_t&;
    using rref_t = this_t&&;
    using data_t = DataTy;

    class node
    {
    public:
        using shared_ptr = ::std::shared_ptr<node>;
        ::std::atomic<node*> next;
        ::std::atomic<node*> prev;
        data_t value;
    };
    using node_ptr = typename node::shared_ptr;
    
    list();
    ~list();

    node_ptr insert_back(node_ptr target);
    node_ptr remove_back(node_ptr target);

    node_ptr head;
};


} // !namespace lockfree
    
} // !namespace vee

#endif // !_VEE_LOCKFREE_LIST_H_