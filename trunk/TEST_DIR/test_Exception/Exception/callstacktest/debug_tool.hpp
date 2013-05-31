/**
 * @brief   ���Թ���
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
		 * @brief ����㲻���ݲ�������ô�㽫�õ���ǰʱ�̵ĵ��ö�ջ
		 *        �����ʹ����������SEH�쳣����ô�뽫LPEXCEPTION_POINTERS->ContextRecord����ȥ
		**/
		static callstack_ptr generate( const void* pContext = NULL );

	private:
		std::tr1::shared_ptr<detail::callstack_Imp>         m_spImp;
	};	
}