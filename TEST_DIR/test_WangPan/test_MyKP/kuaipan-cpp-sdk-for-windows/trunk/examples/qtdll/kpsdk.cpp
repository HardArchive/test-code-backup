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

#include "kpsdk.h"

kpSDK::kpSDK(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    eLoop=new QEventLoop;
    mRet=-1;
    mIsAppPath=true;
    mIsToRecyle=true;
    mIsOverWrite=true;
    connect(this,SIGNAL(helloworld()),this,SLOT(reqTknReplyFinished()));
}
kpSDK::~kpSDK()
{
    delete manager;
}

//签名常用参数偷懒添加···做成一个函数 囧~
void kpSDK::addTknNonceTimes(QString &paraStr, const QString &consKeyStr)
{
    paraStr.append(OACONSUMER_KEY); //cosumer_key
    paraStr.append(consKeyStr);
    paraStr.append("&");
    paraStr.append(OANONCE);        //oauth_nonce
    paraStr.append(getRandNonce());
    paraStr.append("&");
    paraStr.append(OATIMESTAMP);    //oauth_timestamp
    paraStr.append(getTimeStamp());
    //qDebug()<<getTimeStamp();
}

//打开文件
int kpSDK::openFile(QByteArray &buf, const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        buf=file.readAll();
    }
    file.close();
    return buf.size();
}

//---------------------------------------请求临时token
//发送请求requestToken
void kpSDK::reqReqTmpToken()
{
    QString reqTokenUrl=buildReqTknUrl(mConsumerKey,mConsumerSecret);
    inputUrl.setEncodedUrl(reqTokenUrl.toAscii());
    request.setUrl(inputUrl);
    mReqTknReply = manager->get(request);


    connect(mReqTknReply, SIGNAL(finished()), eLoop, SLOT(quit()));
    eLoop->exec();
    reqTknReplyFinished();
}

//构造url
QString kpSDK::buildReqTknUrl(const QString &consKeyStr, const QString &consKeySecretStr)
{
    QString para;
    QString _reqTokenUrl=KP_REQUEST_TOKEN_SRC_URL;
    _reqTokenUrl.append("?");
    _reqTokenUrl.append(OASIGNATURE);
    _reqTokenUrl.append(getReqTknSignature(para,consKeyStr,consKeySecretStr));//添加签名
    _reqTokenUrl.append("&");
    _reqTokenUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _reqTokenUrl;
}

//构造reqToken 获取签名
//  oauth_consumer_key
//  oauth_nonce
//  oauth_timestamp
QString kpSDK::getReqTknSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr)
{
    QString ReqTokenBaseUrl;
    ReqTokenBaseUrl.clear(); //将请求的URI路径进行URL编码
    ReqTokenBaseUrl.append("GET&");
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(KP_REQUEST_TOKEN_SRC_URL));
    ReqTokenBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    return  hmacSha1((consKeySecretStr+"&").toAscii(),ReqTokenBaseUrl.toAscii()); //返回签名
}

//收到临时oauth_token和secret反馈完成
void kpSDK::reqTknReplyFinished()
{
    mRet=mReqTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mReqTknReply->readAll();

    if(KP_CORRECT_RESULT==mRet)//返回正确
    {
        QList<QByteArray> list=getBuf.simplified().split('"');
        mTmpTokenSecret=list.at(TMP_TOKEN_SECRET_INDEX);
        mTmpToken=list.at(TMP_TOKEN_INDEX);
        //setAuthorise(mTmpToken);//进行鉴权

    }
    else
    {
        mTmpToken="NULL";
        mTmpTokenSecret="NULL";
    }
    mReqTknReply->deleteLater();
    eLoop->exit();
}

//---------------------------------------------请求正式token
//请求正式token
void kpSDK::reqAcessToken()
{
    QString acesTknUrl=buildAcesTknUrl(mConsumerKey,mConsumerSecret,\
                                       mTmpToken,mTmpTokenSecret);
    inputUrl.setEncodedUrl(acesTknUrl.toAscii());
    request.setUrl(inputUrl);
    mAcesTknReply=manager->get(request);
    connect(mAcesTknReply, SIGNAL(finished()), eLoop, SLOT(quit()));
    eLoop->exec();
    acesTknReplyFinished();
}

