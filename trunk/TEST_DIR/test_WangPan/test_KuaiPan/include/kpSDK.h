#ifndef KPSDK_H
#define KPSDK_H
#include <iostream>
using namespace std;


//

//��ȡ��ʱtoken��secret����֤url
//���룺consumerKey    consumerSecret
//�����tmpOauthToken  tmpOauthTokenSecret     authoriseUrl
//����: http ״̬
extern "C" __declspec(dllexport) int kpReqTmpToken(string &tmpOauthToken,string &tmpOauthTokenSecret,\
                                                   string &authoriseUrl,\
                                                   const string &consumerKey,const string &consumerSecret);

//��ȡaccessToken
//����:	tmpOauthToken	tmpOauthTokenSecret		consumerKey		consumerSecret
//�����oauthToken		oauthTokenSecret		usrID			chargedDir
//����  ����token��HTTP״̬��
extern "C" __declspec(dllexport) int kpAccessToken(string &oauthToken, string &oauthTokenSecret,\
                                                   string &usrID,      string &chargedDir,\
                                                   const string &consumerKey,const string &consumerSecret,\
                                                   const string &tmpOauthToken,const string &tmpOauthTokenSecret
                                                   );

//��ȡ�û���Ϣ
//����: consumerKey consumerSecret      oauthToken    oauthTokenSecret
//�����max_file_size   user_name       quota_used       quota_total
//����  ����token��HTTP״̬��
extern "C" __declspec(dllexport) int kpUsrInfo(string &max_file_size, string &user_name,\
                                               string &quota_used,    string &quota_total,\
                                               const string &consumerKey,const string &consumerSecret,\
                                               const string &oauthToken,const string &oauthTokenSecret
                                               );

//��ȡ�ļ�(��)��Ϣ
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   dataPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//dataPath  ��ָҪ��ȡ���ļ�(��)·��+��
//�����jsonMetaData
//����  ����token��HTTP״̬��
extern "C" __declspec(dllexport) int kpMetaData(string &jsonMetaData,\
                                                const string &consumerKey,const string &consumerSecret,\
                                                const string &oauthToken,const string &oauthTokenSecret,
                                                const bool   &isAppPath, const string &dataPath
                                                );

//�½��ļ���
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//floderPath  ��ָҪ��ȡ���ļ�(��)·��+��
//�����jsonCreateFolder
//����  ����token��HTTP״̬��
extern "C" __declspec(dllexport) int kpCreateFolder(string &jsonCreateFolder,\
                                                    const string &consumerKey,const string &consumerSecret,\
                                                    const string &oauthToken,const string &oauthTokenSecret,
                                                    const bool   &isAppPath, const string &floderPath
                                                    );

//ɾ���ļ���
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath  to_recycle
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//floderPath  ��ָҪ��ȡ���ļ�(��)·��+��
//to_recycle  �Ƿ�ɾ��������վ��
//�����jsonDelFile
//����  ����token��HTTP״̬��
extern "C" __declspec(dllexport) int kpDelFile(string &jsonDelFile,\
                                               const string &consumerKey,const string &consumerSecret,\
                                               const string &oauthToken,const string &oauthTokenSecret,
                                               const bool   &isAppPath, const string &floderPath,
                                               const bool   &to_recycle
                                               );

//�ƶ��ļ���
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//�����jsonMvFile
//����  ����token��HTTP״̬��
extern "C" __declspec(dllexport) int kpMvFile(string &jsonMvFile,\
                                              const string &consumerKey,const string &consumerSecret,\
                                              const string &oauthToken,const string &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const string &fromPath, const string &toPath
                                              );

//�����ļ�(��)
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//�����jsonMvFile
//����  ����token��HTTP״̬��
extern "C" __declspec(dllexport) int kpCpFile(string &jsonCpFile,\
                                              const string &consumerKey,const string &consumerSecret,\
                                              const string &oauthToken,const string &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const string &fromPath, const string &toPath
                                              );

//��ȡ�ϴ��ڵ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret
//�����uploadNode
//����: http ״̬
extern "C" __declspec(dllexport) int kpUploadLocate(string &uploadNode,\
                                                    const string &consumerKey,const string &consumerSecret,\
                                                    const string &oauthToken,const string &oauthTokenSecret
                                                    );

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
                                                  );

//�����ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isAppPath   fromPath toPath
//����: http ״̬
extern "C" __declspec(dllexport) int kpDownloadFile(const string &consumerKey,const string &consumerSecret,\
                                                    const string &oauthToken,const string &oauthTokenSecret,\
                                                    const bool &isAppPath,const string &fromPath,
                                                    const string &toPath
                                                    );

//�����ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isAppPath   fromPath
//�����jsonShareFile
//����: http ״̬
extern "C" __declspec(dllexport) int kpShareFile(string &jsonShareFile,
                                                 const string &consumerKey,const string &consumerSecret,\
                                                 const string &oauthToken,const string &oauthTokenSecret,\
                                                 const bool &isAppPath,const string &fromPath
                                                 );

#endif


