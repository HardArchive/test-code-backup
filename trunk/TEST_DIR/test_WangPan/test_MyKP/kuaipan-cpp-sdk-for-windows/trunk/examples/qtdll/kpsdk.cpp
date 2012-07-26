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

//ǩ�����ò���͵����ӡ���������һ������ ��~
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

//���ļ�
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

//---------------------------------------������ʱtoken
//��������requestToken
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

//����url
QString kpSDK::buildReqTknUrl(const QString &consKeyStr, const QString &consKeySecretStr)
{
    QString para;
    QString _reqTokenUrl=KP_REQUEST_TOKEN_SRC_URL;
    _reqTokenUrl.append("?");
    _reqTokenUrl.append(OASIGNATURE);
    _reqTokenUrl.append(getReqTknSignature(para,consKeyStr,consKeySecretStr));//���ǩ��
    _reqTokenUrl.append("&");
    _reqTokenUrl.append(removeUrlEncode(para));//���ȥ������Ĳ���
    return _reqTokenUrl;
}

//����reqToken ��ȡǩ��
//  oauth_consumer_key
//  oauth_nonce
//  oauth_timestamp
QString kpSDK::getReqTknSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr)
{
    QString ReqTokenBaseUrl;
    ReqTokenBaseUrl.clear(); //�������URI·������URL����
    ReqTokenBaseUrl.append("GET&");
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(KP_REQUEST_TOKEN_SRC_URL));
    ReqTokenBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    ReqTokenBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    return  hmacSha1((consKeySecretStr+"&").toAscii(),ReqTokenBaseUrl.toAscii()); //����ǩ��
}

//�յ���ʱoauth_token��secret�������
void kpSDK::reqTknReplyFinished()
{
    mRet=mReqTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mReqTknReply->readAll();

    if(KP_CORRECT_RESULT==mRet)//������ȷ
    {
        QList<QByteArray> list=getBuf.simplified().split('"');
        mTmpTokenSecret=list.at(TMP_TOKEN_SECRET_INDEX);
        mTmpToken=list.at(TMP_TOKEN_INDEX);
        //setAuthorise(mTmpToken);//���м�Ȩ

    }
    else
    {
        mTmpToken="NULL";
        mTmpTokenSecret="NULL";
    }
    mReqTknReply->deleteLater();
    eLoop->exit();
}

//---------------------------------------------������ʽtoken
//������ʽtoken
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

//��������accessToken url
QString kpSDK::buildAcesTknUrl(const QString &consKeyStr, const QString &consKeySecretStr, \
                               const QString &tmpTknStr, const QString &tmpTknSercetStr)
{
    QString para;
    QString _acesTknUrl=KP_ACCESS_TOKEN_SRC_URL;
    _acesTknUrl.append("?");
    _acesTknUrl.append(OASIGNATURE);
    _acesTknUrl.append(getAcesTknSignature(para,consKeyStr,consKeySecretStr,\
                                           tmpTknStr,tmpTknSercetStr));
    //���ǩ��
    _acesTknUrl.append("&");
    _acesTknUrl.append(removeUrlEncode(para));//���ȥ������Ĳ���
    return _acesTknUrl;
}

//��������accessToken ǩ��
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getAcesTknSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                   const QString &tmpTknStr, const QString &tmpTknSercetStr)
{
    QString acesTknBaseUrl;
    acesTknBaseUrl.clear(); //�������URI·������URL����
    acesTknBaseUrl.append("GET&");
    acesTknBaseUrl.append(QUrl::toPercentEncoding(KP_ACCESS_TOKEN_SRC_URL));
    acesTknBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);
    paraStr.append(tmpTknStr);

    acesTknBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��

    QString secret=consKeySecretStr+"&";
    secret=secret.append(tmpTknSercetStr);
    return  hmacSha1(secret.toAscii(),acesTknBaseUrl.toAscii()); //����ǩ��
}