//构造请求accessToken url
QString kpSDK::buildAcesTknUrl(const QString &consKeyStr, const QString &consKeySecretStr, \
                               const QString &tmpTknStr, const QString &tmpTknSercetStr)
{
    QString para;
    QString _acesTknUrl=KP_ACCESS_TOKEN_SRC_URL;
    _acesTknUrl.append("?");
    _acesTknUrl.append(OASIGNATURE);
    _acesTknUrl.append(getAcesTknSignature(para,consKeyStr,consKeySecretStr,\
                                           tmpTknStr,tmpTknSercetStr));
    //添加签名
    _acesTknUrl.append("&");
    _acesTknUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _acesTknUrl;
}

//构造请求accessToken 签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getAcesTknSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                   const QString &tmpTknStr, const QString &tmpTknSercetStr)
{
    QString acesTknBaseUrl;
    acesTknBaseUrl.clear(); //将请求的URI路径进行URL编码
    acesTknBaseUrl.append("GET&");
    acesTknBaseUrl.append(QUrl::toPercentEncoding(KP_ACCESS_TOKEN_SRC_URL));
    acesTknBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);
    paraStr.append(tmpTknStr);

    acesTknBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串

    QString secret=consKeySecretStr+"&";
    secret=secret.append(tmpTknSercetStr);
    return  hmacSha1(secret.toAscii(),acesTknBaseUrl.toAscii()); //返回签名
}

//反馈完成 收到accessToken
void kpSDK::acesTknReplyFinished()
{
    mRet=mAcesTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mAcesTknReply->readAll();
    if(KP_CORRECT_RESULT==mRet)//返回正确
    {
        QList<QByteArray> list=getBuf.simplified().split('"');
        mAuTokenInfo.auTokenSecret=list.at(AUTHOR_TOKEN_SECRET_INDEX);
        mAuTokenInfo.auToken=list.at(AUTHOR_TOKEM_INDEX);
        mAuTokenInfo.chargedDir=list.at(AUTHOR_USR_DIR);
        QString tmpUsrId=list.at(AUTHOR_USR_ID).trimmed();
        mAuTokenInfo.usrID=tmpUsrId.mid(2,tmpUsrId.length()-3);
    }
    else
    {
        mAuTokenInfo.auTokenSecret="NULL";
        mAuTokenInfo.auToken="NULL";
        mAuTokenInfo.chargedDir="NULL";
        mAuTokenInfo.usrID="NULL";
    }
    mAcesTknReply->deleteLater();
    eLoop->exit();
}

//-----------------------------------------请求用户信息
//请求用户信息
void kpSDK::reqUsrInfo()
{
    QString usrInfoUrl=buildUsrInfoUrl(mConsumerKey,mConsumerSecret,\
                                       mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret);
    inputUrl.setEncodedUrl(usrInfoUrl.toAscii());
    request.setUrl(inputUrl);
    mUsrInfoReply=manager->get(request);
    connect(mUsrInfoReply, SIGNAL(finished()), eLoop, SLOT(quit()));
    eLoop->exec();
    usrInfoReplyFinished();
}

//构造获取用户信息 url
QString kpSDK::buildUsrInfoUrl(const QString &consKeyStr, const QString &consKeySecretStr, \
                               const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString para;
    QString _usrInfoUrl=KP_GET_USR_INFO_URL;
    _usrInfoUrl.append("?");
    _usrInfoUrl.append(OASIGNATURE);
    _usrInfoUrl.append(getUsrInfoSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr));
    //添加签名
    _usrInfoUrl.append("&");
    _usrInfoUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _usrInfoUrl;
}

//构造获取用户信息 的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getUsrInfoSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr,\
                                   const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString getUsrInfoBaseUrl;
    getUsrInfoBaseUrl.clear(); //将请求的URI路径进行URL编码
    getUsrInfoBaseUrl.append("GET&");
    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(KP_GET_USR_INFO_URL));
    getUsrInfoBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串

    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),getUsrInfoBaseUrl.toAscii()); //返回签名
}

