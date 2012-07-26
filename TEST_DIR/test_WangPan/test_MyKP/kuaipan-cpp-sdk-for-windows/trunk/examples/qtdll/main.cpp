/*
 * kpSDK是一个基于金山快盘和Qt的自由sdk库，您可以自由分发、修改其中的源代码或者重新发布它，
 * 新的任何修改后的重新发布版必须同样在遵守LGPL3或更后续的版本协议下发布.
 * 关于LGPL协议的细则请参考COPYING、COPYING.LESSER文件，
 * 您可以在kpSDK\examples\qtdll的相关目录中获得LGPL协议的副本，
 * 如果没有找到，请连接到 http://www.gnu.org/licenses/ 查看。
*/

//kuaipan cpp sdk for windows
//Email: michael.20@qq.com
//License: GNU Lesser General Public License (LGPL)
//版权所有 (C) 2012 Michael Lee


//可以在本cpp内添加类似
//“extern "C" __declspec(dllexport) void yourFun() {}”系列函数实现动态库的内部函数


#include <qmfcapp.h>
#include <qwinwidget.h>
#include <windows.h>
#include "kpsdk.h"
BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpvReserved*/ )
{
    static bool ownApplication = FALSE;

    if ( dwReason == DLL_PROCESS_ATTACH )
	ownApplication = QMfcApp::pluginInstance( hInstance );
    if ( dwReason == DLL_PROCESS_DETACH && ownApplication )
	delete qApp;

    return TRUE;
}

//获取临时token、secret和认证url
//输入：consumerKey    consumerSecret
//输出：tmpOauthToken  tmpOauthTokenSecret     authoriseUrl
//返回: http 状态
//通过	consumer_key 	consumer_secret使用requestToken得到oauth_token oauth_token_secret
extern "C" __declspec(dllexport) int kpReqTmpToken(string &tmpOauthToken,string &tmpOauthTokenSecret,\
                                                     string &authoriseUrl,\
                                                     const string &consumerKey,const string &consumerSecret)
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->reqReqTmpToken();

    tmpOauthToken=q2s(kp->mTmpToken);
    tmpOauthTokenSecret=q2s(kp->mTmpTokenSecret);
    authoriseUrl=q2s(QString(KP_AUTHORISE_SRC_URL).append(s2q(tmpOauthToken)));

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//获取accessToken
//输入:consumerKey     consumerSecret         tmpOauthToken  tmpOauthTokenSecret
//输出：oauthToken     oauthTokenSecret        usrID       chargedDir
//返回: http 状态
extern "C" __declspec(dllexport) int kpAccessToken(string &oauth_token, string &oauth_token_secret,\
                                                      string &user_id,      string &charged_dir,\
                                                      const string &consumerKey,const string &consumerSecret,\
                                                      const string &tmpOauthToken,const string &tmpOauthTokenSecret
                                                      )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mTmpToken=s2q(tmpOauthToken);
    kp->mTmpTokenSecret=s2q(tmpOauthTokenSecret);
    kp->reqAcessToken();
    oauth_token=q2s(kp->mAuTokenInfo.auToken);
    oauth_token_secret=q2s(kp->mAuTokenInfo.auTokenSecret);
    user_id=q2s(kp->mAuTokenInfo.usrID);
    charged_dir=q2s(kp->mAuTokenInfo.chargedDir);

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//获取用户信息
//输入: consumerKey consumerSecret            oauthToken    oauthTokenSecret
//输出：max_file_size   user_name       quota_used       quota_total
//返回: http 状态
extern "C" __declspec(dllexport) int kpUsrInfo(string &max_file_size, string &user_name,\
                                                  string &quota_used,    string &quota_total,\
                                                  const string &consumerKey,const string &consumerSecret,\
                                                  const string &oauthToken,const string &oauthTokenSecret
                                                  )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->reqUsrInfo();
    max_file_size=q2s(kp->mUsrInfo.maxFileSize);
    user_name=q2s(kp->mUsrInfo.usrName);
    quota_used=q2s(kp->mUsrInfo.quotaUsed);
    quota_total=q2s(kp->mUsrInfo.quotaTotal);

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//获取文件(夹)信息
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   dataPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//dataPath  是指要获取的文件(夹)路径+名
//输出：jsonMetaData
//返回: http 状态
extern "C" __declspec(dllexport) int kpMetaData(string &jsonMetaData,\
                                                  const string &consumerKey,const string &consumerSecret,\
                                                  const string &oauthToken,const string &oauthTokenSecret,
                                                  const bool   &isAppPath, const string &dataPath
                                                  )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsAppPath=isAppPath;
    kp->mDataPath=s2q(dataPath);
    kp->reqMetaData();
    jsonMetaData=q2s(QString(kp->mJsonBuf));

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//新建文件夹
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//floderPath  是指要新建的文件夹路径+名
//输出：jsonCreateFolder
//返回: http 状态
extern "C" __declspec(dllexport) int kpCreateFolder(string &jsonCreateFolder,\
                                                    const string &consumerKey,const string &consumerSecret,\
                                                    const string &oauthToken,const string &oauthTokenSecret,
                                                    const bool   &isAppPath, const string &floderPath
                                                    )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsAppPath=isAppPath;
    kp->mCreateFolderPath=s2q(floderPath);
    kp->reqCreateFloder();
    jsonCreateFolder=q2s(QString(kp->mJsonBuf));

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//删除文件夹
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath  to_recycle
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//floderPath  是指要获取的文件(夹)路径+名
//to_recycle  是否删除到回收站？
//输出：jsonDelFile
extern "C" __declspec(dllexport) int kpDelFile(string &jsonDelFile,\
                                               const string &consumerKey,const string &consumerSecret,\
                                               const string &oauthToken,const string &oauthTokenSecret,
                                               const bool   &isAppPath, const string &floderPath,
                                               const bool   &to_recycle
                                               )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsAppPath=isAppPath;
    kp->mIsToRecyle=to_recycle;
    kp->mDelFilePath=s2q(floderPath);
    kp->reqDelFile();
    jsonDelFile=q2s(QString(kp->mJsonBuf));

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//移动文件(夹)
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//输出：jsonMvFile
//返回: http 状态
extern "C" __declspec(dllexport) int kpMvFile(string &jsonMvFile,\
                                              const string &consumerKey,const string &consumerSecret,\
                                              const string &oauthToken,const string &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const string &fromPath, const string &toPath
                                              )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=s2q(fromPath);
    kp->mToPath=s2q(toPath);
    kp->reqMoveFile();
    jsonMvFile=q2s(QString(kp->mJsonBuf));

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//复制文件(夹)
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//输出：jsonMvFile
extern "C" __declspec(dllexport) int kpCpFile(string &jsonCpFile,\
                                              const string &consumerKey,const string &consumerSecret,\
                                              const string &oauthToken,const string &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const string &fromPath, const string &toPath
                                              )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=s2q(fromPath);
    kp->mToPath=s2q(toPath);
    kp->reqCopyFile();
    jsonCpFile=q2s(QString(kp->mJsonBuf));

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//获取上传节点
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret
//输出：uploadNode
//返回: http 状态
extern "C" __declspec(dllexport) int kpUploadLocate(string &uploadNode,\
                                                       const string &consumerKey,const string &consumerSecret,\
                                                       const string &oauthToken,const string &oauthTokenSecret
                                                       )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->reqUploadLocate();
    uploadNode=q2s(kp->mUploadNode);
    int ret=kp->mRet;
    delete kp;
    return ret;
}

