// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//判断pStr2是否是pStr1的后
bool IsSuffixNoCase(const char *pStr1,int len1,const char *pStr2,int len2)
{
	//如果域名没有后缀长
	if(len1 <=len2)
	{
		return false;
	}

	pStr1 +=(len1-len2);
	for(int i=0;i<len2;i++)
	{
		if(pStr2[i]!=(pStr1[i]|0x20))
		{
			return false;
		}
	}
	return true;
}

TCHAR tszInternationalDomain[12][6] = 
{
	_T("arts"),  //艺术类机构
	_T("com"),   //商业机构
	_T("edu"),   //教育及研究机构
	_T("firm"),  //公司企业
	_T("gov"),   //政府机构
	_T("info"),  //信息服务
	_T("net"),   //网络服务机构
	_T("nom"),   //个人
	_T("org"),   //专业团体
	_T("rec"),   //娱乐类机构
	_T("store"), //销售类公司企业
	_T("web")    //从事WWW活动的机构
};
//国家(地区)域名
TCHAR tszNationDomain[248][3] = 
{
	_T("hk"),
	_T("tw"),
	_T("mo"),
	_T("cn"),
	_T("cf"),
	_T("cl"),
	_T("gi"),
	_T("td"),
	_T("je"),
	_T("zm"),
	_T("vn"),
	_T("Jo"),
	_T("gb"),
	_T("uk"),
	_T("id"),
	_T("in"),
	_T("it"),
	_T("il"),
	_T("ir"),
	_T("iq"),
	_T("ye"),
	_T("am"),
	_T("jm"),
	_T("sy"),
	_T("hu"),
	_T("nz"),
	_T("nc"),
	_T("sg"),
	_T("gr"),
	_T("eh"),
	_T("es"),
	_T("uz"),
	_T("uy"),
	_T("ua"),
	_T("ug"),
	_T("bn"),
	_T("ve"),
	_T("vg"),
	_T("vi"),
	_T("gt"),
	_T("vu"),
	_T("wf"),
	_T("tk"),
	_T("tm"),
	_T("tr"),
	_T("tv"),
	_T("tn"),
	_T("tt"),
	_T("tc"),
	_T("to"),
	_T("tz"),
	_T("th"),
	_T("tj"),
	_T("so"),
	_T("sb"),
	_T("sr"),
	_T("sd"),
	_T("sz"),
	_T("sj"),
	_T("si"),
	_T("sk"),
	_T("lk"),
	_T("vc"),
	_T("pm"),
	_T("sm"),
	_T("lc"),
	_T("kn"),
	_T("sh"),
	_T("st"),
	_T("cx"),
	_T("sa"),
	_T("sc"),
	_T("cy"),
	_T("sn"),
	_T("sl"),
	_T("yu"),
	_T("cs"),
	_T("ws"),
	_T("sv"),
	_T("ch"),
	_T("se"),
	_T("jp"),
	_T("ge"),
	_T("pt"),
	_T("pn"),
	_T("pw"),
	_T("eu"),
	_T("nf"),
	_T("no"),
	_T("nu"),
	_T("ng"),
	_T("ne"),
	_T("ni"),
	_T("np"),
	_T("nr"),
	_T("gs"),
	_T("aq"),
	_T("za"),
	_T("na"),
	_T("mx"),
	_T("mz"),
	_T("mc"),
	_T("ma"),
	_T("md"),
	_T("mm"),
	_T("fm"),
	_T("pe"),
	_T("bd"),
	_T("ms"),
	_T("mn"),
	_T("um"),
	_T("as"),
	_T("us"),
	_T("mr"),
	_T("mu"),
	_T("yt"),
	_T("mq"),
	_T("mh"),
	_T("mk"),
	_T("ml"),
	_T("my"),
	_T("mw"),
	_T("mt"),
	_T("mv"),
	_T("im"),
	_T("mg"),
	_T("ro"),
	_T("rw"),
	_T("lu"),
	_T("re"),
	_T("li"),
	_T("ly"),
	_T("lr"),
	_T("lt"),
	_T("lb"),
	_T("la"),
	_T("ls"),
	_T("lv"),
	_T("ck"),
	_T("ke"),
	_T("hr"),
	_T("kw"),
	_T("ci"),
	_T("km"),
	_T("cc"),
	_T("ky"),
	_T("qa"),
	_T("cm"),
	_T("zw"),
	_T("cz"),
	_T("kh"),
	_T("ga"),
	_T("gh"),
	_T("ca"),
	_T("gw"),
	_T("gn"),
	_T("kg"),
	_T("dj"),
	_T("ki"),
	_T("hn"),
	_T("hm"),
	_T("an"),
	_T("nl"),
	_T("kr"),
	_T("ht"),
	_T("kz"),
	_T("gy"),
	_T("gu"),
	_T("gp"),
	_T("cu"),
	_T("gl"),
	_T("gd"),
	_T("gg"),
	_T("cr"),
	_T("co"),
	_T("cd"),
	_T("cg"),
	_T("gm"),
	_T("fk"),
	_T("cv"),
	_T("fi"),
	_T("fj"),
	_T("ph"),
	_T("va"),
	_T("tf"),
	_T("gf"),
	_T("pf"),
	_T("fo"),
	_T("fr"),
	_T("er"),
	_T("ec"),
	_T("ru"),
	_T("do"),
	_T("dm"),
	_T("tg"),
	_T("tl"),
	_T("tp"),
	_T("de"),
	_T("dk"),
	_T("gq"),
	_T("kp"),
	_T("bv"),
	_T("bi"),
	_T("bf"),
	_T("io"),
	_T("bt"),
	_T("bw"),
	_T("bz"),
	_T("bo"),
	_T("ba"),
	_T("pl"),
	_T("pr"),
	_T("is"),
	_T("be"),
	_T("bj"),
	_T("mp"),
	_T("bg"),
	_T("bm"),
	_T("by"),
	_T("br"),
	_T("pa"),
	_T("bh"),
	_T("ps"),
	_T("py"),
	_T("pk"),
	_T("bs"),
	_T("pg"),
	_T("bb"),
	_T("au"),
	_T("ax"),
	_T("at"),
	_T("ag"),
	_T("ai"),
	_T("ao"),
	_T("ad"),
	_T("ee"),
	_T("ie"),
	_T("et"),
	_T("eg"),
	_T("ac"),
	_T("az"),
	_T("om"),
	_T("aw"),
	_T("ae"),
	_T("ar"),
	_T("af"),
	_T("dz"),
	_T("al")
};