//收到 用户信息
void kpSDK::usrInfoReplyFinished()
{
    mRet=mUsrInfoReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUsrInfoReply->readAll();
    //qDebug()<<getBuf;
    if(KP_CORRECT_RESULT==mRet)//返回正确
    {
        QList<QByteArray> list=getBuf.split('"');
        QString tmpStr;//去后面一位空格，然后去2~（len-3）位
        tmpStr=list.at(USR_MAX_FILE_SIZE_INDEX).trimmed();//": 314572800, "
        mUsrInfo.maxFileSize=tmpStr.mid(2,tmpStr.length()-3);

        tmpStr=list.at(USR_TOTAL_SIZE_INDEX).trimmed();
        mUsrInfo.quotaTotal=tmpStr.mid(2,tmpStr.length()-3);

        tmpStr=list.at(USR_USED_SIZE_INDEX).trimmed();
        mUsrInfo.quotaUsed=tmpStr.mid(2,tmpStr.length()-3);
        mUsrInfo.usrName=list.at(USR_NAME_INDEX);
    }
    else
    {
        mUsrInfo.maxFileSize="NULL";
        mUsrInfo.quotaTotal="NULL";
        mUsrInfo.quotaUsed="NULL";
        mUsrInfo.usrName="NULL";
    }
    mUsrInfoReply->deleteLater();
    eLoop->exit();
}

//-------------------------------------------------获取文件夹信息
//获取文件夹信息
void kpSDK::reqMetaData()
{
    QString dataPath=QUrl::toPercentEncoding(mDataPath);
    QString metaDataUrl=buildMetaDataUrl(mConsumerKey,mConsumerSecret,\
                                       mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                         mIsAppPath,dataPath);
    //qDebug()<<metaDataUrl;
    inputUrl.setEncodedUrl(metaDataUrl.toAscii());
    request.setUrl(inputUrl);
    mMetaDataReply=manager->get(request);

    connect(mMetaDataReply, SIGNAL(finished()), eLoop, SLOT(quit()));
    eLoop->exec();
    metaDataReplyFinished();
}

QString kpSDK::buildMetaDataUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                const QString &oauTknStr, const QString &oauTknSercetStr,\
                                bool &isAppPath, QString &dataPath)
{
    QString para;
    QString newGetMetaDataUrl(KP_GET_META_DATA_URL);
    if(isAppPath)
    {
        newGetMetaDataUrl.append("app_folder");
    }
    else
    {
        newGetMetaDataUrl.append("kuaipan");
    }
    if(dataPath.length()>0)
    {
        newGetMetaDataUrl.append("/");
        newGetMetaDataUrl.append(dataPath);
    }
    QString _metaDataUrl=newGetMetaDataUrl;
    _metaDataUrl.append("?");
    _metaDataUrl.append(OASIGNATURE);
    _metaDataUrl.append(getMetaDataSignature(para,consKeyStr,consKeySecretStr,\
                                             oauTknStr,oauTknSercetStr,\
                                             newGetMetaDataUrl));

    //添加签名
    _metaDataUrl.append("&");
    _metaDataUrl.append(removeUrlEncode(para));//添加去掉编码的参数
    return _metaDataUrl;
}

//构造获取文件夹信息的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getMetaDataSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                    const QString &oauTknStr, const QString &oauTknSercetStr, \
                                    QString &newGetMetaUrl)
{
    QString getMetaDataBaseUrl;
    getMetaDataBaseUrl.clear(); //将请求的URI路径进行URL编码
    getMetaDataBaseUrl.append("GET&");
    getMetaDataBaseUrl.append(QUrl::toPercentEncoding(newGetMetaUrl));
    getMetaDataBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    getMetaDataBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串

    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),getMetaDataBaseUrl.toAscii()); //返回签名
}

//返回可读文件(夹)信息
void kpSDK::metaDataReadReady()
{
    mRet=mMetaDataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mMetaDataReply->readAll();
//        QString str=QString(getBuf).remove(' ');
//        str=str.remove(',');
//        str=str.remove('\n');
//        QList<QString> list=str.split('{');
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);//返回的json不做任何处理

}

//返回可读文件(夹)信息完毕
void kpSDK::metaDataReplyFinished()
{
    mRet=mMetaDataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mMetaDataReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mMetaDataReply->deleteLater();
    eLoop->exit();
}

