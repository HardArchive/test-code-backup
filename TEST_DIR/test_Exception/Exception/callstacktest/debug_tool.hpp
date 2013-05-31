/**
 * @brief   调试工具
 * @author  dongbo
 * @date    2010-3-17
 * @remarks
**/

#pragma once

#include <list>
#include <string>

#include <boost/tr1/tr1/memory>

namespace dbsoft
{
	namespace detail
	{
		class callstack_Imp;
	}
	
	class callstack
	{
	public:
		typedef std::string						       func_name;
		typedef std::list< func_name >                 func_name_list;
		typedef func_name_list::const_iterator         const_iterator;
		typedef std::tr1::shared_ptr<callstack>        callstack_ptr;

		friend class detail::callstack_Imp;

	public:
		callstack();
	public:
		const_iterator  begin() const;
		const_iterator  end() const;
	
	public:
		/** 
		 * @brief 如果你不传递参数，那么你将得到当前时刻的调用堆栈
		 *        如果你使用它来处理SEH异常，那么请将LPEXCEPTION_POINTERS->ContextRecord传进去
		**/
		static callstack_ptr generate( const void* pContext = NULL );

	private:
		std::tr1::shared_ptr<detail::callstack_Imp>         m_spImp;
	};	
}