int GetDomainLevel(const char *pDomain,int domainLen)
{
	//域名后缀列表 com.cn 一定要放在cn前面 其它类似的域名也一样
	static struct SUFFIX
	{
		char *suffix;
		int len;
	}suffixList[]={
		".com",4,
		".com.cn",7,
		".cn",3,
		NULL,0
	};

	if(domainLen <4)
	{
		return 1;
	}
	//if((pDomain[0]|0x20)=='w' &&(pDomain[1]|0x20)=='w' &&(pDomain[2]|0x20)=='w' && pDomain[3]=='.')
	//{
	//	return 1;
	//}

	for(SUFFIX *pCurSuffix=&suffixList[0];pCurSuffix->suffix!=NULL;pCurSuffix++)
	{
		//如果是后缀
		if(IsSuffixNoCase(pDomain,domainLen,pCurSuffix->suffix,pCurSuffix->len))
		{
			//获取是一级还是二级域名
			bool bFindDot=false;
			int cutLen=domainLen-pCurSuffix->len;
			for(int j=0;j<cutLen;j++)
			{
				if(pDomain[j]=='.')
				{
					bFindDot=true;
					break;
				}
			}
			if(bFindDot)
			{
				return 2;
			}
			else
			{
				return 1;
			}
		}
	}
	//执行到此说明和后缀列表不匹配 这样只能以点的个数来判断域名
	int dotNums=0;
	for(int i=0;i<domainLen;i++)
	{
		if(pDomain[i]=='.')
		{
			dotNums++;
		}
	}
	if(dotNums>=2)
	{
		return 2;
	}
	else
	{
		return 1;
	}
	return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{
	char *p = "www.baidu.com.cn";
	int level = GetDomainLevel(p, strlen(p));
	getchar();
	return 0;
}