//----------------------------------------------新建文件夹
//创建文件夹
void kpSDK::reqCreateFloder()
{
    QString createFolderUrl=buildCreateFolderUrl(mConsumerKey,mConsumerSecret,\
                                                 mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                                 mIsAppPath,mCreateFolderPath);
    inputUrl.setEncodedUrl(createFolderUrl.toAscii());
    request.setUrl(inputUrl);
    mCreateFolderReply=manager->get(request);
//    connect(mCreateFolderReply,SIGNAL(readyRead()),this,SLOT(createFolderReadReady()));
    //    connect(mCreateFolderReply,SIGNAL(finished()),this,SLOT(createFolderReplyFinished()));
    connect(mCreateFolderReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    createFolderReplyFinished();
}

//构造创建文件夹rul
QString kpSDK::buildCreateFolderUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                    const QString &oauTknStr, const QString &oauTknSercetStr, \
                                    bool &isAppPath, QString &folderName)
{
    QString para;
    QString _createFolderUrl=KP_CREATE_FOLDER_URL;
    _createFolderUrl.append("?");
    _createFolderUrl.append(OASIGNATURE);
    _createFolderUrl.append(getCreateFolderSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr,isAppPath,folderName));

    //添加签名
    _createFolderUrl.append("&");
    _createFolderUrl.append(para);//添加无编码的参数
    return _createFolderUrl;
}

//构造创建文件夹的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//path
//root
QString kpSDK::getCreateFolderSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr,\
                                        const QString &oauTknStr, const QString &oauTknSercetStr,\
                                        bool &isAppPath, QString &folderName)
{
    QString createFolderBaseUrl;
    createFolderBaseUrl.clear(); //将请求的URI路径进行URL编码
    createFolderBaseUrl.append("GET&");
    createFolderBaseUrl.append(QUrl::toPercentEncoding(KP_CREATE_FOLDER_URL));
    createFolderBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    paraStr.append("&");
    paraStr.append(FOLDER_NAME);
    paraStr.append(QString(QUrl::toPercentEncoding(folderName)));
    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);
    if(isAppPath)
    {
        paraStr.append("app_folder");
    }
    else
    {
        paraStr.append("kuaipan");
    }

    createFolderBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),createFolderBaseUrl.toAscii()); //返回签名
}

//创建文件夹 反馈完毕
void kpSDK::createFolderReplyFinished()
{
    mRet=mCreateFolderReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mCreateFolderReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mCreateFolderReply->deleteLater();
    eLoop->exit();
}

//-------------------------------------------删除文件
//删除文件
void kpSDK::reqDelFile()
{
    QString delFileUrl=buildDelFileUrl(mConsumerKey,mConsumerSecret,\
                                       mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                       mIsAppPath,mDelFilePath,mIsToRecyle);

    inputUrl.setEncodedUrl(delFileUrl.toAscii());
    request.setUrl(inputUrl);
    mDelFileReply=manager->get(request);
    connect(mDelFileReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    delFileReplyFinished();
}

//构造删除文件的url
QString kpSDK::buildDelFileUrl(const QString &consKeyStr, const QString &consKeySecretStr, \
                               const QString &oauTknStr, const QString &oauTknSercetStr, \
                               bool &isAppPath, QString &fileName, bool &toRecyle)
{
    QString para;
    QString _delFileUrl=KP_DEL_FILE_URL;
    _delFileUrl.append("?");
    _delFileUrl.append(OASIGNATURE);
    _delFileUrl.append(getDelFileSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr,isAppPath,fileName,toRecyle));

    //添加签名
    _delFileUrl.append("&");
    _delFileUrl.append(para);//添加无编码的参数
    return _delFileUrl;
}

