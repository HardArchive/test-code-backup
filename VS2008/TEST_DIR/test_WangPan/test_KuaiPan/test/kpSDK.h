#ifndef KPSDK_H
#define KPSDK_H
#include <iostream>
using namespace std;


//

//获取临时token、secret和认证url
//输入：consumerKey    consumerSecret
//输出：tmpOauthToken  tmpOauthTokenSecret     authoriseUrl
//返回: http 状态
extern "C" __declspec(dllexport) int kpReqTmpToken(string &tmpOauthToken,string &tmpOauthTokenSecret,\
                                                   string &authoriseUrl,\
                                                   const string &consumerKey,const string &consumerSecret);

//获取accessToken
//输入:	tmpOauthToken	tmpOauthTokenSecret		consumerKey		consumerSecret
//输出：oauthToken		oauthTokenSecret		usrID			chargedDir
//返回  请求token的HTTP状态码
extern "C" __declspec(dllexport) int kpAccessToken(string &oauthToken, string &oauthTokenSecret,\
                                                   string &usrID,      string &chargedDir,\
                                                   const string &consumerKey,const string &consumerSecret,\
                                                   const string &tmpOauthToken,const string &tmpOauthTokenSecret
                                                   );

//获取用户信息
//输入: consumerKey consumerSecret      oauthToken    oauthTokenSecret
//输出：max_file_size   user_name       quota_used       quota_total
//返回  请求token的HTTP状态码
extern "C" __declspec(dllexport) int kpUsrInfo(string &max_file_size, string &user_name,\
                                               string &quota_used,    string &quota_total,\
                                               const string &consumerKey,const string &consumerSecret,\
                                               const string &oauthToken,const string &oauthTokenSecret
                                               );

//获取文件(夹)信息
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   dataPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//dataPath  是指要获取的文件(夹)路径+名
//输出：jsonMetaData
//返回  请求token的HTTP状态码
extern "C" __declspec(dllexport) int kpMetaData(string &jsonMetaData,\
                                                const string &consumerKey,const string &consumerSecret,\
                                                const string &oauthToken,const string &oauthTokenSecret,
                                                const bool   &isAppPath, const string &dataPath
                                                );

//新建文件夹
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//floderPath  是指要获取的文件(夹)路径+名
//输出：jsonCreateFolder
//返回  请求token的HTTP状态码
extern "C" __declspec(dllexport) int kpCreateFolder(string &jsonCreateFolder,\
                                                    const string &consumerKey,const string &consumerSecret,\
                                                    const string &oauthToken,const string &oauthTokenSecret,
                                                    const bool   &isAppPath, const string &floderPath
                                                    );

//删除文件夹
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath  to_recycle
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//floderPath  是指要获取的文件(夹)路径+名
//to_recycle  是否删除到回收站？
//输出：jsonDelFile
//返回  请求token的HTTP状态码
extern "C" __declspec(dllexport) int kpDelFile(string &jsonDelFile,\
                                               const string &consumerKey,const string &consumerSecret,\
                                               const string &oauthToken,const string &oauthTokenSecret,
                                               const bool   &isAppPath, const string &floderPath,
                                               const bool   &to_recycle
                                               );

//移动文件夹
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//输出：jsonMvFile
//返回  请求token的HTTP状态码
extern "C" __declspec(dllexport) int kpMvFile(string &jsonMvFile,\
                                              const string &consumerKey,const string &consumerSecret,\
                                              const string &oauthToken,const string &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const string &fromPath, const string &toPath
                                              );

//复制文件(夹)
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath 若应用访问权限路径为kuaipan则为false,否为true
//输出：jsonMvFile
//返回  请求token的HTTP状态码
extern "C" __declspec(dllexport) int kpCpFile(string &jsonCpFile,\
                                              const string &consumerKey,const string &consumerSecret,\
                                              const string &oauthToken,const string &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const string &fromPath, const string &toPath
                                              );

//获取上传节点
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret
//输出：uploadNode
//返回: http 状态
extern "C" __declspec(dllexport) int kpUploadLocate(string &uploadNode,\
                                                    const string &consumerKey,const string &consumerSecret,\
                                                    const string &oauthToken,const string &oauthTokenSecret
                                                    );

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
                                                  );

//下载文件
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    、
//     isAppPath   fromPath toPath
//返回: http 状态
extern "C" __declspec(dllexport) int kpDownloadFile(const string &consumerKey,const string &consumerSecret,\
                                                    const string &oauthToken,const string &oauthTokenSecret,\
                                                    const bool &isAppPath,const string &fromPath,
                                                    const string &toPath
                                                    );

//分享文件
//输入: consumerKey consumerSecret  oauthToken    oauthTokenSecret    、
//     isAppPath   fromPath
//输出：jsonShareFile
//返回: http 状态
extern "C" __declspec(dllexport) int kpShareFile(string &jsonShareFile,
                                                 const string &consumerKey,const string &consumerSecret,\
                                                 const string &oauthToken,const string &oauthTokenSecret,\
                                                 const bool &isAppPath,const string &fromPath
                                                 );

#endif