//������� �յ�accessToken
void kpSDK::acesTknReplyFinished()
{
    mRet=mAcesTknReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mAcesTknReply->readAll();
    if(KP_CORRECT_RESULT==mRet)//������ȷ
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

//-----------------------------------------�����û���Ϣ
//�����û���Ϣ
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

//�����ȡ�û���Ϣ url
QString kpSDK::buildUsrInfoUrl(const QString &consKeyStr, const QString &consKeySecretStr, \
                               const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString para;
    QString _usrInfoUrl=KP_GET_USR_INFO_URL;
    _usrInfoUrl.append("?");
    _usrInfoUrl.append(OASIGNATURE);
    _usrInfoUrl.append(getUsrInfoSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr));
    //���ǩ��
    _usrInfoUrl.append("&");
    _usrInfoUrl.append(removeUrlEncode(para));//���ȥ������Ĳ���
    return _usrInfoUrl;
}

//�����ȡ�û���Ϣ ��ǩ��
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getUsrInfoSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr,\
                                   const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString getUsrInfoBaseUrl;
    getUsrInfoBaseUrl.clear(); //�������URI·������URL����
    getUsrInfoBaseUrl.append("GET&");
    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(KP_GET_USR_INFO_URL));
    getUsrInfoBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    getUsrInfoBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��

    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),getUsrInfoBaseUrl.toAscii()); //����ǩ��
}

//�յ� �û���Ϣ
void kpSDK::usrInfoReplyFinished()
{
    mRet=mUsrInfoReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUsrInfoReply->readAll();
    //qDebug()<<getBuf;
    if(KP_CORRECT_RESULT==mRet)//������ȷ
    {
        QList<QByteArray> list=getBuf.split('"');
        QString tmpStr;//ȥ����һλ�ո�Ȼ��ȥ2~��len-3��λ
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

//-------------------------------------------------��ȡ�ļ�����Ϣ
//��ȡ�ļ�����Ϣ
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

    //���ǩ��
    _metaDataUrl.append("&");
    _metaDataUrl.append(removeUrlEncode(para));//���ȥ������Ĳ���
    return _metaDataUrl;
}

//�����ȡ�ļ�����Ϣ��ǩ��
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getMetaDataSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                    const QString &oauTknStr, const QString &oauTknSercetStr, \
                                    QString &newGetMetaUrl)
{
    QString getMetaDataBaseUrl;
    getMetaDataBaseUrl.clear(); //�������URI·������URL����
    getMetaDataBaseUrl.append("GET&");
    getMetaDataBaseUrl.append(QUrl::toPercentEncoding(newGetMetaUrl));
    getMetaDataBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    getMetaDataBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��

    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),getMetaDataBaseUrl.toAscii()); //����ǩ��
}

//���ؿɶ��ļ�(��)��Ϣ
void kpSDK::metaDataReadReady()
{
    mRet=mMetaDataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mMetaDataReply->readAll();
//        QString str=QString(getBuf).remove(' ');
//        str=str.remove(',');
//        str=str.remove('\n');
//        QList<QString> list=str.split('{');
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);//���ص�json�����κδ���

}

//���ؿɶ��ļ�(��)��Ϣ���
void kpSDK::metaDataReplyFinished()
{
    mRet=mMetaDataReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mMetaDataReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mMetaDataReply->deleteLater();
    eLoop->exit();
}

//----------------------------------------------�½��ļ���
//�����ļ���
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

//���촴���ļ���rul
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

    //���ǩ��
    _createFolderUrl.append("&");
    _createFolderUrl.append(para);//����ޱ���Ĳ���
    return _createFolderUrl;
}

//���촴���ļ��е�ǩ��
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
    createFolderBaseUrl.clear(); //�������URI·������URL����
    createFolderBaseUrl.append("GET&");
    createFolderBaseUrl.append(QUrl::toPercentEncoding(KP_CREATE_FOLDER_URL));
    createFolderBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
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

    createFolderBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),createFolderBaseUrl.toAscii()); //����ǩ��
}