//构造删除文件 的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//path
//root
//to_recycle
QString kpSDK::getDelFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                   const QString &oauTknStr, const QString &oauTknSercetStr, \
                                   bool &isAppPath, QString &fileName, bool &toRecyle)
{
    QString delFileBaseUrl;
    delFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    delFileBaseUrl.append("GET&");
    delFileBaseUrl.append(QUrl::toPercentEncoding(KP_DEL_FILE_URL));
    delFileBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);
    paraStr.append("&");
    paraStr.append(FOLDER_NAME);
    paraStr.append(QString(QUrl::toPercentEncoding(fileName)));
    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);
    if(isAppPath)
    {
        paraStr.append("app_folder&");
    }
    else
    {
        paraStr.append("kuaipan&");
    }
    paraStr.append(TO_RECYLE);
    if(toRecyle)
    {
        paraStr.append("True");
    }
    else paraStr.append("False");

    delFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),delFileBaseUrl.toAscii()); //返回签名
}

//删除文件反馈 完毕
void kpSDK::delFileReplyFinished()
{
    mRet=mDelFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mDelFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mDelFileReply->deleteLater();
    eLoop->exit();
}

//-----------------------------------------剪切文件
//请求剪切文件
void kpSDK::reqMoveFile()
{
    QString moveFileUrl=buildMoveFileUrl(mConsumerKey,mConsumerSecret,\
                                         mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                         mIsAppPath,mFromPath,mToPath);
    inputUrl.setEncodedUrl(moveFileUrl.toAscii());
    request.setUrl(inputUrl);
    mMvFileReply=manager->get(request);
    connect(mMvFileReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    moveFileReplyFinished();
}

//构造剪切文件的url
QString kpSDK::buildMoveFileUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                const QString &oauTknStr, const QString &oauTknSercetStr, \
                                bool &isAppPath, QString &fromPath, QString &toPath)
{
    QString para;
    QString _mvFileUrl=KP_MOVE_FILE_URL;
    _mvFileUrl.append("?");
    _mvFileUrl.append(OASIGNATURE);
    _mvFileUrl.append(getMoveFileSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr,isAppPath,fromPath,toPath));
    //添加签名
    _mvFileUrl.append("&");
    _mvFileUrl.append(para);//添加无编码的参数
    return _mvFileUrl;
}

//构造剪切文件的签名
//from_path
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//root
//to_path
QString kpSDK::getMoveFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                    const QString &oauTknStr, const QString &oauTknSercetStr, \
                                    bool &isAppPath, QString &fromPath, QString &toPath)
{
    QString mvFileBaseUrl;
    mvFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    mvFileBaseUrl.append("GET&");
    mvFileBaseUrl.append(QUrl::toPercentEncoding(KP_MOVE_FILE_URL));
    mvFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);
    if(isAppPath)
    {
        paraStr.append("app_folder&");
    }
    else
    {
        paraStr.append("kuaipan&");
    }
    paraStr.append(DATA_TO_PATH);
    paraStr.append(QUrl::toPercentEncoding(toPath));

    mvFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),mvFileBaseUrl.toAscii()); //返回签名
}

//剪切文件后反馈完毕
void kpSDK::moveFileReplyFinished()
{
    mRet=mMvFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mMvFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mMvFileReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------复制文件
//请求复制文件
void kpSDK::reqCopyFile()
{

    QString copyFileUrl=buildCopyFileUrl(mConsumerKey,mConsumerSecret,\
                                         mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                         mIsAppPath,mFromPath,mToPath);
    inputUrl.setEncodedUrl(copyFileUrl.toAscii());
    request.setUrl(inputUrl);
    mCpFileReply=manager->get(request);

    connect(mCpFileReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    copyFileReplyFinished();
}

//构造复制文件的 url
QString kpSDK::buildCopyFileUrl(const QString &consKeyStr, const QString &consKeySecretStr, \
                                const QString &oauTknStr, const QString &oauTknSercetStr,\
                                bool &isAppPath, QString &fromPath, QString &toPath)
{
    QString para;
    QString _cpFileUrl=KP_COPY_FILE_URL;
    _cpFileUrl.append("?");
    _cpFileUrl.append(OASIGNATURE);
    _cpFileUrl.append(getCopyFileSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr,isAppPath,fromPath,toPath));
    //添加签名
    _cpFileUrl.append("&");
    _cpFileUrl.append(para);//添加无编码的参数
    return _cpFileUrl;

}

//构造复制文件的 签名
//from_path
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//root
//to_path
QString kpSDK::getCopyFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                    const QString &oauTknStr, const QString &oauTknSercetStr, \
                                    bool &isAppPath, QString &fromPath, QString &toPath)
{
    QString cpFileBaseUrl;
    cpFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    cpFileBaseUrl.append("GET&");
    cpFileBaseUrl.append(QUrl::toPercentEncoding(KP_COPY_FILE_URL));
    cpFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);
    if(isAppPath)
    {
        paraStr.append("app_folder&");
    }
    else
    {
        paraStr.append("kuaipan&");
    }
    paraStr.append(DATA_TO_PATH);
    paraStr.append(QUrl::toPercentEncoding(toPath));

    cpFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),cpFileBaseUrl.toAscii()); //返回签名
}

