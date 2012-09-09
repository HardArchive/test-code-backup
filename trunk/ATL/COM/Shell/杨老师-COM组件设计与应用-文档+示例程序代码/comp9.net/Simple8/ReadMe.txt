========================================================================
    活动模板库 : Simple8 项目概况
========================================================================

应用程序向导为您创建了这个 Simple8 项目，供您用作
编写动态链接库(DLL)的起点。

此文件包含
组成项目的每个文件的内容摘要。

Simple8.vcproj
    这是用应用程序向导生成的 VC++ 项目的主项目文件。
    它包含有关生成此文件的 Visual C++ 版本的信息，以及
    有关使用应用程序向导选择的
    平台、配置和项目功能的信息。

Simple8.idl
    此文件包含项目中定义的类型库、
    接口和 co-class 的 IDL 定义。
    此文件将由 MIDL 编译器处理以生成:
        C++ 接口定义和 GUID 声明(Simple8.h)
        GUID 定义(Simple8_i.c)
        类型库(Simple8.tlb)
        封送处理代码(Simple8_p.c and dlldata.c)

Simple8.h
    此文件包含在 Simple8.idl 中定义的项的 C++
    接口定义和 GUID 声明。它将在编译期间由 MIDL 重新生成。
Simple8.cpp
    此文件包含对象映射和 DLL 导出的实现。
Simple8.rc
    这是程序使用的所有 Microsoft Windows 资源
    的列表。

Simple8.def
    此模块定义文件为链接器提供有关 DLL 所需的
    导出的信息。它包含如下内容的导出:
        DllGetClassObject  
        DllCanUnloadNow    
        GetProxyDllInfo    
        DllRegisterServer	
        DllUnregisterServer

/////////////////////////////////////////////////////////////////////////////
其他标准文件:

StdAfx.h、StdAfx.cpp
    这些文件用于生成名为 Simple8.pch
    的预编译头(PCH)文件以及名为 StdAfx.obj 的预编译类型文件。

Resource.h
    这是定义资源 ID 的标准头文件。

/////////////////////////////////////////////////////////////////////////////
代理/存根(stub) DLL 项目和模块定义文件:

Simple8ps.vcproj
    此文件是生成代理/存根(stub) DLL (如有必要)的项目文件。
	主项目中的 IDL 文件必须至少包含一个接口，而且必须
	在生成代理/存根(stub) DLL 之前先编译 IDL 文件。	此过程生成
	dlldata.c、Simple8_i.c 和 Simple8_p.c，这些是
	生成代理/存根(stub) DLL 所需的。

Simple8ps.def
    此模块定义文件为链接器提供
    有关代理/存根(stub) 所需的导出的信息。
/////////////////////////////////////////////////////////////////////////////