//�����ļ��� �������
void kpSDK::createFolderReplyFinished()
{
    mRet=mCreateFolderReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mCreateFolderReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mCreateFolderReply->deleteLater();
    eLoop->exit();
}

//-------------------------------------------ɾ���ļ�
//ɾ���ļ�
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

//����ɾ���ļ���url
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

    //���ǩ��
    _delFileUrl.append("&");
    _delFileUrl.append(para);//����ޱ���Ĳ���
    return _delFileUrl;
}

//����ɾ���ļ� ��ǩ��
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
    delFileBaseUrl.clear(); //�������URI·������URL����
    delFileBaseUrl.append("GET&");
    delFileBaseUrl.append(QUrl::toPercentEncoding(KP_DEL_FILE_URL));
    delFileBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
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

    delFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),delFileBaseUrl.toAscii()); //����ǩ��
}

//ɾ���ļ����� ���
void kpSDK::delFileReplyFinished()
{
    mRet=mDelFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mDelFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mDelFileReply->deleteLater();
    eLoop->exit();
}

//-----------------------------------------�����ļ�
//��������ļ�
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

//��������ļ���url
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
    //���ǩ��
    _mvFileUrl.append("&");
    _mvFileUrl.append(para);//����ޱ���Ĳ���
    return _mvFileUrl;
}

//��������ļ���ǩ��
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
    mvFileBaseUrl.clear(); //�������URI·������URL����
    mvFileBaseUrl.append("GET&");
    mvFileBaseUrl.append(QUrl::toPercentEncoding(KP_MOVE_FILE_URL));
    mvFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //�Բ��������������� Ȼ��ϲ�
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

    mvFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),mvFileBaseUrl.toAscii()); //����ǩ��
}

//�����ļ��������
void kpSDK::moveFileReplyFinished()
{
    mRet=mMvFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mMvFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mMvFileReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------�����ļ�
//�������ļ�
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

//���츴���ļ��� url
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
    //���ǩ��
    _cpFileUrl.append("&");
    _cpFileUrl.append(para);//����ޱ���Ĳ���
    return _cpFileUrl;

}

//���츴���ļ��� ǩ��
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
    cpFileBaseUrl.clear(); //�������URI·������URL����
    cpFileBaseUrl.append("GET&");
    cpFileBaseUrl.append(QUrl::toPercentEncoding(KP_COPY_FILE_URL));
    cpFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //�Բ��������������� Ȼ��ϲ�
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

    cpFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),cpFileBaseUrl.toAscii()); //����ǩ��
}

//�����ļ� ��������
void kpSDK::copyFileReplyFinished()
{
    mRet=mCpFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mCpFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mCpFileReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------��ȡ�ϴ��ڵ�
//��ȡ�ϴ��ڵ�
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

//�����ȡ�ϴ��ڵ� url
QString kpSDK::buildUploadLocateUrl(const QString &consKeyStr, const QString &consKeySecretStr,\
                                    const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString para;
    QString _locateUrl=KP_UPLOAD_LOCATE_URL;
    _locateUrl.append("?");
    _locateUrl.append(OASIGNATURE);
    _locateUrl.append(getUploadLocateSignature(para,consKeyStr,consKeySecretStr,\
                                           oauTknStr,oauTknSercetStr));
    //���ǩ��
    _locateUrl.append("&");
    _locateUrl.append(para);//����ޱ���Ĳ���
    return _locateUrl;
}

//�����ȡ�ϴ��ڵ� ǩ��
QString kpSDK::getUploadLocateSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                        const QString &oauTknStr, const QString &oauTknSercetStr)
{
    QString upLocaBaseUrl;
    upLocaBaseUrl.clear(); //�������URI·������URL����
    upLocaBaseUrl.append("GET&");
    upLocaBaseUrl.append(QUrl::toPercentEncoding(KP_UPLOAD_LOCATE_URL));
    upLocaBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    upLocaBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),upLocaBaseUrl.toAscii()); //����ǩ��
}