//复制文件 反馈结束
void kpSDK::copyFileReplyFinished()
{
    mRet=mCpFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mCpFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mCpFileReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------获取上传节点
//获取上传节点
void kpSDK::reqUploadLocate()
{
    QString uploadLocUrl=buildUploadLocateUrl(mConsumerKey,mConsumerSecret,\
                                              mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret);
    inputUrl.setEncodedUrl(uploadLocUrl.toAscii());
    request.setUrl(inputUrl);
    mUploadLocateReply=manager->get(request);
    connect(mUploadLocateReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    getUploadLocateFinished();
}

//构造获取上传节点 url
QString kpSDK::buildUploadLocateUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                    const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString para;
    QString _locateUrl=KP_UPLOAD_LOCATE_URL;
    _locateUrl.append("?");
    _locateUrl.append(OASIGNATURE);
    _locateUrl.append(getUploadLocateSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr));
    //添加签名
    _locateUrl.append("&");
    _locateUrl.append(para);//添加无编码的参数
    return _locateUrl;
}

//构造获取上传节点 签名
QString kpSDK::getUploadLocateSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                        const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString upLocaBaseUrl;
    upLocaBaseUrl.clear(); //将请求的URI路径进行URL编码
    upLocaBaseUrl.append("GET&");
    upLocaBaseUrl.append(QUrl::toPercentEncoding(KP_UPLOAD_LOCATE_URL));
    upLocaBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    upLocaBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),upLocaBaseUrl.toAscii()); //返回签名
}

//获取到上传节点
void kpSDK::getUploadLocateFinished()
{
    mRet=mUploadLocateReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadLocateReply->readAll();
    if(KP_CORRECT_RESULT==mRet)
    {
        QList<QByteArray> list=getBuf.split('"');
        mUploadNode=list.at(3);//上传节点
    }
    else
    {
        mUploadNode.clear();
        mUploadNode.append(getBuf);
    }
    mUploadLocateReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------上传文件
//上传文件
void kpSDK::reqUploadFile()
{
    QByteArray data;
    QString uploadFileUrl=buildUploadFileUrl(mConsumerKey,mConsumerSecret,\
                                             mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                             mIsOverWrite,mIsAppPath,mToPath,mUploadNode);

    inputUrl.setEncodedUrl(uploadFileUrl.toAscii());
    uploadRequest.setUrl(inputUrl);
    buildMultiPart(data,mFromPath);//使用multipart方式上传 构造multipart包
    mUploadFileReply=manager->post(uploadRequest,data);//上传
    connect(mUploadFileReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    uploadFileReplyFinished();
}

//构造上传文件 url
QString kpSDK::buildUploadFileUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                  const QString &oauTknStr, const QString &oauTknSercetStr, \
                                  const bool &isOverWrite, const bool &isAppPath, const QString &toPath, const QString &preStr)
{
    QString para;
    QString fullUploadFileUrl=preStr+KP_UPLOAD_FILE_URL;
    QString _sendFileUrl=fullUploadFileUrl;
    _sendFileUrl.append("?");
    _sendFileUrl.append(OASIGNATURE);
    _sendFileUrl.append(getUploadFileSignature(para,consKeyStr,consKeySecretStr,\
                                               oauTknStr,oauTknSercetStr,\
                                               isOverWrite,isAppPath,\
                                               toPath,fullUploadFileUrl));
    //添加签名
    _sendFileUrl.append("&");
    _sendFileUrl.append(para);//添加无编码的参数
    return _sendFileUrl;
}

//构造上传文件 签名
QString kpSDK::getUploadFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                      const QString &oauTknStr, const QString &oauTknSercetStr,\
                                      const bool &isOverWrite, const bool &isAppPath, const QString &toPath, const QString &fullUrl)
{
    QString uploadFileBaseUrl;
    uploadFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    uploadFileBaseUrl.append("POST&");
    uploadFileBaseUrl.append(QUrl::toPercentEncoding(fullUrl));
    uploadFileBaseUrl.append("&");


    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    paraStr.append("&");
    paraStr.append(UPLOAD_OVER_WRITE); //overwrite
    if(isOverWrite)
        paraStr.append("True");
    else paraStr.append("False");

    paraStr.append("&");
    paraStr.append(FOLDER_NAME);       //path
    paraStr.append(QUrl::toPercentEncoding(toPath));
    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);        //root
    if(isAppPath)
    {
        paraStr.append("app_folder");
    }
    else
    {
        paraStr.append("kuaipan");
    }

    uploadFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),uploadFileBaseUrl.toAscii()); //返回签名
}

