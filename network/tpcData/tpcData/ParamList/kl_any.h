///
/// @file kl_any.h
/// @author Kevin Lynx
/// @brief To implement a simple 'any' library, check out boost::any for more information.
///
#ifndef ___KL_ANY_H_
#define ___KL_ANY_H_


namespace kl_common
{
    /// any clsaa 可以包括任何东西 它可以让你的工作更兼容
    /// any class can wrap any type in it and this can make your work more compatible.
    /// check out boost::any for more information.
    ///
    class any
    {
    public:
        any() : _t( 0 )
        {
        }
        
		//模板构造函数，参数可以是任意类型，真正的数据保存在_t中
        template <typename _Tp>
        any( const _Tp &t ) : _t( new var_holder<_Tp>( t ) )
        {
        }

		//拷贝构造函数
        any( const any &other ) : _t( other._t == 0 ? 0 : other._t->clone() )
        {
        }

		//析构函数，删除保存数据的_t对象
        ~any()
        {
            delete _t;
        }

		//重载等号
        any &operator = ( const any &other )
        {
            if( &other == this )
            {
                return *this;
            }
            delete _t;
            _t = other._t->clone();
            return *this;
        }

		//检查any实例当前是否有值，不管是什么值。因而，当any持有一个指针时，即使该指针值为空，则 empty 也返回 false 
        bool empty() const
        {
            return _t == 0 ;
        }

    private:
        class base_type
        {
        public:
            virtual ~base_type()
            {
            }
            virtual base_type *clone() const = 0;
        };
        
        template <typename _Tp>
        class var_holder : public base_type
        {
        public:
            typedef _Tp type;
            typedef var_holder<type> self;
        public:
            var_holder( const type &t ) : _t( t )
            {
            }

            base_type *clone() const
            {
                return new self( _t );
            }
        public:
            type _t;
        };

    private:
        base_type *_t;
    private:
        template <typename _Tp>
        friend _Tp *any_cast( any* );
    };

    ///
    /// To cast any to the specified type.
    ///any_cast 让你访问any中存放的值。参数为需要取回值的 any 对象。如果类型 ValueType 与所存值不符，any 抛出一个 bad_any_cast 异常    
    template <typename _Tp>
    _Tp *any_cast( any *t )
    {
		//static_cast把《( t->_t )->_t》转换为《any::var_holder<_Tp>*》类型，但没有运行时类型检查来保证转换的安全性
        return t == 0 ? 0 : &static_cast<any::var_holder<_Tp>*>( t->_t )->_t;
    }

    template <typename _Tp>
    const _Tp *any_cast( const any *t )
    {
        return any_cast<_Tp>( const_cast<any*>( t ) );
    }

    ///
    /// If the t is not valid, this function may occur some crash.
    ///
    template <typename _Tp>
    _Tp any_cast( any &t )
    {
        _Tp *ret = any_cast<_Tp>( &t );
        return *ret;
    }

    template <typename _Tp>
    _Tp any_cast( const any &t )
    {
        _Tp *ret = any_cast<_Tp>( const_cast<any*>( &t ) );
        return *ret;
    }

}

#endif // ___KL_ANY_H_