//��ȡ���ϴ��ڵ�
void kpSDK::getUploadLocateFinished()
{
    mRet=mUploadLocateReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadLocateReply->readAll();
    if(KP_CORRECT_RESULT==mRet)
    {
        QList<QByteArray> list=getBuf.split('"');
        mUploadNode=list.at(3);//�ϴ��ڵ�
    }
    else
    {
        mUploadNode.clear();
        mUploadNode.append(getBuf);
    }
    mUploadLocateReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------�ϴ��ļ�
//�ϴ��ļ�
void kpSDK::reqUploadFile()
{
    QByteArray data;
    QString uploadFileUrl=buildUploadFileUrl(mConsumerKey,mConsumerSecret,\
                                             mAuTokenInfo.auToken,mAuTokenInfo.auTokenSecret,\
                                             mIsOverWrite,mIsAppPath,mToPath,mUploadNode);

    inputUrl.setEncodedUrl(uploadFileUrl.toAscii());
    uploadRequest.setUrl(inputUrl);
    buildMultiPart(data,mFromPath);//ʹ��multipart��ʽ�ϴ� ����multipart��
    mUploadFileReply=manager->post(uploadRequest,data);//�ϴ�
    connect(mUploadFileReply,SIGNAL(finished()),eLoop,SLOT(quit()));
    eLoop->exec();
    uploadFileReplyFinished();
}

//�����ϴ��ļ� url
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
    //���ǩ��
    _sendFileUrl.append("&");
    _sendFileUrl.append(para);//����ޱ���Ĳ���
    return _sendFileUrl;
}

//�����ϴ��ļ� ǩ��
QString kpSDK::getUploadFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                      const QString &oauTknStr, const QString &oauTknSercetStr,\
                                      const bool &isOverWrite, const bool &isAppPath, const QString &toPath, const QString &fullUrl)
{
    QString uploadFileBaseUrl;
    uploadFileBaseUrl.clear(); //�������URI·������URL����
    uploadFileBaseUrl.append("POST&");
    uploadFileBaseUrl.append(QUrl::toPercentEncoding(fullUrl));
    uploadFileBaseUrl.append("&");


    //�Բ��������������� Ȼ��ϲ�
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

    uploadFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),uploadFileBaseUrl.toAscii()); //����ǩ��
}

//����MultiPart��
void kpSDK::buildMultiPart(QByteArray &data,const QString &fileName)
{
    QByteArray needToUploadFile;
    if(openFile(needToUploadFile,fileName)<=0)
    {//�ļ���ʧ��
        return ;
    }

    QString crlf="\r\n";
    QString boundary="--------Michael"+getRandNonce();
    QString contentType="multipart/form-data; boundary="+boundary; //��ʼ��ͷ����Ϣ

    QString endBoundary=crlf+"--"+boundary+"--"+crlf; //������

    boundary="--"+boundary+crlf; //ÿ�����ݿ�ͷ
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

//�ϴ��ļ����
void kpSDK::uploadFileReplyFinished()
{
    mRet=mUploadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mUploadFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mUploadFileReply->deleteLater();
    eLoop->exit();
}

//------------------------------------------�����ļ�
//�����ļ�
void kpSDK::reqDownLoadFile()
{
    mDwnFileBuf.clear();

    dwnFile=new QFile(mToPath);
    if(!dwnFile->open(QIODevice::WriteOnly))
    {   //������ļ�ʧ�ܣ���ɾ��file����ʹfileָ��Ϊ0��Ȼ�󷵻�
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

//���� �����ļ� url
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
    //���ǩ��
    _dwnFileUrl.append("&");
    _dwnFileUrl.append(para);//����ޱ���Ĳ���
    return _dwnFileUrl;
}

//���� �����ļ���ǩ��
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
    dwnFileBaseUrl.clear(); //�������URI·������URL����
    dwnFileBaseUrl.append("GET&");
    dwnFileBaseUrl.append(QUrl::toPercentEncoding(KP_DOWNLOAD_FILE_URL));
    dwnFileBaseUrl.append("&");

    paraStr.append(DATA_FROM_PATH);
    paraStr.append(QUrl::toPercentEncoding(fromPath));
    paraStr.append("&");

    //�Բ��������������� Ȼ��ϲ�
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

    dwnFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),dwnFileBaseUrl.toAscii()); //����ǩ��
}