//构造MultiPart包
void kpSDK::buildMultiPart(QByteArray &data,const QString &fileName)
{
    QByteArray needToUploadFile;
    if(openFile(needToUploadFile,fileName)<=0)
    {//文件打开失败
        return ;
    }

    QString crlf="\r\n";
    QString boundary="--------Michael"+getRandNonce();
    QString contentType="multipart/form-data; boundary="+boundary; //开始的头部信息

    QString endBoundary=crlf+"--"+boundary+"--"+crlf; //结束处

    boundary="--"+boundary+crlf; //每行数据开头
    QByteArray bond=boundary.toAscii();

    data.append(bond);


    data.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\""\
                        +fileName+"\""+crlf).toAscii());
    data.append(QString("Content-Type: application/octet-stream"+crlf).toAscii());    

    data.append(crlf.toAscii());
    data.append(needToUploadFile);
    data.append(endBoundary.toAscii());
    uploadRequest.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toAscii());
    uploadRequest.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data.size()).toString());
}

//上传文件完毕
void kpSDK::uploadFileReplyFinished()
{
    mRet=mUploadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mUploadFileReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------下载文件
//下载文件
void kpSDK::reqDownLoadFile()
{
    mDwnFileBuf.clear();

    dwnFile=new QFile(mToPath);
    if(!dwnFile->open(QIODevice::WriteOnly))
    {   //如果打开文件失败，则删除file，并使file指针为0，然后返回
        delete dwnFile;
        dwnFile =NULL;
        return;
    }
    QString downLoadFileUrl=buildDownFileUrl(mConsumerKey,mConsumerSecret,\
                                             mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                             mIsAppPath,mFromPath);
    inputUrl.setEncodedUrl(downLoadFileUrl.toAscii());
    request.setUrl(inputUrl);
    mDownloadFileReply=manager->get(request);
    connect(mDownloadFileReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    downLoadFileReplyFinished();
}

//构造 下载文件 url
QString kpSDK::buildDownFileUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                const QString &oauTknStr, const QString &oauTknSercetStr, \
                                bool &isAppPath, const QString &fromPath)
{
    QString para;
    QString _dwnFileUrl=KP_DOWNLOAD_FILE_URL;
    _dwnFileUrl.append("?");
    _dwnFileUrl.append(OASIGNATURE);
    _dwnFileUrl.append(getDownFileSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr,isAppPath,fromPath));
    //添加签名
    _dwnFileUrl.append("&");
    _dwnFileUrl.append(para);//添加无编码的参数
    return _dwnFileUrl;
}

//构造 下载文件的签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
//path
//root
QString kpSDK::getDownFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                    const QString &oauTknStr, const QString &oauTknSercetStr, \
                                    bool &isAppPath, const QString &fromPath)
{
    QString dwnFileBaseUrl;
    dwnFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    dwnFileBaseUrl.append("GET&");
    dwnFileBaseUrl.append(QUrl::toPercentEncoding(KP_DOWNLOAD_FILE_URL));
    dwnFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    paraStr.append("&");
    paraStr.append(FOLDER_NAME);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");
    paraStr.append(FOLDER_ROOT);
    if(isAppPath)
    {
        paraStr.append("app_folder");
    }
    else
    {
        paraStr.append("kuaipan");
    }

    dwnFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),dwnFileBaseUrl.toAscii()); //返回签名
}

