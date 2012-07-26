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

#ifndef PUBLICFUN_H
#define PUBLICFUN_H
#include <QString>
#include <QCryptographicHash>
#include <QUrl>
#include <QTime>
#include <time.h>
using std::string;

const int KP_CORRECT_RESULT            =200;//��ȷ�ķ��ؽ��

enum EnumTmpTokenIndex
{
    TMP_TOKEN_SECRET_INDEX=3,//���ص���ʱtokenSecret��reply�е��α�
    TMP_TOKEN_INDEX=7       //���ص���ʱtoken��reply�е��α�
};

enum EnumAuTokenIndex   //authToken
{
    AUTHOR_TOKEN_SECRET_INDEX=3,
    AUTHOR_TOKEM_INDEX=7,
    AUTHOR_USR_ID=10,
    AUTHOR_USR_DIR=13
};

enum EnumUsrInfoIndex   //�û���Ϣ
{
    USR_MAX_FILE_SIZE_INDEX=2,
    USR_TOTAL_SIZE_INDEX=4,
    USR_USED_SIZE_INDEX=6,
    USR_NAME_INDEX=9
};



#define KP_REQUEST_TOKEN_SRC_URL   "https://openapi.kuaipan.cn/open/requestToken"
#define KP_AUTHORISE_SRC_URL       "https://www.kuaipan.cn/api.php?ac=open&op=authorise&oauth_token="
#define KP_ACCESS_TOKEN_SRC_URL    "https://openapi.kuaipan.cn/open/accessToken"
#define KP_GET_USR_INFO_URL        "http://openapi.kuaipan.cn/1/account_info"
#define KP_GET_META_DATA_URL       "http://openapi.kuaipan.cn/1/metadata/"
#define KP_CREATE_FOLDER_URL       "http://openapi.kuaipan.cn/1/fileops/create_folder"
#define KP_DEL_FILE_URL            "http://openapi.kuaipan.cn/1/fileops/delete"
#define KP_MOVE_FILE_URL           "http://openapi.kuaipan.cn/1/fileops/move"
#define KP_COPY_FILE_URL           "http://openapi.kuaipan.cn/1/fileops/copy"
#define KP_UPLOAD_LOCATE_URL       "http://api-content.dfs.kuaipan.cn/1/fileops/upload_locate"
#define KP_UPLOAD_FILE_URL         "1/fileops/upload_file"
#define KP_DOWNLOAD_FILE_URL       "http://api-content.dfs.kuaipan.cn/1/fileops/download_file"
#define KP_SHARE_FILE_URL          "http://openapi.kuaipan.cn/1/shares/"

#define OANONCE                   "oauth_nonce="
#define OATIMESTAMP               "oauth_timestamp="
#define OASIGNATURE               "oauth_signature="
#define OACONSUMER_KEY            "oauth_consumer_key="
#define OATOKEN                   "oauth_token="

#define DATA_FILE_LIMIT           "file_limit="
#define DATA_FILTER_EXT           "filter_ext="
#define DATA_LIST                 "list="
#define DATA_PAGE                 "page="
#define DATA_PAGE_SIZE            "page_size="
#define DATA_SORT_BY              "sort_by="

#define FOLDER_NAME               "path="
#define FOLDER_ROOT               "root="

#define TO_RECYLE                 "to_recycle="
#define DATA_FROM_PATH            "from_path="
#define DATA_TO_PATH              "to_path="

#define UPLOAD_OVER_WRITE         "overwrite="

#define BYTE_TO_MB_UNIT           (1024*1024UL)
QString hmacSha1(QByteArray key, QByteArray baseString);
QString getRandNonce();
QString getTimeStamp();
QString removeUrlEncode(const QString &urlStr);
QString stringToUnicode(QString str);
QString unicodeToString(QString str);
QString s2q(const string &s);
string q2s(const QString &s);
#endif // PUBLICFUN_H