//上传文件
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    、
//     isOverWrite isAppPath   uploadNode  fromPath toPath
//输出：jsonUploadFile
//返回: http 状态
extern "C" __declspec(dllexport) int kpUploadFile(string &jsonUploadFile,
                                                  const string &consumerKey,const string &consumerSecret,\
                                                  const string &oauthToken,const string &oauthTokenSecret,\
                                                  const bool &isOverWrite, const bool &isAppPath,\
                                                  const string &uploadNode, const string &fromPath,
                                                  const string &toPath
                                                  )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsOverWrite=isOverWrite;
    kp->mIsAppPath=isAppPath;
    kp->mUploadNode=s2q(uploadNode);
    kp->mFromPath=s2q(fromPath);
    kp->mToPath=s2q(toPath);
    kp->reqUploadFile();
    jsonUploadFile=q2s(kp->mJsonBuf);
    int ret=kp->mRet;
    delete kp;
    return ret;
}

//下载文件
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    、
//     isAppPath   fromPath toPath
//返回: http 状态
extern "C" __declspec(dllexport) int kpDownloadFile(const string &consumerKey,const string &consumerSecret,\
                                                  const string &oauthToken,const string &oauthTokenSecret,\
                                                  const bool &isAppPath,const string &fromPath,
                                                  const string &toPath
                                                  )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=s2q(fromPath);
    kp->mToPath=s2q(toPath);
    kp->reqDownLoadFile();

    int ret=kp->mRet;
    delete kp;
    return ret;
}


//分享文件
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    、
//     isAppPath   fromPath
//输出：jsonShareFile
//返回: http 状态
extern "C" __declspec(dllexport) int kpShareFile(string &jsonShareFile,
                                                 const string &consumerKey,const string &consumerSecret,\
                                                 const string &oauthToken,const string &oauthTokenSecret,\
                                                 const bool &isAppPath,const string &fromPath
                                                 )
{
    kpSDK *kp=new kpSDK();
    kp->mConsumerKey=s2q(consumerKey);
    kp->mConsumerSecret=s2q(consumerSecret);
    kp->mAuTokenInfo.auToken=s2q(oauthToken);
    kp->mAuTokenInfo.auTokenSecret=s2q(oauthTokenSecret);
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=s2q(fromPath);
    kp->reqShareFile();
    jsonShareFile=q2s(kp->mJsonBuf);

    int ret=kp->mRet;
    delete kp;
    return ret;
}