//反馈完毕 获取重定向地址
void kpSDK::downLoadFileReplyFinished()
{
    mRet = mDownloadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(301==mRet ||  302==mRet)//重定位值
    {
        QEventLoop   *downloadEloop=new QEventLoop();
        for(int i=0;i<mDownloadFileReply->rawHeaderPairs().count();i++)
        {
            //qDebug()<<"Pairs()"<<mDownloadFileReply->rawHeaderPairs().at(i) ;
            if( "Location"==mDownloadFileReply->rawHeaderPairs().at(i).first)
            {
                QUrl realDwnFileUrl=QUrl(mDownloadFileReply->rawHeaderPairs().at(i).second);
                mDwnFileRealReply=manager->get(QNetworkRequest(realDwnFileUrl));//用重定向地址重新下载

                connect(mDwnFileRealReply,SIGNAL(finished()),downloadEloop,SLOT(quit()));
                downloadEloop->exec();
                dwnFileRealReplyFinished();
                break;
            }
        }
        downloadEloop->exit();
    }
    mDownloadFileReply->deleteLater();
    eLoop->exit();
}

//下载完成 写入文件
void kpSDK::dwnFileRealReplyFinished()
{
    mRet=mDwnFileRealReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(KP_CORRECT_RESULT==mRet)//返回正确
    {
        if (dwnFile!=NULL) dwnFile->write(mDwnFileRealReply->readAll());
    }
    dwnFile->flush();
    dwnFile->close();
    delete dwnFile;
    dwnFile=NULL;
    mDwnFileRealReply->deleteLater();
}

//--------------------------------------分享文件
//请求分享文件
void kpSDK::reqShareFile()
{
    QString shareFile=QString(QUrl::toPercentEncoding(mFromPath));
    QString shareFileUrl=buildShareFileUrl(mConsumerKey,mConsumerSecret,\
                                           mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                           mIsAppPath,shareFile);
    inputUrl.setEncodedUrl(shareFileUrl.toAscii());
    request.setUrl(inputUrl);
    mShareFileReply=manager->get(request);
    connect(mShareFileReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    shareFileReplyFinished();
}

//构造分享文件 url
QString kpSDK::buildShareFileUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                 const QString &oauTknStr, const QString &oauTknSercetStr, \
                                 bool &isAppPath, const QString &dataPath)
{
    QString para;
    QString newShareFileUrl(KP_SHARE_FILE_URL);
    if(isAppPath)
    {
        newShareFileUrl.append("app_folder");
    }
    else newShareFileUrl.append("kuaipan");
    if(dataPath.length()>0)
    {
        newShareFileUrl.append("/");
        newShareFileUrl.append(dataPath);
    }

    QString _shareFileUrl=newShareFileUrl;
    _shareFileUrl.append("?");
    _shareFileUrl.append(OASIGNATURE);
    _shareFileUrl.append(getShareFileSignature(para,consKeyStr,consKeySecretStr,\
                                               oauTknStr,oauTknSercetStr,\
                                               newShareFileUrl));
    //添加签名
    _shareFileUrl.append("&");
    _shareFileUrl.append(para);//添加无编码的参数
    return _shareFileUrl;
}

//构造 分享文件 签名
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getShareFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                     const QString &oauTknStr, const QString &oauTknSercetStr, \
                                     const QString &newShareFileUrl)
{
    QString shareFileBaseUrl;
    shareFileBaseUrl.clear(); //将请求的URI路径进行URL编码
    shareFileBaseUrl.append("GET&");
    shareFileBaseUrl.append(QUrl::toPercentEncoding(newShareFileUrl));
    shareFileBaseUrl.append("&");

    //对参数进行升序排列 然后合并
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    shareFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// 对参数进行编码 然后合并成源串
    QString secret=consKeySecretStr+"&";//构建密钥
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),shareFileBaseUrl.toAscii()); //返回签名
}

//反馈完毕 提取分享的url
void kpSDK::shareFileReplyFinished()
{
    mRet=mShareFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mShareFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mShareFileReply->deleteLater();
    eLoop->exit();
}



