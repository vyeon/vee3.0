#ifndef _VEE_LOCKFREE_STACK_H_
#define _VEE_LOCKFREE_STACK_H_

namespace vee {

namespace lockfree {

template <typename DataTy>
class stack final
{
public:
	using this_t = stack<DataTy>;
	using ref_t = this_t&;
	using rref_t = this_t&&;
	using data_t = DataTy;
};

} // !namespace lockfree
	
} // !namespace vee

#endif // !_VEE_LOCKFREE_STATCK_H_