//������� ��ȡ�ض����ַ
void kpSDK::downLoadFileReplyFinished()
{
    mRet = mDownloadFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(301==mRet ||  302==mRet)//�ض�λֵ
    {
        QEventLoop   *downloadEloop=new QEventLoop();
        for(int i=0;i<mDownloadFileReply->rawHeaderPairs().count();i++)
        {
            //qDebug()<<"Pairs()"<<mDownloadFileReply->rawHeaderPairs().at(i) ;
            if( "Location"==mDownloadFileReply->rawHeaderPairs().at(i).first)
            {
                QUrl realDwnFileUrl=QUrl(mDownloadFileReply->rawHeaderPairs().at(i).second);
                mDwnFileRealReply=manager->get(QNetworkRequest(realDwnFileUrl));//���ض����ַ��������

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

//������� д���ļ�
void kpSDK::dwnFileRealReplyFinished()
{
    mRet=mDwnFileRealReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(KP_CORRECT_RESULT==mRet)//������ȷ
    {
        if (dwnFile!=NULL) dwnFile->write(mDwnFileRealReply->readAll());
    }
    dwnFile->flush();
    dwnFile->close();
    delete dwnFile;
    dwnFile=NULL;
    mDwnFileRealReply->deleteLater();
}

//--------------------------------------�����ļ�
//��������ļ�
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

//��������ļ� url
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
    //���ǩ��
    _shareFileUrl.append("&");
    _shareFileUrl.append(para);//����ޱ���Ĳ���
    return _shareFileUrl;
}

//���� �����ļ� ǩ��
//oauth_consumer_key
//oauth_nonce
//oauth_timestamp
//oauth_token
QString kpSDK::getShareFileSignature(QString &paraStr, const QString &consKeyStr, const QString &consKeySecretStr, \
                                     const QString &oauTknStr, const QString &oauTknSercetStr, \
                                     const QString &newShareFileUrl)
{
    QString shareFileBaseUrl;
    shareFileBaseUrl.clear(); //�������URI·������URL����
    shareFileBaseUrl.append("GET&");
    shareFileBaseUrl.append(QUrl::toPercentEncoding(newShareFileUrl));
    shareFileBaseUrl.append("&");

    //�Բ��������������� Ȼ��ϲ�
    addTknNonceTimes(paraStr,consKeyStr);
    paraStr.append("&");
    paraStr.append(OATOKEN);        //oauth_token
    paraStr.append(oauTknStr);

    shareFileBaseUrl.append(QUrl::toPercentEncoding(paraStr));// �Բ������б��� Ȼ��ϲ���Դ��
    QString secret=consKeySecretStr+"&";//������Կ
    secret=secret.append(oauTknSercetStr);
    return  hmacSha1(secret.toAscii(),shareFileBaseUrl.toAscii()); //����ǩ��
}

//������� ��ȡ�����url
void kpSDK::shareFileReplyFinished()
{
    mRet=mShareFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray getBuf=mShareFileReply->readAll();
    mJsonBuf.clear();
    mJsonBuf.append(getBuf);
    mShareFileReply->deleteLater();
    eLoop->exit();
}



