/*
 * kpSDK��һ�����ڽ�ɽ���̺�Qt������sdk�⣬���������ɷַ����޸����е�Դ����������·�������
 * �µ��κ��޸ĺ�����·��������ͬ��������LGPL3��������İ汾Э���·���.
 * ����LGPLЭ���ϸ����ο�COPYING��COPYING.LESSER�ļ���
 * ��������kpSDK\examples\qtdll�����Ŀ¼�л��LGPLЭ��ĸ�����
 * ���û���ҵ��������ӵ� http://www.gnu.org/licenses/ �鿴��
*/

//kuaipan cpp sdk for windows
//Email: michael.20@qq.com
//License: GNU Lesser General Public License (LGPL)
//��Ȩ���� (C) 2012 Michael Lee


//�����ڱ�cpp���������
//��extern "C" __declspec(dllexport) void yourFun() {}��ϵ�к���ʵ�ֶ�̬����ڲ�����


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

//��ȡ��ʱtoken��secret����֤url
//���룺consumerKey    consumerSecret
//�����tmpOauthToken  tmpOauthTokenSecret     authoriseUrl
//����: http ״̬
//ͨ��	consumer_key 	consumer_secretʹ��requestToken�õ�oauth_token oauth_token_secret
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

//��ȡaccessToken
//����:consumerKey     consumerSecret         tmpOauthToken  tmpOauthTokenSecret
//�����oauthToken     oauthTokenSecret        usrID       chargedDir
//����: http ״̬
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

//��ȡ�û���Ϣ
//����: consumerKey consumerSecret            oauthToken    oauthTokenSecret
//�����max_file_size   user_name       quota_used       quota_total
//����: http ״̬
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

//��ȡ�ļ�(��)��Ϣ
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   dataPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//dataPath  ��ָҪ��ȡ���ļ�(��)·��+��
//�����jsonMetaData
//����: http ״̬
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

//�½��ļ���
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//floderPath  ��ָҪ�½����ļ���·��+��
//�����jsonCreateFolder
//����: http ״̬
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

//ɾ���ļ���
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath  to_recycle
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//floderPath  ��ָҪ��ȡ���ļ�(��)·��+��
//to_recycle  �Ƿ�ɾ��������վ��
//�����jsonDelFile
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

//�ƶ��ļ�(��)
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//�����jsonMvFile
//����: http ״̬
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

//�����ļ�(��)
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//�����jsonMvFile
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

//��ȡ�ϴ��ڵ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret
//�����uploadNode
//����: http ״̬
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

//�ϴ��ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isOverWrite isAppPath   uploadNode  fromPath toPath
//�����jsonUploadFile
//����: http ״̬
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

//�����ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isAppPath   fromPath toPath
//����: http ״̬
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


//�����ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isAppPath   fromPath
//�����jsonShareFile
//����: http ״̬
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
